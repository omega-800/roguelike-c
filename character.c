#include "character.h"

const char enemies[4][2] = {
  { 4, 1 },
  { 6, 2 },
  { 16, 1 },
  { 2, 8 },
};

player * create_player(pos *p_position) {
  player *tmp;
  if ((tmp = malloc(sizeof *tmp)) == NULL) {
    printw("malloc error occurred");
    (void)exit(EXIT_FAILURE);
  }
  
  tmp->position = p_position;
  tmp->health = PLAYER_MAX_HEALTH;
  tmp->attack = PLAYER_INIT_ATTACK;
  return tmp;
}

npc * create_npc(pos *n_position, char level, char friendly) {
  npc *tmp;
  if ((tmp = malloc(sizeof *tmp)) == NULL) {
    printw("malloc error occurred");
    (void)exit(EXIT_FAILURE);
  }
  
  tmp->position = n_position;
  tmp->health = enemies[level][0];
  tmp->attack = enemies[level][1];
  tmp->friendly = friendly;
  return tmp;
}

void free_player(player *p) {
  free(p->position);
  free(p);
}

void free_npc(npc *p) {
  free(p->position);
  free(p);
}
