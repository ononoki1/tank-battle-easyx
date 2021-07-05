#include "ENTITY.h"

ENTITY_TYPE entity_type[MAX_ENTITY];
static const int entity_type_number[MAX_TYPE] = {MAX_WEAPON, MAX_HEAL};
extern std::list<class ENTITY>::iterator list_entity_null;

void ENTITY::GetEntity(std::list<TANK>::iterator tank) {
  switch (type) {
  case WEAPON:
    tank->ChangeWeapon(id);
    break;
  case HEAL:
    switch (id) {
    case HE_SMALL:
      tank->Hurt(-SMALL_HEALING_BLOOD);
      break;
    case HE_BIG:
      tank->Hurt(-BIG_HEALING_BLOOD);
      break;
    }
    break;
  }
  clearable = true;
}

ENTITY::ENTITY(int Iid, int Itype, int Ix, int Iy) {
  id = Iid;
  type = Itype;
  x = Ix;
  y = Iy;
  int number = 0;
  for (int i = 0; i < type; i++)
    number += entity_type_number[i];
  number += id;
  picture = &entity_type[number].picture;
  picture_cover = &entity_type[number].picture_cover;
}
ENTITY::ENTITY() {
  picture = picture_cover = NULL;
  id = -1;
  type = -1;
  x = 0;
  y = 0;
}
void ENTITY::Paint() {
  putimage(ScreenXPixel(x), ScreenYPixel(y), picture_cover, SRCAND);
  putimage(ScreenXPixel(x), ScreenYPixel(y), picture, SRCPAINT);
}

int ENTITY::Getid() { return id; }
int ENTITY::Gettype() { return type; }
int ENTITY::Getx() { return x; }
int ENTITY::Gety() { return y; }
int ENTITY::GetxEnd() { return x + BLOCK_SIZE - 1; }
int ENTITY::GetyEnd() { return y + BLOCK_SIZE - 1; }

int CalculateEntityid(int entity_number) {
  int i = 0;
  for (i = 0; i < MAX_TYPE; i++) {
    entity_number -= entity_type_number[i];
    if (entity_number < 0)
      break;
  }
  if (i == MAX_TYPE)
    return -1;
  return entity_number + entity_type_number[i];
}

int CalculateEntitytype(int entity_number) {
  int i = 0;
  for (i = 0; i < MAX_TYPE; i++) {
    entity_number -= entity_type_number[i];
    if (entity_number < 0)
      break;
  }
  return i;
}