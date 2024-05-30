#ifndef GAME_H
#define GAME_H

#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "constants.h"
#include "character.h"
#include "map.h"
#include "print.h"
#include "log.h"

#define MAX_LEVELS 4

typedef struct level {
  char **map;
  char **discovered;
  npc **npcs;
  pos *entrance;
  pos *exit;
} lvl;

typedef struct game_instance {
  player *p;
  unsigned char cur_level;
  lvl **levels;
} gi;

void free_game(gi *game);
void print_stats(gi *game);
void move_if_free(int x, int y, gi *game);
void handle_input(char in, gi *game);
gi * create_game_instance();

#endif
