#include "area.h"
#include "memory.h"
#include "utils.h"
#include "game.h"
#include "windows.h"
#include "globals.h"
#include "parser.h"
#include "entities.h"
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

t_item_group *g_item_groups = NULL;
int g_item_group_count = 0;

t_creature_group *g_creature_groups = NULL;
int g_creature_group_count = 0;

t_area *g_area = NULL;


void init_items(void)
{
	char *groups[] = {"weapon", "potion"};
	char *files[] = {WEAPON_FILE, POTION_FILE};

	g_item_group_count = sizeof(groups) / sizeof(char *);
	g_item_groups = my_calloc(g_item_group_count, sizeof(t_item_group));

	for (int i = 0; i < g_item_group_count; ++i)
	{
		g_item_groups[i].category = groups[i];
		g_item_groups[i].array = parse_items(read_file(files[i]), groups[i], &g_item_groups[i].count);
		logger("%s loaded and there are %d", groups[i], g_item_groups[i].count);
	}
}

void init_creatures(void)
{
	char *groups[] = {"goblin"};
	char *files[] = {GOBLIN_FILE};

	g_creature_group_count = sizeof(groups) / sizeof(char *);
	g_creature_groups = my_calloc(g_creature_group_count, sizeof(t_creature_group));

	for (int i = 0; i < g_creature_group_count; ++i)
	{
		g_creature_groups[i].category = groups[i];
		g_creature_groups[i].array = parse_creatures(read_file(files[i]), groups[i], &g_creature_groups[i].count);
	}
}

void init_globals(void)
{
	init_items();
	init_creatures();
}

void init(void)
{
	srand(time(NULL));
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

	g_area = parse_area(read_file(MAP_CAVES));
	start();

	free_game();
	free_globals();
	end_ncurses(0);
}
