#ifndef PRINT_H
#define PRINT_H

#include <ncurses.h>
#include "constants.h"

char prompt(char * text);
void print_char(int x, int y, char type);
void print_matrix(char **map, char **known);
void create_colors();

#endif
