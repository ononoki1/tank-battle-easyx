#pragma once
#ifndef BASIC_DEFINE_INCLUDED
#define BASIC_DEFINE_INCLUDED

#define MAX_MAP 113 //��ͼ�ߴ�
#define WIN_COL 30  //��������
#define WIN_ROW 30  //��������
#define FPS 10      //��Ļˢ����

#define BLOCK_SIZE 16 //һ�����ӵ����ؿ��
#define MAX_LEVEL 2

#include "BLOCK.h"
#include "ENTITY.h"
#include "FLY.h"
#include "TANK.h"
#include "cstdio"
#include "cstdlib"
#include "cstring"
#include "ctime"
#include "easyx.h"
#include "fstream"
#include "iostream"
#include "list"
#include "windows.h"

struct DIRECTION {
  int y, x;
};

struct CELL {
  BLOCK *block;
  std::list<class TANK>::iterator tank;
  std::list<class FLY>::iterator fly;
};

struct DATA {
  int score = 0;
  int num_enemy = 0;
  int num_entity = 0;
  int num_destroyed_basement = 0;
  int enemy_flash = 0;
  int max_map_x = 0;
  int max_map_y = 0;
  int max_basement = 0;
};

extern struct DATA total_data;

void InsertTank(int id, int x, int y, int facing);
void InsertEntity(int id, int type, int x, int y);
void InsertFly(int id, int x, int y, int facing, bool disappearable = false);

extern std::list<class TANK> list_tank;
extern std::list<class ENTITY> list_entity;
extern std::list<class FLY> list_fly;
extern std::list<class TANK>::iterator list_tank_null;
extern std::list<class ENTITY>::iterator list_entity_null;
extern std::list<class FLY>::iterator list_fly_null;

std::list<class TANK>::iterator
DeleteTank(std::list<class TANK>::iterator tank_iterator);
std::list<class ENTITY>::iterator
DeleteEntity(std::list<class ENTITY>::iterator entity_iterator);
std::list<class FLY>::iterator
DeleteFly(std::list<class FLY>::iterator fly_iterator);

extern DIRECTION dir[4]; //��������
extern CELL map[MAX_MAP][MAX_MAP];
extern DWORD starttime;
extern DWORD START;
extern BLOCK block_type[MAX_BLOCK];

int ChangeToScreen(int x);
int ChangeToPixel(int x);

void Gotoxy(int x, int y); //��Ļx����Ϊ�����꣬y����Ϊ������

int Tick(int interVal);

bool IsInMap(int i, int j);

void End();
void Lose();
void Win();
void Next();

void ChangeMap(int i, int j, int B_ID);

int ScreenX(int x); //���ظ�����Ͻ�X����
int ScreenY(int y); //���ظ�����Ͻ�Y����
int ScreenXPixel(int x);
int ScreenYPixel(int y);

void LoadResources();

void ListSetup();

void LoadMap(const char *way);

std::list<class FLY>::iterator FlyCrash(std::list<class FLY>::iterator ite_fly);
std::list<class ENTITY>::iterator
EntityCrash(std::list<class ENTITY>::iterator ite_entity);

#endif