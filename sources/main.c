#include <signal.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include "../headers/area.h"
#include "../headers/utils.h"
#include "../headers/game.h"
#include "../headers/windows.h"
#include "../headers/globals.h"
#include "../headers/parser.h"

WINDOW *stat_win = NULL;
WINDOW *map_win = NULL;
WINDOW *log_win = NULL;
WINDOW *leg_win = NULL;

int main(void)
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	logger("\n%d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	signal(SIGSEGV, handle_segfault);
	srand(time(NULL));
	init_ncurses();
	init_windows();
	t_game *game = new_game(parse_area(read_file(DUNGEON)));
	start(game);
	delete_windows();
	end_ncurses(0);
}
