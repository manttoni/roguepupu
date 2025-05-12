#include "../headers/area.h"
#include "../headers/utils.h"
#include <string.h>

t_node *get_creatures(t_area *area)
{
	t_node *creatures = NULL;
	t_cell *cells = area->cells;
	for (int i = 0; i < AREA(area); ++i)
	{
		t_creature *creature = cells[i].creature;
		if (creature != NULL)
			add_node_last(&creatures, init_node(creature));
	}
	return creatures;
}

void add_creature(t_area *area, t_creature *creature, int index)
{
	area->cells[index].creature = creature;
}

static void populate(t_area *area, char *raw)
{
	t_cell *cell = area->cells;
	for (int i = 0; i < (area->width + 1) * area->height; ++i)
	{
		if (raw[i] == '\n')
			continue;
		*cell = new_cell(raw[i]);
		cell++;
	}
}

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

t_creature *get_player(t_area *area)
{
	int i = get_player_index(area);
	t_cell *c = &area->cells[i];
	return c->creature;
}

t_area *new_area(char *file)
{
	t_area *area = my_calloc(sizeof(t_area));
	char *raw = read_file(file);
	area->height = count_char(raw, '\n');
	area->width = strchr(raw, '\n') - raw;
	area->name = file;
	size_t size = area->height * area->width + 1;
	area->cells = my_calloc(size * sizeof(t_cell));
	populate(area, raw);
	return area;
}
