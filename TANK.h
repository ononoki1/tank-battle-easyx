#pragma once
#ifndef TANK_INCLUDED
#define TANK_INCLUDED

#include "basic_define.h"

#define MAX_TANK 2
enum Direction_Cannot_Go { UP, DOWN, LEFT, RIGHT };

class TANK {
private:
  IMAGE *picture;          //坦克的贴图
  IMAGE *picture_cover;    //掩图
  int id;                  //坦克的ID
  int x;                   //坦克的x坐标(像素坐标)
  int y;                   //坦克的y坐标(像素坐标)
  int size_x;              //坦克的x大小
  int size_y;              //坦克的y大小
  int facing;              //坦克的朝向
  int weapon_id;           //坦克的武器ID
  int blood;               //坦克的血量
  int speed = 1;           //坦克的速度
  int cold_time = 0;       //武器冷却时间，为零可以攻击
  int use_times = -1;      //武器使用次数，为-1表示无限使用
  bool cannot_go[4] = {0}; //坦克哪个方向不能走，值为1代表不能走
public:
  TANK(int Iid, int Ix, int Iy, int Ifacing);
  TANK();
  bool CanStand(int x, int y); //判断是否卡墙
  void Turning(int position);  //转向
  int Hurt(int hurt);
  //受伤or回血
  void Move(std::list<class TANK>::iterator ite_tank);
  //移动
  int Speed() { return speed; } //获取速度（似乎没用...）
  bool CanMove();               //有问题
  void Paint();                 //输出
  bool Clearable();             //是否可清除

  void Shoot();    //射击
  void Flash();    //每帧更新数据
  bool CanShoot(); //判断是否可以射击

  int Getx();
  int Gety(); //左上坐标的x,y值
  int GetxEnd();
  int GetyEnd();   //右下坐标的x,y值
  int Getfacing(); //获取坦克方向
  int Getid();     //获取坦克ID
  int Getblood();  //获取坦克血量
  int GetWeapenID();

  void ChangeWeapon(int new_weapon_id);   //换武器
  void ChangeCannotGo(int new_cannot_go); //更新不能走的方向
  void ChangeCanGo(int new_can_go);       //更新能走的方向

  void SetIterator(std::list<class TANK>::iterator ite_tank);
  void ClearIterator(std::list<class TANK>::iterator ite_tank);
  //更新map
};

struct TANK_TYPE {
  IMAGE picture;       //坦克贴图
  IMAGE picture_cover; //掩图
  int size_x;          //坦克的x大小
  int size_y;          //坦克的y大小
  int blood;           //坦克的血量
  int speed;           //坦克的速度
  TANK_TYPE(LPCTSTR Lpicture, LPCTSTR Lpicture_cover, int Isize_x, int Isize_y,
            int Iblood, int Ispeed) {
    loadimage(&picture, Lpicture);
    loadimage(&picture_cover, Lpicture_cover);
    size_x = Isize_x;
    size_y = Isize_y;
    blood = Iblood;
    speed = Ispeed;
  }
  TANK_TYPE() {
    picture = NULL;
    size_x = 0;
    size_y = 0;
    blood = 0;
    speed = 0;
  }
};

struct WEAPON_DATA {
  int use_times;
  int cold_time;
  int force;
  WEAPON_DATA() { use_times = cold_time = force = 0; }
  WEAPON_DATA(int Iuse_times, int Icold_time, int Iforce) {
    use_times = Iuse_times;
    cold_time = Icold_time;
    force = Iforce;
  }
};

#endif