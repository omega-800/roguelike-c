#ifndef MAP_H
#define MAP_H

#include <stdlib.h>
#include <ncurses.h>
#include "constants.h"

#define MIN_ROOM_WIDTH 8
#define MIN_ROOM_HEIGHT 4
#define MIN_PADDING_X 2
#define MIN_PADDING_Y 1

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

int rand_range(int min, int max);
void fill_matrix(char map[TOTAL_HEIGHT][TOTAL_WIDTH], int x, int y, int width, int height, char symbol);
snode * add_snode(snode *head, int x, int y, int w, int h, char cdir, int cpos);
void create_rooms(snode *head);
void free_map_tree(snode *head);
snode * create_map_tree();
int distance(pos *a, pos *b);
pos * find_free_tile_from(char map[TOTAL_HEIGHT][TOTAL_WIDTH], int x, int y, int radius);
pos * find_free_tile(char map[TOTAL_HEIGHT][TOTAL_WIDTH]);
void populate_matrix_rooms(char map[TOTAL_HEIGHT][TOTAL_WIDTH], snode *head);

#endif
