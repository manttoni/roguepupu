#include "area.h"
#include "memory.h"
#include "utils.h"
#include "game.h"
#include "windows.h"
#include "globals.h"
#include "parser.h"
#include <signal.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

WINDOW *stat_win = NULL;
WINDOW *map_win = NULL;
WINDOW *log_win = NULL;
WINDOW *leg_win = NULL;
WINDOW *examine_win = NULL;

t_item *g_weapons = NULL;
int	g_weapon_count = 0;

int main(void)
{
	g_weapons = parse_weapons(read_file(WEAPON_FILE));
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	logger("\n%d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	signal(SIGSEGV, handle_segfault);
	srand(time(NULL));
	init_ncurses();
	init_windows();
	t_game *game = new_game(parse_area(read_file(MAP_DUNGEON)));
	start(game);
	free_game(game);
	free_globals();
	delete_windows();
	end_ncurses(0);
}
