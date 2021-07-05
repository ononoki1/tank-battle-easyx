#include "BLOCK.h"
#include "ENTITY.h"
#include "FLY.h"
#include "TANK.h"
#include "basic_define.h"

#define MAX_ENEMY 100
using namespace std;
int scr_x, scr_y;
extern BLOCK block_type[MAX_BLOCK];

static int level = 0;

const int ENEMY_FLASH_TIME = 100;

struct DATA total_data;

void Print();
bool DataChange(list<class TANK>::iterator &a);
void Init();
void GenerateRandomMap();
void Begin();
void GenerateEnemy();
bool IsBase(int i, int j);
void PaintBackground(int i, int j);
void CharToTchar(char *_char, TCHAR *tchar);
void NextLevel();
void OutsidePtint();

std::list<class FLY>::iterator FlyCrash(list<class FLY>::iterator fly);

int main() {
  srand((unsigned int)time(NULL));
  initgraph((WIN_COL + 20) * BLOCK_SIZE, WIN_ROW * BLOCK_SIZE);
  Init();
  _getch();
  Begin();

  bool flash;

  while (1) {
    if (GetKeyState(VK_F2) < 0) {
      cleardevice();
      clearcliprgn();
      outtextxy(((WIN_COL + 20) / 2 - 2) * BLOCK_SIZE,
                (WIN_ROW / 3) * BLOCK_SIZE, _T("暂停"));
      starttime = GetTickCount();
      while (1) {
        if (GetKeyState(VK_F2) > 0) {
          starttime = GetTickCount();
          OutsidePtint();
          Print();
          break;
        }
      }
    }

    static DWORD mytime = GetTickCount();
    if (mytime - starttime < 1000 / FPS) //帧率控制  20FPS
    {
      Sleep(1000 / FPS - (mytime - starttime));
    }

    std::list<class TANK>::iterator me = list_tank.begin();
    me++;
    flash = DataChange(me);
    if (flash)
      Print();
    if (me->Getblood() <= 0) {
      Lose();
      break;
    }
    if (GetKeyState(VK_F1) < 0) {
      End();
      break;
    }
    if (total_data.num_destroyed_basement >= total_data.max_basement) {
      NextLevel();
    }
  }

  return 0;
}

void Init() {
  LoadResources();
  setfillcolor(BLACK);
  TCHAR s1[] = _T("坦克大战");
  TCHAR s2[] = _T("开始游戏");

  settextstyle(BLOCK_SIZE, 0, _T("宋体"));
  outtextxy(((WIN_COL + 20) / 2 - 2) * BLOCK_SIZE, (WIN_ROW / 3) * BLOCK_SIZE,
            s1);
  outtextxy(((WIN_COL + 20) / 2 - 2) * BLOCK_SIZE,
            (WIN_ROW / 3 + 2) * BLOCK_SIZE, s2);
}

void Begin() {
  NextLevel();
  OutsidePtint();
  char sc[20];
  TCHAR st[20];
  scr_y = (total_data.max_map_y - 3) * BLOCK_SIZE + BLOCK_SIZE / 2;
  scr_x = (total_data.max_map_x / 2) * BLOCK_SIZE + BLOCK_SIZE / 2;
  Print();
}

bool DataChange(list<class TANK>::iterator &me) {
  bool ans = 0;
  if (GetAsyncKeyState(VK_UP)) {
    me->Turning(UP);
    {
      me->Move(me);
      scr_y = me->Gety() + BLOCK_SIZE * 3 / 2;
    }
    ans = 1;
  }
  if (GetAsyncKeyState(VK_LEFT)) {
    me->Turning(LEFT);
    {
      me->Move(me);
      scr_x = me->Getx() + BLOCK_SIZE * 3 / 2;
    }
    ans = 1;
  }
  if (GetAsyncKeyState(VK_DOWN)) {
    me->Turning(DOWN);
    {
      me->Move(me);
      scr_y = me->Gety() + BLOCK_SIZE * 3 / 2;
    }
    ans = 1;
  }
  if (GetAsyncKeyState(VK_RIGHT)) {
    me->Turning(RIGHT);
    {
      me->Move(me);
      scr_x = me->Getx() + BLOCK_SIZE * 3 / 2;
    }
    ans = 1;
  }
  if (GetAsyncKeyState(VK_SPACE) && me->CanShoot()) {
    me->Shoot();
    ans = 1;
  }
  me->Flash();

  for (list<class FLY>::iterator it = list_fly_null, useless = --it;
       it != list_fly.begin(); it--) {
    it->Move(it);
    it = FlyCrash(it);
  }
  for (list<class ENTITY>::iterator it = list_entity_null, useless = --it;
       it != list_entity.begin(); it--) {
    it = EntityCrash(it);
  }
  if (total_data.num_destroyed_basement == total_data.max_basement) {
    Begin();
    me = list_tank.begin();
    me++;
    return 1;
  }

  {
    GenerateEnemy();
    list<class TANK>::iterator it = list_tank.begin();
    it++;
    it++;
    for (; it != list_tank_null; it++)

    {
      it->Move(it);
      if (rand() % 5 == 0)
        it->Turning(rand() % 4);
      if (rand() % 10 == 0)
        it->Shoot();
      it->Flash();
    }
  }

  return 1;
}

