#include <ncurses.h>
#include <stdbool.h>
#include "../headers/cell.h"
#include "../headers/file_reader.h"
#include "../headers/area.h"

t_cell new_cell(char ch)
{
	t_cell cell;
	cell.entity = NULL;
	cell.terrain = new_terrain(ch);
	return cell;
}

bool is_blocking(t_cell *cell)
{
	char ch = cell->entity->ch;
	const char blockers[] = "#";
	return strchr(blockers, ch) != NULL;
}

t_cell *neighbor(e_direction dir, t_area *area, t_cell *cell)
{
	int index = cell - area->cells;
	switch(dir)
	{
		case UP:
			if (index - area->width < 0)
				return NULL;
			return &area->cells[index - area->width];
		case DOWN:
			if (index + area->width >= area->height * area->width)
				return NULL;
			return &area->cells[index + area->width];
		case LEFT:
			if (index % area->width == 0)
				return NULL;
			return &area->cells[index - 1];
		case RIGHT:
			if (index % area->width == area->width - 1)
				return NULL;
			return &area->cells[index + 1];
		default:
			return NULL;
	}
}
