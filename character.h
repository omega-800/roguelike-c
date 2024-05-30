#ifndef CHARACTER_H
#define CHARACTER_H

#include <stdlib.h>
#include <ncurses.h>
#include "constants.h"

#define PLAYER_MAX_HEALTH 64
#define PLAYER_MAX_LEVEL 8
#define PLAYER_INIT_ATTACK 2
#define PLAYER_IDLE_HEAL 1
#define PLAYER_IDLE_TIME 16
#define PLAYER_SIGHT 8

extern const char enemies[4][3];
extern const int player_stats[PLAYER_MAX_LEVEL][4];

typedef struct character {
  pos *position;
  char friendly;
  char level;
  char health;
  //void (*move_p)(struct character *);
} npc;

typedef struct player {
  pos *position;
  int health;
  int idle_streak;
  int level;
  int kills;
  //void (*move_p)(struct player *);
} player;

player * create_player(pos *p_position);
npc * create_npc(pos *n_position, char level, char friendly);
void free_player(player *p);
void free_npc(npc *p);
//void move_player(player *p);
//void move_npc(npc *c);

#endif
