#include "BLOCK.h"
#include "basic_define.h"

BLOCK block_type[MAX_BLOCK];
extern CELL map[MAX_MAP][MAX_MAP];

const static int entity_break_presentage = 10000;
const static int entity_break_probibility[MAX_ENTITY] = {0, 20, 10, 10, 5};

void BLOCK::Destroy(int x, int y, int harm, int radium) {
  int ans = id;
  for (int i = 0; i < harm; i++)
    ans = block_type[ans].next_block_number;
  map[x][y].block = &block_type[ans];
  if (id == B_BASEMENT)
    total_data.num_destroyed_basement++;
  map[x][y].block->CreateEntity(x, y);
  if (radium > 0)
    Explode(x, y, radium, 1);
  else if (is_explosive)
    Explode(x, y, explosion_radium, explosion_forces);
}

void BLOCK::CreateEntity(int x, int y) {
  if (id != 0)
    return;
  int random = rand() % entity_break_presentage;
  int i;
  for (i = 0; i < MAX_ENTITY; i++) {
    random -= entity_break_probibility[i];
    if (random < 0)
      break;
  }
  if (i != MAX_ENTITY) {
    InsertEntity(CalculateEntityid(i), CalculateEntitytype(i), ChangeToPixel(x),
                 ChangeToPixel(y));
  }
}

void BLOCK::Explode(int x, int y, int r, int forces) {
  int min_x = (x - r) > 0 ? x - r : 0;
  int min_y = (y - r) > 0 ? y - r : 0;
  int max_x = (x + r) < total_data.max_map_x ? x + r : total_data.max_map_x - 1;
  int max_y = (y + r) < total_data.max_map_y ? y + r : total_data.max_map_y - 1;
  for (int i = min_x; i <= max_x; i++)
    for (int j = min_y; j <= max_y; j++) {
      if (Check(i, j, x, y, r)) {
        map[i][j].block->Destroy(i, j, forces); //不简单递归
      }
    }
}

bool BLOCK::Check(int x1, int y1, int x2, int y2,
                  int r) //检查(x1,y1)和(x2,y2)之间的距离是不是<=r
{
  return r * r - (x1 - x2) * (x1 - x2) - (y1 - y2) * (y1 - y2) >= 0;
}

bool BLOCK::IsExplosive() { return is_explosive; }

bool BLOCK::IsPassable() { return is_passable; }

void BLOCK::Paint(int x, int y) {
  if (&picture == NULL)
    return;
  putimage(ScreenX(x), ScreenY(y), &picture_cover, SRCAND);
  putimage(ScreenX(x), ScreenY(y), &picture, SRCPAINT);
}

void BLOCK::BlockClear(int x, int y) {
  clearrectangle(x * BLOCK_SIZE, y * BLOCK_SIZE, (x + size_x) * BLOCK_SIZE,
                 (y + size_y) * BLOCK_SIZE);
}

int BLOCK::Getid() { return id; }