void NextLevel() {
  level++;
  switch (level) {
  case 1:
    LoadMap("resources\\levels\\level1.txt");
    break;
  case 2:
    Next();
    LoadMap("resources\\levels\\level2.txt");
    break;
  default:
    Win();
  }
}

void Print() {
  HRGN rgn = CreateRectRgn(0, 0, WIN_COL * BLOCK_SIZE, WIN_ROW * BLOCK_SIZE);
  setcliprgn(rgn);
  DeleteObject(rgn);
  BeginBatchDraw();

  int up = scr_y / BLOCK_SIZE - WIN_ROW / 2;
  int down = scr_y / BLOCK_SIZE + WIN_ROW / 2;
  int left = scr_x / BLOCK_SIZE - WIN_COL / 2;
  int right = scr_x / BLOCK_SIZE + WIN_COL / 2;
  for (int i = left; i <= right; i++)
    for (int j = up; j <= down; j++)
      if (IsInMap(i, j))
        if (!map[i][j].block->IsCover())
          map[i][j].block->Paint(i, j);
        else
          block_type[0].Paint(i, j);
      else
        PaintBackground(i, j);
  for (list<class TANK>::iterator it = list_tank_null, useless = --it;
       it != list_tank.begin(); it--) {
    if (it->Clearable()) {
      if (it->Getid() == 0) {
        Lose();
      }
      it = DeleteTank(it);
    } else
      it->Paint();
  }
  for (list<class ENTITY>::iterator it = list_entity_null, useless = --it;
       it != list_entity.begin(); it--)
    it->Paint();
  for (list<class FLY>::iterator it = list_fly_null, useless = --it;
       it != list_fly.begin(); it--) {
    if (it->Clearable())
      it = DeleteFly(it);
    else
      it->Paint();
  }
  for (int i = left; i <= right; i++)
    for (int j = up; j <= down; j++)
      if (IsInMap(i, j))
        if (map[i][j].block->IsCover())
          map[i][j].block->Paint(i, j);
  EndBatchDraw();
  setcliprgn(NULL);

  char sc[20];
  TCHAR st[20];
  std::list<class TANK>::iterator me = list_tank.begin();
  me++;

  switch (me->GetWeapenID()) {
  case 0:
    outtextxy((WIN_COL + 2) * BLOCK_SIZE, (WIN_ROW * 3 / 4 - 3) * BLOCK_SIZE,
              _T("普通"));
    break;
  case 2:
    outtextxy((WIN_COL + 2) * BLOCK_SIZE, (WIN_ROW * 3 / 4 - 3) * BLOCK_SIZE,
              _T("激光"));
    break;
  case 1:
    outtextxy((WIN_COL + 2) * BLOCK_SIZE, (WIN_ROW * 3 / 4 - 3) * BLOCK_SIZE,
              _T("爆破"));
    break;
  }

  sprintf_s(sc, 20, "%3d",
            total_data.max_basement - total_data.num_destroyed_basement);
  CharToTchar(sc, st);
  outtextxy((WIN_COL + 1) * BLOCK_SIZE, (WIN_ROW * 3 / 4 - 1) * BLOCK_SIZE,
            st); //基地数

  sprintf_s(sc, 20, "%3d", me->Getblood());
  CharToTchar(sc, st);
  outtextxy((WIN_COL + 1) * BLOCK_SIZE, (WIN_ROW * 3 / 4 + 1) * BLOCK_SIZE,
            st); //血量
}

