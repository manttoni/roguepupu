#include <signal.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include "../headers/area.h"
#include "../headers/utils.h"
#include "../headers/game.h"
#include "../headers/windows.h"
#include "../headers/globals.h"

WINDOW *map_win = NULL;
WINDOW *log_win = NULL;
WINDOW *leg_win = NULL;

int main(void)
{
	fclose((fopen("logs/debug.log", "w")));
	signal(SIGSEGV, handle_segfault);
	srand(time(NULL));
	init_ncurses();
	init_windows();
	t_game *game = new_game(new_area(DUNGEON));
	start(game);
	delete_windows();
	end_ncurses(0);
}
