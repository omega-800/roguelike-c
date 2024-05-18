#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>

#define up 0
#define right 1
#define down 2
#define left 3
#define directions 4

#define TOTAL_WIDTH 128
#define TOTAL_HEIGHT 32
#define MIN_ROOM_WIDTH 4
#define MIN_ROOM_HEIGHT 2
#define MIN_PADDING_X 2
#define MIN_PADDING_Y 1

#define PLAYER_MAX_HEALTH 64
//#define COLOR_LIGHT_RED 8

#define WALL 1
#define EMPTY 2
#define PLAYER 3
#define HEALTH 4

typedef struct coordinates {
  int x;
  int y;
} pos;

typedef struct player {
  pos *position;
  char health;
} player;

typedef struct room_node {
  pos *position;
  pos *size;
} rnode;

typedef struct section_node {
  pos *position;
  pos *size;
  int corridor_pos;
  char corridor_direction;
  rnode *room;
  struct section_node *child2;
  struct section_node *child1;
} snode;

typedef struct game_instance {
  player *p;
  snode *node_map;
  char map[TOTAL_HEIGHT][TOTAL_WIDTH];
} gi;

unsigned char *characters[] = { "x", "#", "_", "0" };

snode * add_snode(snode *head, int x, int y, int w, int h, char cdir, int cpos) {
  snode *section = NULL;
  pos *section_pos = NULL;
  pos *section_size = NULL;
  if ((section = malloc(sizeof *section)) == NULL 
      || (section_pos = malloc(sizeof *section_pos)) == NULL
      || (section_size = malloc(sizeof *section_size)) == NULL) {
    printw("malloc error occurred");
    (void)exit(EXIT_FAILURE);
  }
  section_pos->x = x;
  section_pos->y = y;
  section->position = section_pos;
  section_size->x = w;
  section_size->y = h;
  section->size = section_size;
  section->child1 = NULL;
  section->child2 = NULL;
  if (head == NULL) { 
    return section;
  }
  section->corridor_pos = cpos;
  section->corridor_direction = cdir;
  if (head->child1 == NULL) {
    head->child1 = section;
  } else {
    head->child2 = section;
  }
  return section;
}

void fill_matrix(char map[TOTAL_HEIGHT][TOTAL_WIDTH], int x, int y, int width, int height, char symbol) {
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      map[i + y][j + x] = symbol;
    }
  }
}

void print_char(int x, int y, char type) {
  mvaddch(y, x, *characters[type] | A_BOLD | COLOR_PAIR(type));
}

void print_stats(player *p) {
  mvprintw(TOTAL_HEIGHT + 1, 0, "Health: "); 
  for (int i = 0; i < PLAYER_MAX_HEALTH / 2; i++) {
    mvaddch(TOTAL_HEIGHT + 1, 8 + i, (p->health - i * 2 == 1 ? '.' : p->health > i * 2 ? ':' : ' ') | A_BOLD | COLOR_PAIR(HEALTH));
  }
  mvprintw(TOTAL_HEIGHT + 1, 9 + (PLAYER_MAX_HEALTH / 2), "(%d / %d)", p->health, PLAYER_MAX_HEALTH); 
  printw("\nPosition: x(%d) y(%d)\n", p->position->x, p->position->y);
  printw("colors: %d", COLORS);
}

void print_matrix(char map[TOTAL_HEIGHT][TOTAL_WIDTH]) {
  for (int i = 0; i < TOTAL_HEIGHT; i++) {
    for (int j = 0; j < TOTAL_WIDTH; j++) {
        print_char(j, i, map[i][j]);
        //attron(COLOR_PAIR(map[i][j]));
        //addstr(characters[map[i][j]]);
        //attroff(COLOR_PAIR(map[i][j]));
    }
    printw("\n");
  }
  printw("Press x or q to exit, hjkl to navigate\n");
}

