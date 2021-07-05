#include "basic_define.h"

std::list<class TANK> list_tank;
std::list<class ENTITY> list_entity;
std::list<class FLY> list_fly;
std::list<class TANK>::iterator list_tank_null;
std::list<class ENTITY>::iterator list_entity_null;
std::list<class FLY>::iterator list_fly_null;

void ListSetup() //��������ȵ����������,���������Լ���ʼ��
{
  list_tank.resize(1);
  list_entity.resize(1);
  list_fly.resize(1);
  list_tank_null = list_tank.end();
  list_entity_null = list_entity.end();
  list_fly_null = list_fly.end();
}

void ListClear() {
  list_tank.clear();
  list_entity.clear();
  list_fly.clear();
}

void InsertTank(int id, int x, int y, int facing) {
  list_tank.insert(list_tank_null, TANK(id, x, y, facing));
}

void InsertEntity(int id, int type, int x, int y) {
  list_entity.push_back(ENTITY(id, type, x, y));
}

void InsertFly(int id, int x, int y, int facing, bool disappearable) {
  list_fly.push_back(FLY(id, x, y, facing, disappearable));
}

std::list<class TANK>::iterator
DeleteTank(std::list<class TANK>::iterator tank_iterator) {
  tank_iterator->ClearIterator(tank_iterator);
  return list_tank.erase(tank_iterator);
}

std::list<class ENTITY>::iterator
DeleteEntity(std::list<class ENTITY>::iterator entity_iterator) {
  return list_entity.erase(entity_iterator);
}

std::list<class FLY>::iterator
DeleteFly(std::list<class FLY>::iterator fly_iterator) {
  fly_iterator->ClearIterator(fly_iterator);
  return list_fly.erase(fly_iterator);
}
