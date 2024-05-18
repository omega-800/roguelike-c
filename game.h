#ifndef GAME_H
#define GAME_H

#include <string.h>
#include "constants.h"
#include "character.h"
#include "map.h"
#include "print.h"

typedef struct level {
  unsigned char depth;
  unsigned char enemies;
  unsigned char enemy_lvl_min;
  unsigned char enemy_lvl_max;
  snode *node_map;
  char map[TOTAL_HEIGHT][TOTAL_WIDTH];
} lvl;

typedef struct game_instance {
  player *p;
  unsigned char cur_level;
  lvl *levels[MAX_LEVELS];
} gi;

void print_stats(gi *game);
void move_if_free(int x, int y, player *p, char map[TOTAL_HEIGHT][TOTAL_WIDTH]);
void handle_input(char in, player *p, char map[TOTAL_HEIGHT][TOTAL_WIDTH]);
void game_over(gi *game);
gi * create_game_instance();

#endif
