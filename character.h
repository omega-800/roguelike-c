#ifndef CHARACTER_H
#define CHARACTER_H

#include <stdlib.h>
#include <ncurses.h>
#include "constants.h"

#define PLAYER_MAX_HEALTH 32
#define PLAYER_INIT_ATTACK 1

typedef struct character {
  pos *position;
  char health;
  char attack;
  char friendly;
} npc;

typedef struct player {
  pos *position;
  char health;
  char attack;
} player;

player * create_player(pos *p_position);
npc * create_npc(pos *n_position, char level, char friendly);
void free_player(player *p);

#endif
