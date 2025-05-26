#include "fungus.h"
#include "area.h"
#include "globals.h"
#include "cell.h"
#include "interface.h"
#include <string.h>

t_fungus *new_fungus(t_fungus *template)
{
	t_fungus *fungus = my_calloc(1, sizeof(*fungus));
	memmove(fungus, template, sizeof(*fungus));
	return fungus;
}

static int check_requirement(t_area *area, size_t cell_index, char *fungus_requirement)
{
	if (strcmp(fungus_requirement, "space") == 0)
	{
		for (size_t i = 0; i < AREA(area); ++i)
		{
			if (area->cells[i].fungus == NULL && !has_blocking_terrain(&area->cells[i]))
				continue;
			if (distance(&area->cells[i], &area->cells[cell_index]) <= FUNGUS_SPAWN_SPACE)
				return 0;
		}
		return 1;
	}
	if (strcmp(fungus_requirement, "water") == 0)
	{
		for (size_t i = 0; i < END_DIRECTION; ++i)
			if (neighbor(g_dirs[i], &area->cells[cell_index])->terrain->ch == '~')
				return 1;
		return 0;
	}
	if (strcmp(fungus_requirement, "volcanic") == 0)
		return 0;
	if (strcmp(fungus_requirement, "wood") == 0)
		return 0;
	if (strcmp(fungus_requirement, "wall") == 0)
	{
		for (size_t i = 0; i < NEIGHBOR_DIRECTIONS; ++i)
		{
			if (neighbor(g_dirs[i], &area->cells[cell_index])->terrain->ch == '#')
				return 1;
		}
		return 0;
	}
	if (strcmp(fungus_requirement, "shadow") == 0)
		return 0;
	if (strcmp(fungus_requirement, "magic") == 0)
		return 0;
	if (strcmp(fungus_requirement, "cursed") == 0)
		return 0;
	if (strcmp(fungus_requirement, "corpse") == 0)
		return area->cells[cell_index].terrain->ch == 'R';
	return 0;
}

int cell_spawns_fungus(t_area *area, size_t cell_index, t_fungus *fungus)
{
	char **f_spawn = fungus->spawn;
	while (f_spawn != NULL && *f_spawn != NULL)
	{
		if (check_requirement(area, cell_index, *f_spawn) == 0)
			return 0;
		f_spawn++;
	}
	return 1;
}

void spawn_fungus(t_area *area, size_t cell_index)
{
	if (rand() % 101 >= FUNGAL_DENSITY)
		return;

	for (int i = 0; i < g_fungus_count; ++i)
	{
		t_fungus *f = &g_fungi[i];
		if (cell_spawns_fungus(area, cell_index, f))
			area->cells[cell_index].fungus = new_fungus(&g_fungi[i]);
		else
			continue;
		return;
	}
}

void populate_fungi(t_area *area)
{
	for (size_t i = 0; i < AREA(area); ++i)
	{
		if (strchr(TERRAIN_BLOCKING, area->cells[i].terrain->ch) != NULL)
			continue;

		// check if a fungus will spawn here
		spawn_fungus(area, i);
	}
}
