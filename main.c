#include <ncurses.h>
#include <time.h>
#include "game.h"
#include "print.h"

int main() {
  srand(time(NULL));
  gi *game = create_game_instance();
  char in = '0';
  initscr();
  create_colors();
  print_matrix(game->levels[game->cur_level]->map); 
  //move(p->position->y, p->position->x);
  while (in != 'x' && in != 'q') {
    if (game->p->health < 1) {
      game_over(game); 
      game = create_game_instance();
      print_matrix(game->levels[game->cur_level]->map); 
    }
    print_stats(game);
    refresh();
    in = getch();
    handle_input(in, game);
  }
  endwin();
  return 0;
}

