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
WINDOW *inventory_win = NULL;

t_item *g_weapons = NULL;
t_area *g_area = NULL;

int	g_weapon_count = 0;

void init_globals(void)
{
	g_weapons = parse_weapons(read_file(WEAPON_FILE));
}

void init(void)
{
	init_logger();
	init_globals();
	signal(SIGSEGV, handle_segfault);
	srand(time(NULL));
	init_ncurses();
	init_windows();
}

int main(void)
{
	init();

	t_game *game = new_game(parse_area(read_file(MAP_DUNGEON)));
	start(game);

	free_game(game);
	free_globals();
	end_ncurses(0);
}
