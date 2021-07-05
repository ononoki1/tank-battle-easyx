#pragma once
#ifndef ENTITY_INCLUDED
#define ENTITY_INCLUDED

#include "basic_define.h"

#define WEAPON 0

#define WE_BASIC 0
#define WE_EXPLOSION 1
#define WE_LASER 2
#define MAX_WEAPON 3

#define HEAL 1

#define HE_SMALL 0
#define SMALL_HEALING_BLOOD 1
#define HE_BIG 1
#define BIG_HEALING_BLOOD 3
#define MAX_HEAL 2

#define MAX_ENTITY 5
#define MAX_TYPE 2

class ENTITY {
private:
  IMAGE *picture;         //��ͼ
  IMAGE *picture_cover;   //��ͼ
  int id;                 //ʵ���id
  int type;               //ʵ�������
  int x;                  //ʵ���x����(��������)
  int y;                  //ʵ���y����(��������)
  bool clearable = false; //�ж��Ƿ�����
public:
  ENTITY(int Iid, int Itype, int Ix, int Iy);
  ENTITY();
  void GetEntity(std::list<class TANK>::iterator tank);
  //̹�˻�õ���
  bool Clearable() { return clearable; } //�ж��Ƿ�����
  void Paint();                          //���

  int Getid();   //��ȡʵ��id
  int Gettype(); //��ȡʵ������
  int Getx();
  int Gety(); //���������x,yֵ
  int GetxEnd();
  int GetyEnd(); //���������x,yֵ
};

struct ENTITY_TYPE {
  IMAGE picture;       //��ͼ
  IMAGE picture_cover; //��ͼ
  int id;
  int type;
  ENTITY_TYPE(LPCTSTR Lpicture, LPCTSTR Lpicture_cover, int Itype, int Iid) {
    loadimage(&picture, Lpicture);
    loadimage(&picture_cover, Lpicture_cover);
    id = Iid;
    type = Itype;
  }
  ENTITY_TYPE() {
    picture = picture_cover = NULL;
    id = type = -1;
  }
};

int CalculateEntityid(int entity_number);
int CalculateEntitytype(int entity_number);

#endif