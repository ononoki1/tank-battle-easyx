#include "basic_define.h"

extern std::list<class TANK>::iterator list_tank_null;
extern std::list<class ENTITY>::iterator list_entity_null;
extern std::list<class FLY>::iterator list_fly_null;
extern WEAPON_DATA weapon_data[MAX_WEAPON];

static bool IsCrash(int x1, int y1, int x2, int y2, int x3, int y3, int x4,
                    int y4);

std::list<class FLY>::iterator
FlyCrash(std::list<class FLY>::iterator ite_fly) {
  if (ite_fly->Getspeed() < -3 || ite_fly->Getspeed() == 0)
    return ite_fly;
  int min_x = ChangeToScreen(ite_fly->Getx());
  int min_y = ChangeToScreen(ite_fly->Gety());
  int max_x = ChangeToScreen(ite_fly->GetxEnd());
  int max_y = ChangeToScreen(ite_fly->GetyEnd());
  int force = weapon_data[ite_fly->Getid()].force;
  int radium = ite_fly->GetExplosionRadium();
  for (int i = min_x; i <= max_x; i++)
    for (int j = min_y; j <= max_y; j++)
      if (IsInMap(i, j)) {
        if (!map[i][j].block->IsPassable()) {
          map[i][j].block->Destroy(i, j, force, radium);
          int t_x, t_y;
          switch (ite_fly->Getfacing()) {
          case UP:
          case DOWN:
            t_x = i + dir[LEFT].x;
            t_y = j + dir[LEFT].y;
            if (IsInMap(t_x, t_y)) {
              if (!map[t_x][t_y].block->IsPassable()) {
                map[t_x][t_y].block->Destroy(t_x, t_y, force);
              }
            }
            t_x = i + dir[RIGHT].x;
            t_y = j + dir[RIGHT].y;
            if (IsInMap(t_x, t_y)) {
              if (!map[t_x][t_y].block->IsPassable()) {
                map[t_x][t_y].block->Destroy(t_x, t_y, force);
              }
            }
            break;
          case LEFT:
          case RIGHT:
            t_x = i + dir[UP].x;
            t_y = j + dir[UP].y;
            if (IsInMap(t_x, t_y)) {
              if (!map[t_x][t_y].block->IsPassable()) {
                map[t_x][t_y].block->Destroy(t_x, t_y, force);
              }
            }
            t_x = i + dir[DOWN].x;
            t_y = j + dir[DOWN].y;
            if (IsInMap(t_x, t_y)) {
              if (!map[t_x][t_y].block->IsPassable()) {
                map[t_x][t_y].block->Destroy(t_x, t_y, force);
              }
            }
            break;
          }
          return DeleteFly(ite_fly);
          break;
        }
        if (map[i][j].tank != list_tank_null) {
          if (IsCrash(map[i][j].tank->Getx(), map[i][j].tank->Gety(),
                      map[i][j].tank->GetxEnd(), map[i][j].tank->GetyEnd(),
                      ite_fly->Getx(), ite_fly->Gety(), ite_fly->GetxEnd(),
                      ite_fly->GetyEnd())) //�˸������ж��Ƿ���ײ
          {
            map[i][j].tank->Hurt(force);
            if (map[i][j].tank->Clearable()) {
              if (map[i][j].tank->Getid() == 0) {
                Lose();
              }
              DeleteTank(map[i][j].tank);
            }
            return DeleteFly(ite_fly);
          }
        }
      }
  return ite_fly;
}

std::list<class ENTITY>::iterator
EntityCrash(std::list<class ENTITY>::iterator ite_entity) {
  int min_x = ChangeToScreen(ite_entity->Getx());
  int min_y = ChangeToScreen(ite_entity->Gety());
  int max_x = ChangeToScreen(ite_entity->GetxEnd());
  int max_y = ChangeToScreen(ite_entity->GetyEnd());
  for (int i = min_x; i <= max_x; i++)
    for (int j = min_y; j <= max_y; j++)
      if (IsInMap(i, j)) {
        if (map[i][j].tank != list_tank_null) {
          ite_entity->GetEntity(map[i][j].tank);
          if (ite_entity->Clearable()) {
            return DeleteEntity(ite_entity);
          }
        }
      }
  return ite_entity;
}

static bool IsCrash(int x1, int y1, int x2, int y2, int x3, int y3, int x4,
                    int y4) {
  if (x3 > x2 || x4 < x1)
    return false;
  if (y3 > y2 || y4 < y1)
    return false;
  return true;
}