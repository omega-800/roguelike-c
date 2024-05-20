#include "game.h"

// i=lvl, enemy_count, e_min_lvl, e_max_lvl
const char level_stats[MAX_LEVELS][3] = {
  { 8, 1, 2 },
  { 12, 1, 3 },
  { 16, 2, 4 },
  { 32, 3, 4 },
};

void print_stats(gi *game) {
  mvprintw(TOTAL_HEIGHT, 0,"Press x or q to exit, hjkl to navigate\n"); 
  mvprintw(TOTAL_HEIGHT + 1, 0, "Health: "); 
  for (int i = 0; i < PLAYER_MAX_HEALTH / 2; i++) {
    mvaddch(TOTAL_HEIGHT + 1, 8 + i, (game->p->health - i * 2 == 1 ? '+' : game->p->health > i * 2 ? '#' : ' ') | A_BOLD | COLOR_PAIR(HEALTH));
  }
  mvprintw(TOTAL_HEIGHT + 1, 9 + (PLAYER_MAX_HEALTH / 2), "(%d / %d)", game->p->health, PLAYER_MAX_HEALTH); 
  printw("\nLevel: %d / %d\n", game->cur_level + 1, MAX_LEVELS);
  printw("Position: x(%d) y(%d)\n", game->p->position->x, game->p->position->y);
  printw("Dimensions: x(%d) y(%d)\n", COLS, LINES);
  printw("colors: %d\n", COLORS);
  //please help i am confusion
  //  npc *npcs[level_stats[0][0]];
  //  printw("npcs: %d\n", sizeof(npcs));
  //  printw("*npcs: %d\n", sizeof(*npcs));
  //  printw("**npcs: %d\n", sizeof(**npcs));
  //  for (int i = 0; i < level_stats[0][0]; i++) {
  //    printw("npc %d:x(%d) y(%d)\n", i, game->levels[game->cur_level]->npcs[i]->position->x, game->levels[game->cur_level]->npcs[i]->position->y);
  //  }
}

char try_move(int x, int y, char **map, pos *pos, char type){
  if (map[pos->y + y][pos->x + x] != EMPTY) return 0;
  map[pos->y][pos->x] = EMPTY;
  print_char(pos->x, pos->y, EMPTY);
  pos->y += y;
  pos->x +=x;
  map[pos->y][pos->x] = type;
  print_char(pos->x, pos->y, type);
  return 1;
}

void move_npc(npc *curr, pos *p_pos, lvl *curlvl) {
  char minus_x = curr->position->x > p_pos->x;
  char minus_y = curr->position->y > p_pos->y;
  char sees = distance(curr->position, p_pos) < enemies[curr->level-1][2];
  char closer_x = abs(curr->position->x - p_pos->x) < abs(p_pos->y - curr->position->y);
  if(sees && try_move(closer_x ? 0 : minus_x ? -1 : 1, closer_x ? minus_y ? -1 : 1 : 0, curlvl->map, curr->position, curr->level)) {
    //log_msg(LOGFILE, "moved npc: x(%d) y(%d) lvl(%d) (saw player)", curr->position->x, curr->position->y, curr->level);
  } else {
    pos* new_pos = find_free_tile_from(curlvl->map, curr->position->x, curr->position->y, 1);
    try_move(new_pos->x > curr->position->x ? 1 : new_pos->x < curr->position->x ? -1 : 0, new_pos->y > curr->position->y ? 1 : new_pos->y < curr->position->y ? -1 : 0, curlvl->map, curr->position, curr->level);
    free(new_pos);
    //log_msg(LOGFILE, "moved npc: x(%d) y(%d) lvl(%d) (randomly)", curr->position->x, curr->position->y, curr->level);
  }
}

