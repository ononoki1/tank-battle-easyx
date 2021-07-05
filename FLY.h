#pragma once
#ifndef FLY_INCLUDED
#define FLY_INCLUDED

#include "basic_define.h"
#define MAX_FLY 3

class FLY {
private:
  IMAGE *picture;       //�ӵ�����ͼ
  IMAGE *picture_cover; //��ͼ
  int id;               //�ӵ���ID
  int x;                //�ӵ���x����(��������)
  int y;                //�ӵ���y����(��������)
  int size_x;           //�ӵ���x��С
  int size_y;           //�ӵ���y��С
  int facing;           //�ӵ��ĳ���
  int speed;            //�ӵ����ٶ�
  int explosion_radium; //�ӵ��ı��ư뾶
  void FillingFly(); //ר����Լ����Move�����ĸ�������
  bool from_i; //�ӵ��Ƿ������ҵ�̹��
public:
  FLY(int Iid, int Ix, int Iy, int Ifacing, bool Bdisappearable = false);
  FLY();
  bool CanStand(int x, int y); //�Ƿ�ǽ
  void Turning(int position);  //ת��
  void Move(std::list<class FLY>::iterator ite_fly);
  //�ƶ�
  void Paint();     //��ͼ
  bool Clearable(); //�Ƿ�����

  int Getx();
  int Gety(); //���������x,yֵ
  int GetxEnd();
  int GetyEnd();            //���������x,yֵ
  int Getid();              //��ȡ�ӵ�id
  int Getfacing();          //��ȡ�ӵ�����
  int GetExplosionRadium(); //��ȡ�ӵ���ը�뾶
  int Getspeed();           //��ȡ�ӵ��ٶ�

  bool IsAlive(); //�ӵ��Ƿ����
  bool IsFromI();
  void SetIterator(std::list<class FLY>::iterator ite_fly);
  void ClearIterator(std::list<class FLY>::iterator ite_fly);
  //����map
};

struct FLY_TYPE {
  IMAGE picture;        //�ӵ���ͼ
  IMAGE picture_cover;  //����
  int size_x;           //�ӵ���x��С
  int size_y;           //�ӵ���y��С
  int speed;            //�ӵ����ٶ�
  int explosion_radium; //�ӵ��ı��ư뾶
  FLY_TYPE(LPCTSTR Lpicture, LPCTSTR Lpicture_cover, int Isize_x, int Isize_y,
           int Ispeed, int Iexplosion_radium) {
    loadimage(&picture, Lpicture);
    loadimage(&picture_cover, Lpicture_cover);
    size_x = Isize_x;
    size_y = Isize_y;
    speed = Ispeed;
    explosion_radium = Iexplosion_radium;
  }
  FLY_TYPE() {
    picture = NULL;
    size_x = 0;
    size_y = 0;
    speed = 0;
  }
};

#endif