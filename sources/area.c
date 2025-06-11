#include "area.h"
#include "cell.h"
#include "utils.h"
#include "globals.h"
#include "windows.h"
#include "mech.h"
#include "interface.h"
#include <string.h>

t_cell *get_creature_cell(t_creature *creature)
{
	int i = get_creature_index(creature);
	return &g_area->cells[i];
}

int get_creature_index(t_creature *creature)
{
	for (size_t i = 0; i < AREA(g_area); ++i)
	{
		t_cell *cell = &g_area->cells[i];
		if (creature == cell->creature)
			return i;
	}
	return -1;
}

int get_player_index(void)
{
	for (size_t i = 0; i < g_area->width * g_area->height; ++i)
	{
		t_cell *c = &g_area->cells[i];
		if (c->creature == NULL)
			continue;
		if (c->creature->ch == '@')
			return i;
	}
	logger("player char index not found");
	end_ncurses(1);
	return -1;
}

t_cell *get_player_cell(void)
{
	int i = get_player_index();
	return &g_area->cells[i];
}

t_creature *get_player(void)
{
	int i = get_player_index();
	t_cell *c = &g_area->cells[i];
	return c->creature;
}

/* Get creatures visible by creature, filtered by flags
 * if creature == NULL, find globally */
t_node *get_entities(t_creature *creature, int flags)
{
	static size_t entities_gotten = 0;
	t_node *list = NULL;
	for (size_t i = 0; i < AREA(g_area); ++i)
	{
		t_cell *cell = get_cell(i);
		int cell_flags = get_cell_flags(cell);

		if (cell->creature != NULL)
			cell_flags |= TARGET_CREATURE;

		if (creature != NULL && is_neighbor(cell, get_creature_cell(creature)))
			cell_flags |= RANGE_MELEE;

		if (creature != NULL && visibility(get_creature_cell(creature), cell) == VISION_FULL)
			cell_flags |= TARGET_VISIBLE;

		if ((flags & cell_flags) != flags)
			continue;

		add_node_last(&list, new_node(cell));
	}
	entities_gotten++;
	print_log("Entities got: %d", entities_gotten);
	return list;
}