// https://gamedev.stackexchange.com/questions/82059/algorithm-for-procedural-2d-map-with-connected-paths
void create_rooms(snode *head) {
  char direction = rand() % 2;
  int split, cpos;
  int min_x = (MIN_ROOM_WIDTH * 2) + (MIN_PADDING_X * 4);
  int min_y = (MIN_ROOM_HEIGHT * 2) + (MIN_PADDING_Y * 4);
  if (direction == 0 && head->size->x > min_x) {
    split = MIN_ROOM_WIDTH + (MIN_PADDING_X * 2) + (rand() % (head->size->x - min_x)); 
    //printw("x:%d y:%d w:%d h:%d d:%d s:%d\n", head->position->x, head->position->y, head->size->x, head->size->y, direction, split);
    cpos = (rand() % (head->size->y - (MIN_PADDING_Y * 2))) + head->position->y + MIN_PADDING_Y;
    create_rooms(add_snode(head, head->position->x, head->position->y, split, head->size->y, right, cpos));
    create_rooms(add_snode(head, head->position->x + split, head->position->y, head->size->x - split, head->size->y, left, cpos));
  } else if (direction == 1 && head->size->y > min_y) {
    split = MIN_ROOM_HEIGHT + (MIN_PADDING_Y * 2) + (rand() % (head->size->y - min_y)); 
    //printw("x:%d y:%d w:%d h:%d d:%d s:%d\n", head->position->x, head->position->y, head->size->x, head->size->y, direction, split);
    cpos = (rand() % (head->size->x - (MIN_PADDING_X * 2))) + head->position->x + MIN_PADDING_X;
    create_rooms(add_snode(head, head->position->x, head->position->y, head->size->x, split, down, cpos));
    create_rooms(add_snode(head, head->position->x, head->position->y + split, head->size->x, head->size->y - split, up, cpos));
  } else {
    rnode *room = NULL;
    pos *room_pos = NULL;
    pos *room_size = NULL;
    if ((room = malloc(sizeof *room)) == NULL 
        || (room_pos = malloc(sizeof *room_pos)) == NULL
        || (room_size = malloc(sizeof *room_size)) == NULL) {
      printw("malloc error occurred");
      (void)exit(EXIT_FAILURE);
    }
    room_pos->x = head->position->x + MIN_PADDING_X;
    room_pos->y= head->position->y + MIN_PADDING_Y;
    room->position = room_pos;
    clear();
    printw("%d, %d", room_pos->x, room_pos->y);
    room_size->x = head->size->x - (MIN_PADDING_X * 2);
    room_size->y = head->size->y - (MIN_PADDING_Y * 2);
    room->size = room_size;
    head->room = room;
  }
  refresh();
}

void populate_matrix_rooms(char map[TOTAL_HEIGHT][TOTAL_WIDTH], snode *head) {
  if (head->room && head->room->position && head->room->size && head->room->position->x) {
    //printw("%d %d %d %d", head->room->position->x, head->room->position->y, head->room->size->x, head->room->size->y);
    fill_matrix(map, head->room->position->x, head->room->position->y, head->room->size->x, head->room->size->y, EMPTY);
  }
  int c_x, c_y, c_w, c_h = 0;
  switch (head->corridor_direction) {
    case up: 
      c_x = head->corridor_pos;
      c_y = head->position->y;
      c_w = MIN_PADDING_X;
      c_h = MIN_PADDING_Y;
      break;
    case right:
      c_x = head->position->x + head->size->x - MIN_PADDING_X;
      c_y = head->corridor_pos;
      c_w = MIN_PADDING_X;
      c_h = MIN_PADDING_Y;
      break;
    case down:
      c_x = head->corridor_pos;
      c_y = head->position->y + head->size->y - MIN_PADDING_Y;
      c_w = MIN_PADDING_X;
      c_h = MIN_PADDING_Y;
      break;
    case left:
      c_x = head->position->x;
      c_y = head->corridor_pos;
      c_w = MIN_PADDING_X;
      c_h = MIN_PADDING_Y;
      break;
  }

  //printw("x:%d y:%d w:%d h:%d d:%c p:%d\n", head->position->x, head->position->y, head->size->x, head->size->y, head->corridor_direction, head->corridor_pos);
  //printw("%c %d %d %d %d %s \n", head->corridor_direction, c_x, c_y, c_w, c_h, EMPTY);
  fill_matrix(map, c_x, c_y, c_w, c_h, EMPTY);

  if (head->child1 != NULL) {
    populate_matrix_rooms(map, head->child1);
  }
  if (head->child2 != NULL) {
    populate_matrix_rooms(map, head->child2);
  }
}

