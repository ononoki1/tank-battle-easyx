#include "basic_define.h"

extern BLOCK block_type[MAX_BLOCK];
extern TANK_TYPE tank_type[MAX_TANK];
extern FLY_TYPE fly_type[MAX_FLY];
extern ENTITY_TYPE entity_type[MAX_ENTITY];
extern WEAPON_DATA weapon_data[MAX_WEAPON];

extern std::list<class TANK> list_tank;
extern std::list<class ENTITY> list_entity;
extern std::list<class FLY> list_fly;
extern std::list<class TANK>::iterator list_tank_null;
extern std::list<class ENTITY>::iterator list_entity_null;
extern std::list<class FLY>::iterator list_fly_null;

DWORD starttime;
DWORD START;

DIRECTION dir[4] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

CELL map[MAX_MAP][MAX_MAP];

extern int scr_x, scr_y;

int ChangeToScreen(int x) { return x / BLOCK_SIZE; }
int ChangeToPixel(int x) { return x * BLOCK_SIZE; }

void End() {
  cleardevice();
  outtextxy(((WIN_COL + 20) / 2 - 2) * BLOCK_SIZE, (WIN_ROW / 3) * BLOCK_SIZE,
            _T("��Ϸ����"));
  _getch();
}

bool IsInMap(int i, int j) {
  return i >= 0 && i < total_data.max_map_x && j >= 0 &&
         j < total_data.max_map_y;
}

void Gotoxy(int x, int y) {
  HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
  COORD pos;
  pos.X = x;
  pos.Y = y;
  SetConsoleCursorPosition(handle, pos);
}

int Tick(int interVal) {
  DWORD time = GetTickCount();
  if (START - time % interVal == 0)
    return 1;
  else
    return 0;
}

int ScreenX(int x) { return x * BLOCK_SIZE - scr_x + WIN_COL * BLOCK_SIZE / 2; }

int ScreenY(int y) { return y * BLOCK_SIZE - scr_y + WIN_ROW * BLOCK_SIZE / 2; }

int ScreenXPixel(int x) { return x - scr_x + WIN_COL * BLOCK_SIZE / 2; }

int ScreenYPixel(int y) { return y - scr_y + WIN_ROW * BLOCK_SIZE / 2; }

