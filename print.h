#ifndef PRINT_H
#define PRINT_H

#include <ncurses.h>
#include "constants.h"

void print_char(int x, int y, char type);
void print_matrix(char map[TOTAL_HEIGHT][TOTAL_WIDTH]);
void create_colors();

#endif