snode * create_map_tree() {
  srand(time(NULL));
  snode *root = add_snode(NULL, 0, 0, TOTAL_WIDTH, TOTAL_HEIGHT, 0, 0);
  create_rooms(root);
  return root;
}

// spiral from random initial pos
pos * find_free_tile(char map[TOTAL_HEIGHT][TOTAL_WIDTH]) {
  int x = random() % TOTAL_WIDTH;
  int y = random() % TOTAL_HEIGHT;
  char direction = up;
  int steps = 1;
  int steps_done = 0;
  pos *tmp;

  while (true) {
    if (x < TOTAL_WIDTH && x >= 0 && y < TOTAL_HEIGHT && y >= 0 && map[y][x] == EMPTY) {
      break;
    }
    switch (direction) {
      case up: y++;
        break;
      case right: x++;
        break;
      case down: y--;
        break;
      case left: x--;
        break;
    }
    steps_done += 1;
    if (steps_done == (steps * 2)) {
      steps++;
      steps_done = 0; 
    }
    if (steps_done % steps == 0) {
      direction = (direction + 1) % directions;
    }
  }

  if ((tmp = malloc(sizeof *tmp)) == NULL) {
    printw("malloc error occurred");
    (void)exit(EXIT_FAILURE);
  } 

  tmp->x = x;
  tmp->y = y;
  return tmp;
}

player * create_player(char map[TOTAL_HEIGHT][TOTAL_WIDTH]) {
  player *tmp;
  if ((tmp = malloc(sizeof *tmp)) == NULL) {
    printw("malloc error occurred");
    (void)exit(EXIT_FAILURE);
  }
  
  tmp->position = find_free_tile(map);
  tmp->health = PLAYER_MAX_HEALTH;
  return tmp;
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
  } else {
    p->health--;
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

void free_map_tree(snode *head) {
  if (head->child1 != NULL) free_map_tree(head->child1);
  if (head->child2 != NULL) free_map_tree(head->child2);
  if (head->room != NULL) {
    free(head->room->position);
    free(head->room->size);
    free(head->room);
  }
  free(head->position);
  free(head->size);
  free(head);
}

void free_player(player *p) {
  free(p->position);
  free(p);
}

void game_over(gi *game) {
  clear();
  printw("You lost!\nStart new game? (y/n)");
  refresh();
  if(getch() != 'y') exit;
  free_map_tree(game->node_map);
  free_player(game->p);
  free(game);
}

void create_colors() {
  start_color();
  //init_color(COLOR_LIGHT_RED, 600, 300, 300);
  init_pair(EMPTY, COLOR_YELLOW, COLOR_GREEN);
  init_pair(WALL, COLOR_BLACK, COLOR_WHITE);
  init_pair(PLAYER, COLOR_RED, COLOR_MAGENTA);
  init_pair(HEALTH, COLOR_RED, COLOR_WHITE);
  noecho();
}

gi * create_game_instance() {
  gi *tmp;
  if ((tmp = malloc(sizeof *tmp)) == NULL) {
    printw("malloc error occurred");
    (void)exit(EXIT_FAILURE);
  }
  char map[TOTAL_HEIGHT][TOTAL_WIDTH];
  snode *node_map = create_map_tree();
  fill_matrix(map, 0, 0, TOTAL_WIDTH, TOTAL_HEIGHT, WALL);
  populate_matrix_rooms(map, node_map);
  player *p = create_player(map);
  fill_matrix(map, p->position->x, p->position->y, 1, 1, PLAYER);
  tmp->p = p;
  tmp->node_map = node_map;
  memcpy(tmp->map, map, sizeof(tmp->map)); 
  return tmp;
}

int main() {
  gi *game = create_game_instance();
  char in;
  initscr();
  create_colors();
  print_matrix(game->map); 
  //move(p->position->y, p->position->x);
  while (in != 'x' && in != 'q') {
    if (game->p->health < 1) {
      game_over(game); 
      game = create_game_instance();
    } else {
      print_stats(game->p);
      refresh();
      in = getch();
      handle_input(in, game->p, game->map);
    }
  }

  endwin();
  return 0;
}

