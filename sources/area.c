#include "area.h"
#include "cell.h"
#include "utils.h"
#include "globals.h"
#include "windows.h"
#include "mech.h"
#include "interface.h"
#include <string.h>

int get_player_index(void)
{
	for (int i = 0; i < g_area->width * g_area->height; ++i)
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

t_node *get_interactables(int flags)
{
	int pi = get_player_index();
	t_cell *pc = &g_area->cells[pi];

	t_node *list = NULL;
	for (int i = 0; i < AREA(g_area); ++i)
	{
		t_cell *cell = &g_area->cells[i];

		// terrain checks
		if (flags & SCAN_CLOSED && !is_closed(cell))
			continue;

		// mech checks
		if (flags & SCAN_LOCKED && !is_locked(cell))
			continue;
		if (flags & SCAN_TRAPPED && !is_trapped(cell))
			continue;

		// creature checks
		if (flags & SCAN_ENEMY && !has_enemy(cell, get_player()))
			continue;

		// item checks
		if (flags & SCAN_ITEM && !has_item(cell))
			continue;

		// cell checks
		if (flags & SCAN_VISIBLE && !is_visible(pc, cell))
			continue;
		if (flags & SCAN_NEIGHBOR && !is_neighbor(pc, cell))
			continue;
		if (!is_interactable(cell))
			continue;

		add_node_last(&list, new_node(cell));
	}
	return list;
}
