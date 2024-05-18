#include "game.h"

void print_stats(gi *game) {
  mvprintw(TOTAL_HEIGHT, 0,"Press x or q to exit, hjkl to navigate\n"); 
  mvprintw(TOTAL_HEIGHT + 1, 0, "Health: "); 
  for (int i = 0; i < PLAYER_MAX_HEALTH / 2; i++) {
    mvaddch(TOTAL_HEIGHT + 1, 8 + i, (game->p->health - i * 2 == 1 ? '.' : game->p->health > i * 2 ? ':' : ' ') | A_BOLD | COLOR_PAIR(HEALTH));
  }
  mvprintw(TOTAL_HEIGHT + 1, 9 + (PLAYER_MAX_HEALTH / 2), "(%d / %d)", game->p->health, PLAYER_MAX_HEALTH); 
  printw("\nLevel: %d / %d\n", game->cur_level, MAX_LEVELS);
  printw("Position: x(%d) y(%d)\n", game->p->position->x, game->p->position->y);
  printw("colors: %d", COLORS);
}

void move_if_free(int x, int y, player *p, char map[TOTAL_HEIGHT][TOTAL_WIDTH]) {
  if (map[p->position->y + y][p->position->x + x] != WALL){
    map[p->position->y][p->position->x] = EMPTY;
    print_char(p->position->x, p->position->y, EMPTY);
    
    p->position->y += y;
    p->position->x += x;

    map[p->position->y][p->position->x] = PLAYER;
    print_char(p->position->x, p->position->y, PLAYER);
    //move(p->position->y, p->position->x);
  }  
}

void handle_input(char in, player *p, char map[TOTAL_HEIGHT][TOTAL_WIDTH]) {
  switch (in) {
    case 'k': move_if_free(0, -1, p, map);
      break;
    case 'l': move_if_free(1, 0, p, map);
      break;
    case 'j': move_if_free(0, 1, p, map);
      break;
    case 'h': move_if_free(-1, 0, p, map);
      break;
  }
}

void game_over(gi *game) {
  clear();
  printw("You lost!\nStart new game? (y/n)");
  refresh();
  if(getch() != 'y') exit;
  free_map_tree(game->levels[game->cur_level]->node_map);
  free_player(game->p);
  free(game);
}

gi * create_game_instance() {
  gi *tmp;
  lvl *l;
  lvl *lvls[MAX_LEVELS];

  if ((tmp = malloc(sizeof *tmp)) == NULL
      || (l = malloc(sizeof *l)) == NULL) {
    printw("malloc error occurred");
    (void)exit(EXIT_FAILURE);
  }
  for (int i = 0; i < MAX_LEVELS; i++) {
    if ((lvls[i] = malloc(sizeof *l)) == NULL) {
      printw("malloc error occurred");
      (void)exit(EXIT_FAILURE);
    }
  }
  char map[TOTAL_HEIGHT][TOTAL_WIDTH];
  snode *node_map = create_map_tree();
  fill_matrix(map, 0, 0, TOTAL_WIDTH, TOTAL_HEIGHT, WALL);
  populate_matrix_rooms(map, node_map);
  player *p = create_player(find_free_tile(map));
  fill_matrix(map, p->position->x, p->position->y, 1, 1, PLAYER);
  
  l->depth = 0;
  l->enemies = 2;
  l->enemy_lvl_min = 0;
  l->enemy_lvl_max = 1;
  l->node_map = node_map;
  memcpy(l->map, map, sizeof(l->map)); 

  lvls[0] = l;

  tmp->p = p;
  tmp->cur_level = 0;
  memcpy(tmp->levels, lvls, sizeof(tmp->levels)); 
  return tmp;
}

