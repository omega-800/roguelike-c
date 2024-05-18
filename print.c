#include "print.h"

unsigned char *characters[] = { "x", "#", "_", "0" };

void print_char(int x, int y, char type) {
  mvaddch(y, x, *characters[type] | A_BOLD | COLOR_PAIR(type));
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
