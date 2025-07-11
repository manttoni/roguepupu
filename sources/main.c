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
WINDOW *exa_win = NULL;
WINDOW *inv_win = NULL;
WINDOW *cha_win = NULL;
WINDOW *deb_win = NULL;

size_t g_allocations_made = 0;
size_t g_allocations_size = 0;
size_t g_nodes_allocated = 0;
t_item_group *g_item_groups = NULL;
size_t g_item_group_count = 0;

t_creature_group *g_creature_groups = NULL;
size_t g_creature_group_count = 0;

t_fungus *g_fungi = NULL;
size_t g_fungus_count = 0;

t_area *g_area = NULL;
e_direction g_dirs[] = {UPLEFT, UP, UPRIGHT, LEFT, RIGHT, DOWNLEFT, DOWN, DOWNRIGHT};

t_node *g_allocations = NULL;

void init_items(void)
{
	char *groups[] = {"weapon", "armor", "potion", "reagent"};
	char *files[] = {WEAPON_FILE, ARMOR_FILE, POTION_FILE, REAGENT_FILE};

	g_item_group_count = sizeof(groups) / sizeof(char *);
	g_item_groups = my_calloc(g_item_group_count, sizeof(*g_item_groups));

	for (size_t i = 0; i < g_item_group_count; ++i)
	{
		g_item_groups[i].category = groups[i];
		g_item_groups[i].array = parse_items(read_file(files[i]), groups[i], &g_item_groups[i].count);
	}
}

void init_creatures(void)
{
	char *groups[] = {"goblin"};
	char *files[] = {GOBLIN_FILE};

	g_creature_group_count = sizeof(groups) / sizeof(groups[0]);
	g_creature_groups = my_calloc(g_creature_group_count, sizeof(*g_creature_groups));

	for (size_t i = 0; i < g_creature_group_count; ++i)
	{
		g_creature_groups[i].category = groups[i];
		g_creature_groups[i].array = parse_creatures(read_file(files[i]), groups[i], &g_creature_groups[i].count);
	}
}

void init_fungi(void)
{
	char *file = FUNGUS_FILE;
	g_fungi = parse_fungi(read_file(file), &g_fungus_count);
}

void init_globals(void)
{
	logger("Initializing items");
	init_items();
	logger("Initializing creatures");
	init_creatures();
	logger("Initializing fungi");
	init_fungi();
	logger("Globals initialized");
}

void init(void)
{
	srand(time(NULL));
	init_logger();
	init_globals();
	signal(SIGSEGV, handle_segfault);
	init_ncurses();
	init_windows();
	logger("Init ready");
}

int main(void)
{
	init();
	g_area = parse_area(read_file(MAP_CAVES));
	populate_fungi(g_area);
	logger("Game started");
	start();
	logger("Game ended");
	end_ncurses(0);
}
