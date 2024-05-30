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

typedef struct room_list_node {
  rnode *cur;
  struct room_list_node *next;
} rlnode;

typedef struct section_node {
  pos *position;
  pos *size;
  rnode *room;
  struct section_node *child2;
  struct section_node *child1;
} snode;

int rand_range(int min, int max);
void fill_matrix(char **map, int x, int y, int width, int height, char symbol);
void fill_matrix_around(char **map, int x, int y, int width, int height, char symbol, int radius);
snode * add_snode(snode *head, int x, int y, int w, int h);
void create_rnode_list(snode *head, rlnode *rooms);
rlnode * create_corridors(snode *head);
void create_rooms(snode *head);
void free_map_tree(snode *head);
void free_rlnodes(rlnode *rooms);
snode * create_map_tree();
int distance(pos *a, pos *b);
pos * find_free_tile_from(char **map, int x, int y, int radius);
pos * find_free_tile(char **map);
void populate_matrix_rooms(char **map, snode *head);
void populate_matrix_corridors(char **map, rlnode *rooms);
void populate_matrix_exits(char **map, pos *entrance, pos *xt);
char ** init_map(snode *node_map,rlnode *corridors,pos *entrance, pos *xt, int potions);
char ** init_discovered();
pos * get_exit(rlnode *rooms);
pos * rand_pos_in_room(rnode *room);

#endif
