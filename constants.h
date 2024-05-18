#ifndef CONSTANTS_H
#define CONSTANTS_H

#define up 0
#define right 1
#define down 2
#define left 3
#define directions 4

#define MAX_LEVELS 16
#define TOTAL_WIDTH 128
#define TOTAL_HEIGHT 32
#define MIN_ROOM_WIDTH 4
#define MIN_ROOM_HEIGHT 2
#define MIN_PADDING_X 2
#define MIN_PADDING_Y 1

//#define COLOR_LIGHT_RED 8

#define WALL 1
#define EMPTY 2
#define PLAYER 3
#define HEALTH 4

typedef struct coordinates {
  int x;
  int y;
} pos;

#endif
