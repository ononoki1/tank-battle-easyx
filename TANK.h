#pragma once
#ifndef TANK_INCLUDED
#define TANK_INCLUDED

#include "basic_define.h"

#define MAX_TANK 2
enum Direction_Cannot_Go { UP, DOWN, LEFT, RIGHT };

class TANK {
private:
  IMAGE *picture;          //̹�˵���ͼ
  IMAGE *picture_cover;    //��ͼ
  int id;                  //̹�˵�ID
  int x;                   //̹�˵�x����(��������)
  int y;                   //̹�˵�y����(��������)
  int size_x;              //̹�˵�x��С
  int size_y;              //̹�˵�y��С
  int facing;              //̹�˵ĳ���
  int weapon_id;           //̹�˵�����ID
  int blood;               //̹�˵�Ѫ��
  int speed = 1;           //̹�˵��ٶ�
  int cold_time = 0;       //������ȴʱ�䣬Ϊ����Թ���
  int use_times = -1;      //����ʹ�ô�����Ϊ-1��ʾ����ʹ��
  bool cannot_go[4] = {0}; //̹���ĸ��������ߣ�ֵΪ1��������
public:
  TANK(int Iid, int Ix, int Iy, int Ifacing);
  TANK();
  bool CanStand(int x, int y); //�ж��Ƿ�ǽ
  void Turning(int position);  //ת��
  int Hurt(int hurt);
  //����or��Ѫ
  void Move(std::list<class TANK>::iterator ite_tank);
  //�ƶ�
  int Speed() { return speed; } //��ȡ�ٶȣ��ƺ�û��...��
  bool CanMove();               //������
  void Paint();                 //���
  bool Clearable();             //�Ƿ�����

  void Shoot();    //���
  void Flash();    //ÿ֡��������
  bool CanShoot(); //�ж��Ƿ�������

  int Getx();
  int Gety(); //���������x,yֵ
  int GetxEnd();
  int GetyEnd();   //���������x,yֵ
  int Getfacing(); //��ȡ̹�˷���
  int Getid();     //��ȡ̹��ID
  int Getblood();  //��ȡ̹��Ѫ��
  int GetWeapenID();

  void ChangeWeapon(int new_weapon_id);   //������
  void ChangeCannotGo(int new_cannot_go); //���²����ߵķ���
  void ChangeCanGo(int new_can_go);       //�������ߵķ���

  void SetIterator(std::list<class TANK>::iterator ite_tank);
  void ClearIterator(std::list<class TANK>::iterator ite_tank);
  //����map
};

struct TANK_TYPE {
  IMAGE picture;       //̹����ͼ
  IMAGE picture_cover; //��ͼ
  int size_x;          //̹�˵�x��С
  int size_y;          //̹�˵�y��С
  int blood;           //̹�˵�Ѫ��
  int speed;           //̹�˵��ٶ�
  TANK_TYPE(LPCTSTR Lpicture, LPCTSTR Lpicture_cover, int Isize_x, int Isize_y,
            int Iblood, int Ispeed) {
    loadimage(&picture, Lpicture);
    loadimage(&picture_cover, Lpicture_cover);
    size_x = Isize_x;
    size_y = Isize_y;
    blood = Iblood;
    speed = Ispeed;
  }
  TANK_TYPE() {
    picture = NULL;
    size_x = 0;
    size_y = 0;
    blood = 0;
    speed = 0;
  }
};

struct WEAPON_DATA {
  int use_times;
  int cold_time;
  int force;
  WEAPON_DATA() { use_times = cold_time = force = 0; }
  WEAPON_DATA(int Iuse_times, int Icold_time, int Iforce) {
    use_times = Iuse_times;
    cold_time = Icold_time;
    force = Iforce;
  }
};

#endif