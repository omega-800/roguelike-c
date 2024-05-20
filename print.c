#include "print.h"

char *characters[] = { "x", "1", "2", "3", "4", "#", "_", "@", "T" };

void print_char(int x, int y, char type) {
  mvaddch(y, x, *characters[type] | A_BOLD | COLOR_PAIR(type));
  //mvaddch(y, x, *characters[type - '0'] | A_BOLD | COLOR_PAIR(type));
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
  init_pair(ENEMY_1, COLOR_RED, COLOR_MAGENTA);
  init_pair(ENEMY_2, COLOR_RED, COLOR_BLUE);
  init_pair(ENEMY_3, COLOR_BLUE, COLOR_MAGENTA);
  init_pair(ENEMY_4, COLOR_MAGENTA, COLOR_BLUE);
  init_pair(EMPTY, COLOR_YELLOW, COLOR_BLACK);
  init_pair(WALL, COLOR_BLACK, COLOR_WHITE);
  init_pair(PLAYER, COLOR_CYAN, COLOR_GREEN);
  init_pair(HEALTH, COLOR_RED, COLOR_WHITE);
  init_pair(DEAD, COLOR_YELLOW, COLOR_BLACK);
  noecho();
}
