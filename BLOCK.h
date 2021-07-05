#pragma once

#ifndef BLOCK_INCLUDED
#define BLOCK_INCLUDED

#include "conio.h"
#include "easyx.h"
#include "graphics.h"

#define B_SPACE 0x00000000
#define B_ONE 0x00000001
#define B_TWO 0x00000002
#define B_THREE 0x00000003
#define B_FOUR 0x00000004
#define B_FIVE 0x00000005
#define B_TNT 0x00000006
#define B_BEDROCK 0x00000007
#define B_SHIELD 0x00000008
#define B_BASEMENT 0x00000009
#define MAX_BLOCK 0x0000000A

class BLOCK {
private:
  IMAGE picture;
  IMAGE picture_cover;
  int id; //����ID
  int size_x;            // x�����С
  int size_y;            // y�����С
  int explosion_radium;  //��ը�뾶
  int explosion_forces;  //��ը����
  int next_block_number; //�ݻٺ��Ϊ�ķ���
  bool is_explosive;     //�Ƿ�ɱ�ը
  bool is_passable;      //��ͨ��
  bool is_cover;         //�Ƿ������巽��

  bool Check(int x1, int y1, int x2, int y2, int r);

public:
  void Explode(int x, int y, int r, int forces); //���鱬ը
  void Destroy(int x, int y, int harm = 1, int radium = 0);
  //�������˴ݻ�
  void CreateEntity(int x, int y); //���ɵ���

  bool IsExplosive();                 //�����Ƿ�ɱ�ը
  bool IsPassable();                  //�����Ƿ��ͨ��
  bool IsCover() { return is_cover; } //�����Ƿ�������

  void Paint(int x, int y); //��ͼ
  int Getid();                   //��ȡ����id
  void BlockClear(int x, int y); //�ƺ�û��...

  BLOCK(LPCTSTR Lway, LPCTSTR Lcover, int Iid, int Isize_x, int Isize_y,
        int Iexplosion_radium, int Iexplosion_forces, int Inext_block_number,
        bool Bis_explosive, bool Bis_passable, bool Bis_cover) {
    loadimage(&picture, Lway);
    loadimage(&picture_cover, Lcover);
    id = Iid;
    size_x = Isize_x;
    size_y = Isize_y;
    explosion_radium = Iexplosion_radium;
    explosion_forces = Iexplosion_forces;
    next_block_number = Inext_block_number;
    is_explosive = Bis_explosive;
    is_passable = Bis_passable;
    is_cover = Bis_cover;
  }

  BLOCK() {
    picture = NULL;
    id = -1;
    size_x = 0;
    size_y = 0;
    explosion_radium = -1;
    explosion_forces = -1;
    next_block_number = -1;
    is_explosive = false;
    is_passable = false;
    is_cover = false;
  }
};

#endif