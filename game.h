#ifndef GAME_H
#define GAME_H

#include <string.h>
#include <stdlib.h>
#include "constants.h"
#include "character.h"
#include "map.h"
#include "print.h"
#include "log.h"

#define MAX_LEVELS 4

typedef struct level {
  char map[TOTAL_HEIGHT][TOTAL_WIDTH];
  npc *npcs[];
} lvl;

typedef struct game_instance {
  player *p;
  unsigned char cur_level;
  lvl *levels[MAX_LEVELS];
} gi;

void print_stats(gi *game);
void move_if_free(int x, int y, gi *game);
void handle_input(char in, gi *game);
void game_over(gi *game);
gi * create_game_instance();

#endif
