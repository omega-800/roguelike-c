#include "character.h"

// i=lvl, health, atk, sight
const char enemies[4][3] = {
  { 4, 1, 4 },
  { 6, 2, 8 },
  { 16, 1, 16 },
  { 4, 8, 8 },
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
  tmp->idle_streak = 0;
  //tmp->move_p = move_player;
  return tmp;
}

npc * create_npc(pos *n_position, char level, char friendly) {
  npc *tmp;
  if ((tmp = malloc(sizeof *tmp)) == NULL) {
    printw("malloc error occurred");
    (void)exit(EXIT_FAILURE);
  }
  
  tmp->position = n_position;
  tmp->level = level;
  tmp->friendly = friendly;
  tmp->health = enemies[level][0];
  //tmp->health = enemies[level - '0'][0];
  //tmp->move_p = move_npc;
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
/*
void move_player(player *p){
  p->position->x = 0;
}

void move_npc(npc *p){

}
*/
