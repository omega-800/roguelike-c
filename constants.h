#ifndef CONSTANTS_H
#define CONSTANTS_H

#define up 0
#define right 1
#define down 2
#define left 3
#define directions 4

#define TOTAL_WIDTH 128
#define TOTAL_HEIGHT 32

//#define COLOR_LIGHT_RED 8

#define ENEMY_1 1
#define ENEMY_2 2
#define ENEMY_3 3
#define ENEMY_4 4
#define WALL 5
#define EMPTY 6
#define PLAYER 7
#define DEAD 8
#define PREV 9
#define NEXT 10

#define HEALTH 127

typedef struct coordinates {
  int x;
  int y;
} pos;

#endif
