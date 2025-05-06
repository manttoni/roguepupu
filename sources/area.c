#include "../headers/area.h"
#include "../headers/utils.h"
#include <string.h>

void allocate(t_area *area)
{
	size_t size = area->height * area->width + 1;
	area->cells = my_malloc(size * sizeof(t_cell));
	logger("Area memory allocated %s", area->name);
}

void populate(t_area *area, char *raw)
{
	t_cell *cell = area->cells;
	for (int i = 0; i < (area->width + 1) * area->height; ++i)
	{
		if (raw[i] == '\n')
			continue;
		*cell = new_cell(raw[i]);
		cell++;
	}
	logger("Area populated");
}

int get_player_index(t_area *area)
{
	for (int i = 0; i < area->width * area->height; ++i)
	{
		t_cell *c = &area->cells[i];
		if (c->entity == NULL)
			continue;
		if (c->entity->ch == '@')
			return i;
	}
	logger("player char index not found");
	end_ncurses(1);
	return -1;
}

t_entity *get_player(t_area *area)
{
	for (int i = 0; i < area->width * area->height; ++i)
	{
		t_cell *c = &area->cells[i];
		if (c->entity == NULL)
			continue;
		if (c->entity->ch == '@')
			return c->entity;
	}
	logger("player char not found");
	end_ncurses(1);
	return NULL;
}

t_area *new_area(char *file)
{
	t_area *area = my_malloc(sizeof(t_area));
	char *raw = read_file(file);
	area->height = count_char(raw, '\n');
	area->width = strchr(raw, '\n') - raw;
	area->name = file;
	logger("Area created: %s, height: %d, width: %d", area->name, area->height, area->width);
	allocate(area);
	populate(area, raw);
	return area;
}