void move_if_free(int x, int y, gi *game) {
  lvl *curlvl = game->levels[game->cur_level];
  pos *p_pos = game->p->position;

  if (curlvl->map[p_pos->y + y][p_pos->x + x] <= 4) {
    // either an enemy or something broke
    npc *enemy;
    for (int i = 0; i < level_stats[game->cur_level][0]; i++) {
      if (curlvl->npcs[i]->position->x == p_pos->x + x && curlvl->npcs[i]->position->y == p_pos->y + y) {
        enemy = curlvl->npcs[i];
        break;
      }
    }
    if (enemy) {
      if (enemy->health > 0) {
        //log_msg(LOGFILE, "preattack player h(%d) npc h(%d)", game->p->health, enemy->health);
        enemy->health -= game->p->attack;
        game->p->idle_streak = 0;
        //log_msg(LOGFILE, "ATTACK! player h(%d) npc h(%d)", game->p->health, enemy->health);
      }
      if (enemy->health <= 0) {
        print_char(enemy->position->x, enemy->position->y, DEAD);
      }
    }
  } else if (try_move(x, y, curlvl->map, p_pos, PLAYER)){
    if (game->p->idle_streak < PLAYER_IDLE_TIME) {
      game->p->idle_streak++;
    } else {
      game->p->idle_streak = 0;
      if (game->p->health + PLAYER_IDLE_HEAL <= PLAYER_MAX_HEALTH) {
        game->p->health += PLAYER_IDLE_HEAL;
      } else {
        game->p->health = PLAYER_MAX_HEALTH;
      }
    }
    //log_msg(LOGFILE, "moved player: x(%d) y(%d)", p_pos->x, p_pos->y);
  }

  for (int i = 0; i < level_stats[game->cur_level][0]; i++) {
    if (curlvl->npcs[i]->health <= 0) continue;
    if (distance(curlvl->npcs[i]->position, p_pos) == 1) {
      game->p->health -= enemies[curlvl->npcs[i]->level][1];
      //game->p->health -= enemies[curlvl->npcs[i]->level - '0'][1];
      game->p->idle_streak = 0;
    } else {
      move_npc(curlvl->npcs[i], p_pos, curlvl);
    }
  }
}

void handle_input(char in, gi *game) {
  switch (in) {
    case 'k': move_if_free(0, -1, game);
              break;
    case 'l': move_if_free(1, 0, game);
              break;
    case 'j': move_if_free(0, 1, game);
              break;
    case 'h': move_if_free(-1, 0, game);
              break;
  }
}

void free_map(char **map) {
  for (int i = 0; i < TOTAL_HEIGHT; i++) {
    free(map[i]);
  }
  free(map);
}

void free_levels(lvl **lvls) {
  for (int i = 0; i < MAX_LEVELS; i++) {
    free_map(lvls[i]->map);
    for (int j = 0; j < level_stats[i][0]; j++) {
      free_npc(lvls[i]->npcs[j]);
    }
    free(lvls[i]->npcs);
    free(lvls[i]);
  }
  free(lvls);
}

void free_game(gi *game) {
  free_player(game->p);
  free_levels(game->levels);
  free(game);
}

void game_over(gi *game) {
  clear();
  printw("You lost!\nStart new game? (y/n)");
  refresh();
  if(getch() != 'y') exit(1);
  free_game(game);
}

char ** init_map() {
  char **map = NULL;
  if ((map = calloc(TOTAL_HEIGHT, sizeof *map)) == NULL) {
    printw("malloc error occurred");
    (void)exit(EXIT_FAILURE);
  }
  for (int i = 0; i < TOTAL_HEIGHT; i++) {
    if ((map[i] = calloc(TOTAL_WIDTH, sizeof **map)) == NULL) {
      printw("malloc error occurred");
      (void)exit(EXIT_FAILURE);
    }
  }
  draw_map_and_free(map);
  return map;
}

lvl * create_lvl(int stage) {
  lvl *l = NULL;
  npc **npcs = NULL;
  if ((l = malloc(sizeof *l)) == NULL
      || (npcs = calloc(level_stats[stage][0], sizeof **npcs)) == NULL) {
    printw("malloc error occurred");
    (void)exit(EXIT_FAILURE);
  }

  l->map = init_map();

  for (int i = 0; i < level_stats[stage][0]; i++) {
    npcs[i] = create_npc(find_free_tile(l->map), rand_range(level_stats[stage][1], level_stats[stage][2]) + 1, 0);
    fill_matrix(l->map, npcs[i]->position->x, npcs[i]->position->y, 1, 1, npcs[i]->level);
    log_msg(LOGFILE, "created npc %d: x(%d) y(%d) lvl(%d)", i, npcs[i]->position->x, npcs[i]->position->y, npcs[i]->level);
  }
  l->npcs = npcs;
  return l;
}

gi * create_game_instance() {
  clear_log(LOGFILE);
  log_msg(LOGFILE, "game started");
  gi *tmp = NULL;
  lvl **lvls = NULL;

  if ((tmp = malloc(sizeof *tmp)) == NULL
    || (lvls = calloc(MAX_LEVELS, sizeof **lvls)) == NULL) {
    printw("malloc error occurred");
    (void)exit(EXIT_FAILURE);
  }
  for (int i = 0; i < MAX_LEVELS; i++) {
    lvls[i] = create_lvl(i);
  }

  player *p = create_player(find_free_tile(lvls[0]->map));
  fill_matrix(lvls[0]->map, p->position->x, p->position->y, 1, 1, PLAYER);
  log_msg(LOGFILE, "created player: x(%d) y(%d)", p->position->x, p->position->y);

  tmp->p = p;
  tmp->cur_level = 0;
  tmp->levels = lvls; 
  return tmp;
}

