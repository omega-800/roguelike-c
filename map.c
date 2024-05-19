#include "map.h"

int distance(pos *a, pos *b) {
  return abs(a->x - b->x) + abs(a->y - b->y);
}

int rand_range(int min, int max) {
  return (random() % (max - min + 1)) + min - 1;
}

void fill_matrix(char map[TOTAL_HEIGHT][TOTAL_WIDTH], int x, int y, int width, int height, char symbol) {
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      map[i + y][j + x] = symbol;
    }
  }
}

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

    room_size->x = rand_range(MIN_ROOM_WIDTH, head->size->x - (MIN_PADDING_X * 2));
    room_size->y = rand_range(MIN_ROOM_HEIGHT, head->size->y - (MIN_PADDING_Y * 2));
    room->size = room_size;
    room_pos->x = rand_range(head->position->x + MIN_PADDING_X, head->position->x + head->size->x - MIN_PADDING_X - room_size->x);
    room_pos->y = rand_range(head->position->y + MIN_PADDING_Y, head->position->y + head->size->y - MIN_PADDING_Y - room_size->y);
    room->position = room_pos;
    head->room = room;
  }
}

void free_map_tree(snode *head) {
  if (head->child1) free_map_tree(head->child1);
  if (head->child2) free_map_tree(head->child2);
  if (head->room) {
    free(head->room->position);
    free(head->room->size);
    free(head->room);
  }
  free(head->position);
  free(head->size);
  free(head);
}

snode * create_map_tree() {
  snode *root = add_snode(NULL, 0, 0, TOTAL_WIDTH, TOTAL_HEIGHT, 0, 0);
  create_rooms(root);
  return root;
}

pos * find_free_tile_from(char map[TOTAL_HEIGHT][TOTAL_WIDTH], int startx, int starty, int radius) {
  int x = startx;
  int y = starty;
  char direction = rand() % directions;
  int steps = 1;
  int steps_done = 0;
  pos *tmp;

  if ((tmp = malloc(sizeof *tmp)) == NULL) {
    printw("malloc error occurred");
    (void)exit(EXIT_FAILURE);
  } 

  tmp->y = starty; 
  tmp->x = startx; 

  while (true) {
    if (steps > radius * 3) {
      return tmp;
    } else if (x >= TOTAL_WIDTH || x < 0 || y >= TOTAL_HEIGHT || y < 0) {
      if ((x >= 0 ? x - TOTAL_WIDTH > startx : x * -1 > TOTAL_WIDTH - startx) && (y >= 0 ? y - TOTAL_HEIGHT > starty : y * -1 > TOTAL_HEIGHT - starty)) {
        return tmp;
      }
    } else if (map[y][x] == EMPTY && abs(x - startx) + abs(y - starty) <= radius) {
      break;
    }
    switch (direction) {
      case up: y--;
        break;
      case right: x++;
        break;
      case down: y++;
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

  tmp->x = x;
  tmp->y = y;
  return tmp;
}

// spiral from random initial pos
pos * find_free_tile(char map[TOTAL_HEIGHT][TOTAL_WIDTH]) {
  int x = random() % TOTAL_WIDTH;
  int y = random() % TOTAL_HEIGHT;
  return find_free_tile_from(map, x, y, TOTAL_WIDTH + TOTAL_HEIGHT);
}

void populate_matrix_rooms(char map[TOTAL_HEIGHT][TOTAL_WIDTH], snode *head) {
  if (head->child1) {
    populate_matrix_rooms(map, head->child1);
  }
  if (head->child2) {
    populate_matrix_rooms(map, head->child2);
  }
  //???
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
}