void LoadResources() {
  block_type[0] =
      BLOCK(_T("resources\\block\\·.gif"), _T("resources\\block\\mask.gif"),
            B_SPACE, 1, 1, 0, 0, 0, false, true, false);
  block_type[1] =
      BLOCK(_T("resources\\block\\ǽ1.gif"), _T("resources\\block\\mask.gif"),
            B_ONE, 1, 1, 0, 0, 0, false, false, false);
  block_type[2] =
      BLOCK(_T("resources\\block\\ǽ2.gif"), _T("resources\\block\\mask.gif"),
            B_TWO, 1, 1, 0, 0, 1, false, false, false);
  block_type[3] =
      BLOCK(_T("resources\\block\\ǽ3.gif"), _T("resources\\block\\mask.gif"),
            B_THREE, 1, 1, 0, 0, 2, false, false, false);
  block_type[4] =
      BLOCK(_T("resources\\block\\ǽ4.gif"), _T("resources\\block\\mask.gif"),
            B_FOUR, 1, 1, 0, 0, 3, false, false, false);
  block_type[5] =
      BLOCK(_T("resources\\block\\ǽ5.gif"), _T("resources\\block\\mask.gif"),
            B_FIVE, 1, 1, 0, 0, 4, false, false, false);
  block_type[6] =
      BLOCK(_T("resources\\block\\TNT.gif"), _T("resources\\block\\mask.gif"),
            B_TNT, 1, 1, 3, 5, 0, true, false, false);
  block_type[7] =
      BLOCK(_T("resources\\block\\����.gif"), _T("resources\\block\\mask.gif"),
            B_BEDROCK, 1, 1, 0, 0, 7, false, false, false);
  block_type[8] = BLOCK(_T("resources\\block\\����.gif"),
                        _T("resources\\block\\����_mask.gif"), B_SHIELD, 1, 1,
                        0, 0, 0, false, true, true);
  block_type[9] =
      BLOCK(_T("resources\\block\\����.gif"), _T("resources\\block\\mask.gif"),
            B_BASEMENT, 1, 1, 0, 0, 0, false, false, false);
  ////////////////
  tank_type[0] =
      TANK_TYPE(_T("resources\\tank\\����̹��.gif"),
                _T("resources\\tank\\����̹��_mask.gif"), 3, 3, 1, 2);
  tank_type[1] =
      TANK_TYPE(_T("resources\\tank\\�з�̹��.gif"),
                _T("resources\\tank\\�з�̹��_mask.gif"), 3, 3, 1, 2);
  ////////////////
  fly_type[0] = FLY_TYPE(_T("resources\\fly\\��ͨ.gif"),
                         _T("resources\\fly\\��ͨ_mask.gif"), 1, 1, 5, 0);
  fly_type[1] = FLY_TYPE(_T("resources\\fly\\����.gif"),
                         _T("resources\\fly\\����_mask.gif"), 1, 1, 3, 2);
  fly_type[2] =
      FLY_TYPE(_T("resources\\fly\\����.gif"),
               _T("resources\\fly\\����_mask.gif"), 1, 1, -1, 0); //-1��ʾ˲ʱ
  ////////////////
  entity_type[0] = ENTITY_TYPE(_T("resources\\fly\\��ͨ.gif"),
                               _T("resources\\fly\\��ͨ_mask.gif"), 0, 0);
  entity_type[1] = ENTITY_TYPE(_T("resources\\entity\\����.gif"),
                               _T("resources\\entity\\����_mask.gif"), 0, 1);
  entity_type[2] = ENTITY_TYPE(_T("resources\\entity\\����.gif"),
                               _T("resources\\entity\\����_mask.gif"), 0, 2);

  entity_type[3] = ENTITY_TYPE(_T("resources\\entity\\��Ѫ.gif"),
                               _T("resources\\entity\\��Ѫ_mask.gif"), 1, 0);
  entity_type[4] = ENTITY_TYPE(_T("resources\\entity\\���Ѫ.gif"),
                               _T("resources\\entity\\���Ѫ_mask.gif"), 1, 2);
  ////////////////
  weapon_data[0] = WEAPON_DATA(-1, 40, 1);
  weapon_data[1] = WEAPON_DATA(3, 80, 2);
  weapon_data[2] = WEAPON_DATA(3, 80, 1);
}

void Lose() {
  cleardevice();
  outtextxy(((WIN_COL + 20) / 2 - 2) * BLOCK_SIZE, (WIN_ROW / 3) * BLOCK_SIZE,
            _T("�����ˣ�"));
  Sleep(10000);
  _getch();
  exit(0);
}

void Win() {
  cleardevice();
  outtextxy(((WIN_COL + 20) / 2 - 2) * BLOCK_SIZE, (WIN_ROW / 3) * BLOCK_SIZE,
            _T("��Ӯ�ˣ�"));
  Sleep(1000);
  _getch();
  exit(0);
}

void ListClear();
void Next() {
  cleardevice();
  outtextxy(((WIN_COL + 20) / 2 - 2) * BLOCK_SIZE, (WIN_ROW / 3) * BLOCK_SIZE,
            _T("��һ��"));
  Sleep(1000);
  _getch();
  ListClear();
}

void LoadMap(const char *way) {
  ListSetup();
  std::ifstream fp;
  fp.open(way, std::ios::in);
  int x, y, basement, tank_x, tank_y;
  fp >> y >> x >> basement >> tank_y >> tank_x;
  total_data.max_map_x = x;
  total_data.max_map_y = y;
  total_data.max_basement = basement;
  total_data.num_destroyed_basement = 0;
  for (int i = 0; i < y; i++)
    for (int j = 0; j < x; j++) {
      int block_id;
      fp >> block_id;
      map[j][i].block = &block_type[block_id];
      map[j][i].fly = list_fly_null;
      map[j][i].tank = list_tank_null;
    }
  fp.close();
  InsertTank(0, ChangeToPixel(tank_x), ChangeToPixel(tank_y), UP);
}
