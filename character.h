#ifndef CHARACTER_H
#define CHARACTER_H

#include <stdlib.h>
#include <ncurses.h>
#include "constants.h"

#define PLAYER_MAX_HEALTH 32
#define PLAYER_INIT_ATTACK 2
#define PLAYER_IDLE_HEAL 1
#define PLAYER_IDLE_TIME 16

extern const char enemies[4][3];

typedef struct character {
  pos *position;
  char friendly;
  char level;
  char health;
  //void (*move_p)(struct character *);
} npc;

typedef struct player {
  pos *position;
  char health;
  char attack;
  char idle_streak;
  //void (*move_p)(struct player *);
} player;

player * create_player(pos *p_position);
npc * create_npc(pos *n_position, char level, char friendly);
void free_player(player *p);
void free_npc(npc *p);
//void move_player(player *p);
//void move_npc(npc *c);

#endif