void GenerateRandomMap() {
  LoadMap("resources\\levels\\level1.txt");
}

bool IsBase(int i, int j) {
  if (j == 1 && (i == MAX_MAP / 4 || i == MAX_MAP / 2 || i == MAX_MAP * 3 / 4))
    return 1;
  if (i == 1 && (j == MAX_MAP / 4 || j == MAX_MAP / 2 || j == MAX_MAP * 3 / 4))
    return 1;
  if (i == MAX_MAP - 2 &&
      (j == MAX_MAP / 4 || j == MAX_MAP / 2 || j == MAX_MAP * 3 / 4))
    return 1;
  return 0;
}

void GenerateEnemy() {
  total_data.enemy_flash++;
  total_data.enemy_flash %= ENEMY_FLASH_TIME;

  if (!total_data.enemy_flash && total_data.num_enemy < MAX_ENEMY) {
    int tx = rand() % WIN_COL;
    int ty = rand() % WIN_ROW;
    if (rand() % 2 == 0)
      tx = -tx;
    if (rand() % 2 == 0)
      ty = -ty;
    tx += scr_x / BLOCK_SIZE;
    ty += scr_y / BLOCK_SIZE;
    for (int i = tx; i <= tx + 2; i++)
      for (int j = ty; j <= ty + 2; j++)
        if (!IsInMap(i, j))
          return;
        else if (!map[i][j].block->IsPassable())
          return;
    InsertTank(1, tx * BLOCK_SIZE, ty * BLOCK_SIZE, UP);
    std::list<class TANK>::iterator it;
    it = list_tank.end();
    it--;
    it->SetIterator(it);
    total_data.num_enemy++;
  }
}

void PaintBackground(int i, int j) {
  int top = ScreenY(j);
  int left = ScreenX(i);
  int right = max(ScreenX(i), (WIN_COL * 2 + 1) * BLOCK_SIZE / 2 - 1);
  fillrectangle(left, top, right, top + BLOCK_SIZE);
}

void CharToTchar(char *_char, TCHAR *tchar) {
  int iLength;

  iLength = MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, NULL, 0);
  MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, tchar, iLength);
}

void OutsidePtint() {
  cleardevice();
  setlinecolor(RED);
  line((WIN_COL * 2 + 1) * BLOCK_SIZE / 2, 0,
       (WIN_COL * 2 + 1) * BLOCK_SIZE / 2, WIN_ROW * BLOCK_SIZE);
  setlinecolor(BLACK);
  outtextxy((WIN_COL + 1) * BLOCK_SIZE, (WIN_ROW / 4 - 2) * BLOCK_SIZE,
            _T("游戏状态："));
  outtextxy((WIN_COL + 1) * BLOCK_SIZE, (WIN_ROW / 4) * BLOCK_SIZE,
            _T("开始！"));
  outtextxy((WIN_COL + 1) * BLOCK_SIZE, (WIN_ROW / 4 + 3) * BLOCK_SIZE,
            _T("操作方法："));
  outtextxy((WIN_COL + 1) * BLOCK_SIZE, (WIN_ROW / 4 + 4) * BLOCK_SIZE,
            _T("方向键操作坦克"));
  outtextxy((WIN_COL + 1) * BLOCK_SIZE, (WIN_ROW / 4 + 5) * BLOCK_SIZE,
            _T("空格开火"));
  outtextxy((WIN_COL + 1) * BLOCK_SIZE, (WIN_ROW / 4 + 6) * BLOCK_SIZE,
            _T("F2暂停"));
  outtextxy((WIN_COL + 1) * BLOCK_SIZE, (WIN_ROW / 4 + 7) * BLOCK_SIZE,
            _T("F1退出"));

  outtextxy((WIN_COL + 1) * BLOCK_SIZE, (WIN_ROW * 3 / 4 - 2) * BLOCK_SIZE,
            _T("剩余基地："));
  outtextxy((WIN_COL + 1) * BLOCK_SIZE, (WIN_ROW * 3 / 4 - 4) * BLOCK_SIZE,
            _T("武器："));
  outtextxy((WIN_COL + 1) * BLOCK_SIZE, (WIN_ROW * 3 / 4) * BLOCK_SIZE,
            _T("生命："));
}