#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>

#define up 'u'
#define right 'r'
#define down 'd'
#define left 'l'

#define TOTAL_WIDTH 64
#define TOTAL_HEIGHT 32
#define MIN_ROOM_WIDTH 4
#define MIN_ROOM_HEIGHT 2
#define MIN_PADDING_X 2
#define MIN_PADDING_Y 1

typedef struct room_node {
  int x_pos;
  int y_pos;
  int height;
  int width;
} rnode;

typedef struct section_node {
  int x_pos;
  int y_pos;
  int height;
  int width;
  int corridor_pos;
  char corridor_direction;
  rnode *room;
  struct section_node *child2;
  struct section_node *child1;
} snode;

unsigned char *map[TOTAL_HEIGHT][TOTAL_WIDTH];
unsigned char *wall = "█";
unsigned char *ground = "_";
unsigned char *test = "x";

snode * add_snode(snode *head, int x, int y, int w, int h, char cdir, int cpos) {
  snode *tmp = NULL;
  if ((tmp = malloc(sizeof(snode))) == NULL) {
    printf("malloc error ocurred");
    (void)exit(EXIT_FAILURE);
  }
  tmp->x_pos = x;
  tmp->y_pos = y;
  tmp->width = w;
  tmp->height = h;
  tmp->child1 = NULL;
  tmp->child2 = NULL;
  if (head == NULL) { 
    return tmp;
  }
  tmp->corridor_pos = cpos;
  tmp->corridor_direction = cdir;
  if (head->child1 == NULL) {
    head->child1 = tmp;
  } else {
    head->child2 = tmp;
  }
  return tmp;
}

void fill_tiles(int x, int y, int width, int height, unsigned char *symbol) {
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      map[i + y][j + x] = symbol;
    }
  }
}

void init_map() {
  fill_tiles(0, 0, TOTAL_WIDTH, TOTAL_HEIGHT, wall);
}

void draw_map() {
  for (int i = 0; i < TOTAL_HEIGHT; i++) {
    for (int j = 0; j < TOTAL_WIDTH; j++) {
      printf("%s", map[i][j]);
    }
    printf("\n");
  }
}

// https://gamedev.stackexchange.com/questions/82059/algorithm-for-procedural-2d-map-with-connected-paths
void create_rooms(snode *head) {
  char direction = rand() % 2;
  int split, cpos;
  int min_x = (MIN_ROOM_WIDTH * 2) + (MIN_PADDING_X * 4);
  int min_y = (MIN_ROOM_HEIGHT * 2) + (MIN_PADDING_Y * 4);
  if (direction == 0 && head->width > min_x) {
    split = MIN_ROOM_WIDTH + (MIN_PADDING_X * 2) + (rand() % (head->width - min_x)); 
    //printf("x:%d y:%d w:%d h:%d d:%d s:%d\n", head->x_pos, head->y_pos, head->width, head->height, direction, split);
    cpos = (rand() % (head->height - (MIN_PADDING_Y * 2))) + head->y_pos + MIN_PADDING_Y;
    create_rooms(add_snode(head, head->x_pos, head->y_pos, split, head->height, right, cpos));
    create_rooms(add_snode(head, head->x_pos + split, head->y_pos, head->width - split, head->height, left, cpos));
  } else if (direction == 1 && head->height > min_y) {
    split = MIN_ROOM_HEIGHT + (MIN_PADDING_Y * 2) + (rand() % (head->height - min_y)); 
    //printf("x:%d y:%d w:%d h:%d d:%d s:%d\n", head->x_pos, head->y_pos, head->width, head->height, direction, split);
    cpos = (rand() % (head->width - (MIN_PADDING_X * 2))) + head->x_pos + MIN_PADDING_X;
    create_rooms(add_snode(head, head->x_pos, head->y_pos, head->width, split, down, cpos));
    create_rooms(add_snode(head, head->x_pos, head->y_pos + split, head->width, head->height - split, up, cpos));
  } else {
    rnode *tmp = NULL;
    if ((tmp = malloc(sizeof(rnode))) == NULL) {
      printf("malloc error ocurred");
      (void)exit(EXIT_FAILURE);
    }
    tmp->x_pos = head->x_pos + MIN_PADDING_X;
    tmp->y_pos = head->y_pos + MIN_PADDING_Y;
    tmp->width = head->width - (MIN_PADDING_X * 2);
    tmp->height = head->height - (MIN_PADDING_Y * 2);
    head->room = tmp;
  }
}

void print_rooms(snode *head) {
  if (head->room != NULL) {
    //printf("x:%d y:%d w:%d h:%d d:%c p:%d\n", head->room->x_pos, head->room->y_pos, head->room->width, head->room->height, head->corridor_direction, head->corridor_pos);
    fill_tiles(head->room->x_pos, head->room->y_pos, head->room->width, head->room->height, ground);
    /*switch (head->corridor_direction) {
      case up: 
        c_x = head->corridor_pos;
        c_y = head->y_pos;
        c_w = 2;
        c_h = head->room->y_pos - head->y_pos;
        break;
      case right:
        c_x = head->room->x_pos + head->room->width;
        c_y = head->corridor_pos;
        c_w = (head->x_pos + head->width) - (head->room->x_pos + head->room->width);
        c_h = 1;
        break;
      case down:
        c_x = head->corridor_pos;
        c_y = head->room->y_pos + head->room->height;
        c_w = 2;
        c_h = (head->y_pos + head->height) - (head->room->y_pos + head->room->height);
        break;
      case left:
        c_x = head->x_pos;
        c_y = head->corridor_pos;
        c_w = head->room->x_pos - head->x_pos;
        c_h = 1;
        break;
    }*/
  }
  int c_x, c_y, c_w, c_h = 0;
  switch (head->corridor_direction) {
    case up: 
      c_x = head->corridor_pos;
      c_y = head->y_pos;
      c_w = MIN_PADDING_X;
      c_h = MIN_PADDING_Y;
      break;
    case right:
      c_x = head->x_pos + head->width - MIN_PADDING_X;
      c_y = head->corridor_pos;
      c_w = MIN_PADDING_X;
      c_h = MIN_PADDING_Y;
      break;
    case down:
      c_x = head->corridor_pos;
      c_y = head->y_pos + head->height - MIN_PADDING_Y;
      c_w = MIN_PADDING_X;
      c_h = MIN_PADDING_Y;
      break;
    case left:
      c_x = head->x_pos;
      c_y = head->corridor_pos;
      c_w = MIN_PADDING_X;
      c_h = MIN_PADDING_Y;
      break;
  }

  printf("x:%d y:%d w:%d h:%d d:%c p:%d\n", head->x_pos, head->y_pos, head->width, head->height, head->corridor_direction, head->corridor_pos);
  printf("%c %d %d %d %d %s \n", head->corridor_direction, c_x, c_y, c_w, c_h, ground);
  fill_tiles(c_x, c_y, c_w, c_h, ground);

  if (head->child1 != NULL) {
    print_rooms(head->child1);
  }
  if (head->child2 != NULL) {
    print_rooms(head->child2);
  }
}

snode * create_map() {
  init_map();
  srand(time(NULL));
  snode *root = add_snode(NULL, 0, 0, TOTAL_WIDTH, TOTAL_HEIGHT, 0, 0);
  create_rooms(root);
  print_rooms(root);
  printf("\n");
  draw_map(); 
  //refresh();
  return root;
}

int main() {
  initscr();
  snode *map = create_map();
  getch();
  endwin();
  return 0;
}
