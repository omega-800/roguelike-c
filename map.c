#include "map.h"

int distance(pos *a, pos *b) {
  return abs(a->x - b->x) + abs(a->y - b->y);
}

int rand_range(int min, int max) {
  return (random() % (max - min + 1)) + min - 1;
}

void fill_matrix(char **map, int x, int y, int width, int height, char symbol) {
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      map[i + y][j + x] = symbol;
    }
  }
}

snode * add_snode(snode *head, int x, int y, int w, int h) {
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
  section->room = NULL;
  if (head == NULL) { 
    return section;
  }
  if (head->child1 == NULL) {
    head->child1 = section;
  } else {
    head->child2 = section;
  }
  return section;
}

void append_rlnode(rlnode *head, rlnode *next) {
  if (head->next) {
    append_rlnode(head->next, next);
  } else {
    head->next = next;
  }
}

rlnode * create_rlnode() {
  rlnode *tmp = NULL;
  if ((tmp = malloc(sizeof *tmp)) == NULL) {
    printw("malloc error occurred");
    (void)exit(EXIT_FAILURE);
  }
  tmp->next = NULL;
  tmp->cur = NULL;
  return tmp;
}

void create_rnode_list(snode *head, rlnode *rooms) {
  if (head->room) {
    if (rooms->cur){
      rlnode *newnode = create_rlnode();
      newnode->cur = head->room;
      append_rlnode(rooms, newnode);
    } else {
      rooms->cur = head->room;
    }
  }
  if (head->child1) create_rnode_list(head->child1, rooms);
  if (head->child2) create_rnode_list(head->child2, rooms);
}

rnode * create_room(snode *head) {
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

    return room;
}

// https://gamedev.stackexchange.com/questions/82059/algorithm-for-procedural-2d-map-with-connected-paths
void create_rooms(snode *head) {
  char direction = rand() % 2;
  int split;
  int min_x = (MIN_ROOM_WIDTH * 2) + (MIN_PADDING_X * 4);
  int min_y = (MIN_ROOM_HEIGHT * 2) + (MIN_PADDING_Y * 4);
  if (direction == 0 && head->size->x > min_x) {
    split = MIN_ROOM_WIDTH + (MIN_PADDING_X * 2) + (rand() % (head->size->x - min_x)); 
    //printw("x:%d y:%d w:%d h:%d d:%d s:%d\n", head->position->x, head->position->y, head->size->x, head->size->y, direction, split);
    create_rooms(add_snode(head, head->position->x, head->position->y, split, head->size->y));
    create_rooms(add_snode(head, head->position->x + split, head->position->y, head->size->x - split, head->size->y));
    
  } else if (direction == 1 && head->size->y > min_y) {
    split = MIN_ROOM_HEIGHT + (MIN_PADDING_Y * 2) + (rand() % (head->size->y - min_y)); 
    //printw("x:%d y:%d w:%d h:%d d:%d s:%d\n", head->position->x, head->position->y, head->size->x, head->size->y, direction, split);
    create_rooms(add_snode(head, head->position->x, head->position->y, head->size->x, split));
    create_rooms(add_snode(head, head->position->x, head->position->y + split, head->size->x, head->size->y - split));
  } else {
    head->room = create_room(head);
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

void free_rlnodes(rlnode *rooms) {
  if(rooms->next) free_rlnodes(rooms->next);
  free(rooms);
}

snode * create_map_tree() {
  snode *root = add_snode(NULL, 0, 0, TOTAL_WIDTH, TOTAL_HEIGHT);
  create_rooms(root);
  return root;
}

rlnode * create_corridors(snode *root) {
  rlnode *rooms = create_rlnode();
  create_rnode_list(root, rooms);
  return rooms;
}

pos * find_free_tile_from(char **map, int startx, int starty, int radius) {
  int x = startx;
  int y = starty;
  char direction = rand() % directions;
  int steps = 1;
  int steps_done = 0;
  pos *tmp = NULL;

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
pos * find_free_tile(char **map) {
  int x = random() % TOTAL_WIDTH;
  int y = random() % TOTAL_HEIGHT;
  return find_free_tile_from(map, x, y, TOTAL_WIDTH + TOTAL_HEIGHT);
}

void populate_matrix_corridors(char **map, rlnode *rooms) {
  if (!rooms->next) return;
  int from_x = rand_range(rooms->cur->position->x + 1, rooms->cur->size->x + rooms->cur->position->x - 1);
  int from_y = rand_range(rooms->cur->position->y + 1, rooms->cur->size->y + rooms->cur->position->y - 1);
  int to_x = rand_range(rooms->next->cur->position->x + 1, rooms->next->cur->size->x + rooms->next->cur->position->x - 1);
  int to_y = rand_range(rooms->next->cur->position->y + 1, rooms->next->cur->size->y + rooms->next->cur->position->y - 1);
  char minus_x = from_x > to_x;
  char minus_y = from_y > to_y;
  fill_matrix(map, minus_x ? to_x : from_x, minus_y ? to_y : from_y, abs(from_x - to_x), 1, EMPTY);
  fill_matrix(map, minus_x ? to_x : from_x, minus_y ? to_y : from_y, 2, abs(from_y - to_y), EMPTY);
  populate_matrix_corridors(map, rooms->next);  
}

void populate_matrix_rooms(char **map, snode *head) {
  if (head->child1) populate_matrix_rooms(map, head->child1);
  if (head->child2) populate_matrix_rooms(map, head->child2);
  //???
  if (head->room && head->room->position && head->room->position->x) fill_matrix(map, head->room->position->x, head->room->position->y, head->room->size->x, head->room->size->y, EMPTY);
}

void draw_map_and_free(char **map) {
  snode *node_map = create_map_tree();
  rlnode *corridors = create_corridors(node_map);
  fill_matrix(map, 0, 0, TOTAL_WIDTH, TOTAL_HEIGHT, WALL);
  populate_matrix_rooms(map, node_map);
  populate_matrix_corridors(map, corridors);
  free_map_tree(node_map);
  free_rlnodes(corridors);
}
