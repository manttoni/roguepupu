#include "../headers/area.h"
#include "../headers/cell.h"
#include "../headers/utils.h"
#include "../headers/globals.h"
#include "../headers/windows.h"
#include "../headers/mech.h"
#include <string.h>

int get_player_index(t_area *area)
{
	for (int i = 0; i < area->width * area->height; ++i)
	{
		t_cell *c = &area->cells[i];
		if (c->creature == NULL)
			continue;
		if (c->creature->ch == '@')
			return i;
	}
	logger("player char index not found");
	end_ncurses(1);
	return -1;
}

t_cell *get_player_cell(t_area *area)
{
	int i = get_player_index(area);
	return &area->cells[i];
}

t_creature *get_player(t_area *area)
{
	int i = get_player_index(area);
	t_cell *c = &area->cells[i];
	return c->creature;
}

t_node *get_interactables(t_area *area, int flags)
{
	logger("get_interactables(%s, %d)", area->name, flags);
	int pi = get_player_index(area);
	t_cell *pc = &area->cells[pi];

	t_node *list = NULL;
	for (int i = 0; i < AREA(area); ++i)
	{
		t_cell *cell = &area->cells[i];

		if (flags & CLOSED && !is_closed(cell->terrain))
			continue;

		if (flags & LOCKED && !is_locked(cell->mech))
			continue;
		if (flags & TRAPPED && !is_trapped(cell->mech))
			continue;

		if (flags & ENEMY && !is_enemy(cell->creature))
			continue;

		if (flags & VISIBLE && !is_visible(area, pc, cell))
			continue;
		if (flags & NEIGHBOR && !is_neighbor(area, pc, cell))
			continue;
		if (!is_interactable(cell))
			continue;

		add_node_last(&list, new_node(cell));
	}
	return list;
}
