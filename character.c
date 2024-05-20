#include "character.h"

// attack, health, healtime, healcnt
const int player_stats[PLAYER_MAX_LEVEL][4] = {
  { 2, 32, 12, 1 },
  { 4, 64, 12, 2 },
  { 6, 69, 16, 2 },
  { 8, 96, 16, 2 },
  { 10, 128, 32, 4 },
  { 12, 128, 32, 4 },
  { 16, 256, 32, 4 },
  { 69, 420, 69, 8 },
};
// i=lvl, health, atk, sight
const char enemies[4][3] = {
  { 4, 2, 16 },
  { 6, 2, 8 },
  { 16, 2, 64 },
  { 12, 4, 2 },
};

player * create_player(pos *p_position) {
  player *tmp;
  if ((tmp = malloc(sizeof *tmp)) == NULL) {
    printw("malloc error occurred");
    (void)exit(EXIT_FAILURE);
  }
  
  tmp->position = p_position;
  tmp->idle_streak = 0;
  tmp->level = 0;
  tmp->kills = 0;
  tmp->health = player_stats[0][1];
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
