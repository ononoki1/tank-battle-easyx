#include "TANK.h"

TANK_TYPE tank_type[MAX_TANK];
WEAPON_DATA weapon_data[MAX_WEAPON];
extern FLY_TYPE fly_type[MAX_FLY];
extern std::list<class TANK>::iterator list_tank_null;

TANK::TANK(int Iid, int Ix, int Iy, int Ifacing) {
  id = Iid;
  x = Ix;
  y = Iy;
  facing = Ifacing;

  picture = &tank_type[id].picture;
  picture_cover = &tank_type[id].picture_cover;

  size_x = tank_type[id].size_x;
  size_y = tank_type[id].size_y;
  blood = tank_type[id].blood;
  speed = tank_type[id].speed;

  weapon_id = 0;
}
TANK::TANK() {
  id = -1;
  x = y = facing = size_x = size_y = blood = speed = weapon_id = 0;
  picture = NULL;
  picture_cover = NULL;
}

void TANK::Turning(int position) { facing = position; }

int TANK::Hurt(int hurt) {
  blood -= hurt;
  return blood > 0;
}

void TANK::Move(std::list<class TANK>::iterator ite_tank) {
  std::list<class TANK>::iterator tank_iterator;
  tank_iterator = map[ChangeToScreen(x)][ChangeToScreen(y)].tank;
  ClearIterator(ite_tank);
  for (int i = 0; i < speed; i++) {
    int xx = x + dir[facing].x;
    int yy = y + dir[facing].y;
    if (!CanStand(xx, yy))
      break;
    x = xx;
    y = yy;
  }
  SetIterator(ite_tank);
}

bool TANK::Clearable() { return blood <= 0; }

void TANK::Paint() {
#define PI 3.14159265
  IMAGE print_picture = NULL;
  IMAGE print_cover = NULL;
  switch (facing) {
  case UP:
    print_picture = *picture;
    print_cover = *picture_cover;
    break;
  case DOWN:
    rotateimage(&print_picture, picture, PI, BLACK);
    rotateimage(&print_cover, picture_cover, PI, WHITE);
    break;
  case LEFT:
    rotateimage(&print_picture, picture, PI / 2, BLACK);
    rotateimage(&print_cover, picture_cover, PI / 2, WHITE);
    break;
  case RIGHT:
    rotateimage(&print_picture, picture, -PI / 2, BLACK);
    rotateimage(&print_cover, picture_cover, -PI / 2, WHITE);
    break;
  }
  putimage(ScreenXPixel(x), ScreenYPixel(y), &print_cover, SRCAND);
  putimage(ScreenXPixel(x), ScreenYPixel(y), &print_picture, SRCPAINT);
#undef PI
}

bool TANK::CanStand(int x, int y) {
  int min_x = ChangeToScreen(x);
  int min_y = ChangeToScreen(y);
  int max_x = ChangeToScreen(x + size_x * BLOCK_SIZE - 1);
  int max_y = ChangeToScreen(y + size_y * BLOCK_SIZE - 1);
  for (int i = min_x; i <= max_x; i++)
    for (int j = min_y; j <= max_y; j++) {
      if (!map[i][j].block->IsPassable())
        return 0;
    }
  return 1;
}

bool TANK::CanMove() {
  return map[y + dir[facing].y][x + dir[facing].x].block->IsPassable();
}

void TANK::Shoot() {
  if (!CanShoot())
    return;
  int fly_x = -1, fly_y = -1;
  int fly_size_x = fly_type[weapon_id].size_x;
  int fly_size_y = fly_type[weapon_id].size_y;
  switch (facing) {
  case UP:
    fly_x = x + ChangeToPixel(size_x) / 2 - ChangeToPixel(fly_size_x) / 2;
    fly_y = y - ChangeToPixel(fly_size_y);
    break;
  case DOWN:
    fly_x = x + ChangeToPixel(size_x) / 2 - ChangeToPixel(fly_size_x) / 2;
    fly_y = y + ChangeToPixel(size_y);
    break;
  case LEFT:
    fly_x = x - ChangeToPixel(fly_size_x);
    fly_y = y + ChangeToPixel(size_y) / 2 - ChangeToPixel(fly_size_y) / 2;
    break;
  case RIGHT:
    fly_x = x + ChangeToPixel(size_x);
    fly_y = y + ChangeToPixel(size_y) / 2 - ChangeToPixel(fly_size_y) / 2;
    break;
  }
  InsertFly(weapon_id, fly_x, fly_y, facing);

  cold_time = weapon_data[weapon_id].cold_time;
  use_times--;
  if (use_times == 0) {
    ChangeWeapon(0);
  }
}

bool TANK::CanShoot() { return !cold_time; }

int TANK::Getx() { return x; }
int TANK::Gety() { return y; }
int TANK::GetxEnd() { return x + size_x * BLOCK_SIZE - 1; }
int TANK::GetyEnd() { return y + size_y * BLOCK_SIZE - 1; }
int TANK::Getfacing() { return facing; }
int TANK::Getid() { return id; }
int TANK::Getblood() { return blood; }

void TANK::ChangeWeapon(int new_weapon_id) {
  weapon_id = new_weapon_id;
  use_times = weapon_data[weapon_id].use_times;
}

void TANK::ChangeCannotGo(int new_cannot_go) { cannot_go[new_cannot_go] = 1; }
void TANK::ChangeCanGo(int new_can_go) { cannot_go[new_can_go] = 0; }

void TANK::SetIterator(std::list<class TANK>::iterator ite_tank) {
  int min_x = ChangeToScreen(x);
  int min_y = ChangeToScreen(y);
  int max_x = ChangeToScreen(x + size_x * BLOCK_SIZE - 1);
  int max_y = ChangeToScreen(y + size_y * BLOCK_SIZE - 1);
  for (int i = min_x; i <= max_x; i++)
    for (int j = min_y; j <= max_y; j++)
      map[i][j].tank = ite_tank;
}

void TANK::ClearIterator(std::list<class TANK>::iterator ite_tank) {
  int min_x = ChangeToScreen(x);
  int min_y = ChangeToScreen(y);
  int max_x = ChangeToScreen(x + size_x * BLOCK_SIZE - 1);
  int max_y = ChangeToScreen(y + size_y * BLOCK_SIZE - 1);
  for (int i = min_x; i <= max_x; i++)
    for (int j = min_y; j <= max_y; j++)
      if (map[i][j].tank == ite_tank)
        map[i][j].tank = list_tank_null;
}

void TANK::Flash() {
  if (cold_time != 0)
    cold_time--;
}

int TANK::GetWeapenID() { return weapon_id; }