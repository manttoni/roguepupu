#include <ncurses.h>
#include <stdbool.h>
#include "../headers/cell.h"
#include "../headers/file_reader.h"
#include "../headers/area.h"
#include "../headers/globals.h"
#include <math.h>

char cell_char(t_cell *cell)
{
	// priority order is creature > item > terrain > mech
	if (cell->creature != NULL)
		return cell->creature->ch;
	if (cell->item != NULL)
		return cell->item->ch;
	if (cell->terrain != NULL)
		return cell->terrain->ch;
	if (cell->mech != NULL)
		return cell->mech->ch;
	return '?';
}

/* Real distance between cells */
double distance(t_area *area, t_cell *a, t_cell *b)
{
	int ida = a - area->cells;
	int idb = b - area->cells;
	int x_a = ida % area->width;
	int y_a = ida / area->width;
	int x_b = idb % area->width;
	int y_b = idb / area->width;
	return hypot(x_a - x_b, y_a - y_b);
}


/* manhattan distance between 2 cells */
int mandis(t_area *area, t_cell *a, t_cell *b)
{
	int ida = a - area->cells;
	int idb = b - area->cells;
	int x_a = ida % area->width;
	int y_a = ida / area->width;
	int x_b = idb % area->width;
	int y_b = idb / area->width;
	return abs(x_a - x_b) + abs(y_a - y_b);
}

char *cell_string(t_cell *cell)
{
	if (cell->creature != NULL)
		return cell->creature->name;
	if (cell->item != NULL)
		return cell->item->name;
	if (cell->terrain != NULL)
		return cell->terrain->name;
	if (cell->mech != NULL)
		return cell->mech->name;
	return NULL;
}

int is_blocked(t_cell *cell)
{
	if (strchr(BLOCKING_TERRAIN, cell->terrain->ch) != NULL)
		return 1;
	if (cell->creature != NULL)
		return 1;
	return 0;
}

int is_interactable(t_cell *cell)
{
	if (cell->creature != NULL)
		return 1;
	if (cell->item != NULL)
		return 1;
	if (cell->terrain != NULL && strchr(INTERACTABLE_TERRAIN, cell->terrain->ch) != NULL)
		return 1;
	if (cell->mech != NULL && strchr(INTERACTABLE_MECH, cell->mech->ch) != NULL)
		return 1;
	return 0;
}

int is_neighbor(t_area *area, t_cell *cell, t_cell *other)
{
	const int dirs[8] = {UPLEFT, UP, UPRIGHT, LEFT, RIGHT, DOWNLEFT, DOWN, DOWNRIGHT};
	for (int i = 0; i <= 8; ++i)
	{
		t_cell *n = neighbor(dirs[i], area, cell);
		if (n == other)
			return 1;
	}
	return 0;
}

// can eye see cell? using Bresenhams algorithm
int is_visible(t_area *area, t_cell *eye, t_cell *view_cell)
{
	if (eye == view_cell)
		return 1;
	int w = area->width, h = area->height;

	int ei = eye - area->cells;
	int vi = view_cell - area->cells;

	int ex = ei % w, ey = ei / w;
	int vx = vi % w, vy = vi / w;

	int dx = abs(vx - ex), sx = ex < vx ? 1 : -1;
	int dy = -abs(vy - ey), sy = ey < vy ? 1 : -1;
	int err = dx + dy;

	int x = ex, y = ey;

	while (1)
	{
		if (!(x == ex && y == ey)) {
			if (x < 0 || y < 0 || x >= w || y >= h)
				return 0;

			t_cell *cell = &area->cells[y * w + x];

			if (x == vx && y == vy)
				return 1; // Reached the target
			if (cell->terrain && strchr("#D", cell->terrain->ch)) {
				return 0; // Blocked
			}
		}

		int e2 = 2 * err;
		if (e2 >= dy) { err += dy; x += sx; }
		if (e2 <= dx) { err += dx; y += sy; }
	}

	return 1; // Fallback, though unreachable
}

t_cell new_cell(char terrain, char mech, char item, char creature, int area_level)
{
	t_cell cell;
	memset(&cell, 0, sizeof(t_cell));
	cell.terrain = new_terrain(terrain, area_level);
	cell.mech = new_mech(mech, area_level);
	cell.item = new_item(item, area_level);
	cell.creature = new_creature(creature, area_level);
	return cell;
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
			if (index + area->width >= AREA(area))
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
		case UPLEFT:
			if (index - area->width - 1 < 0 || index % area->width == 0)
				return NULL;
			return &area->cells[index - area->width - 1];
		case UPRIGHT:
			if (index - area->width + 1 < 0 || (index + 1) % area->width == 0)
				return NULL;
			return &area->cells[index - area->width + 1];
		case DOWNLEFT:
			if (index + area->width - 1 >= AREA(area) || index % area->width == 0)
				return NULL;
			return &area->cells[index + area->width - 1];
		case DOWNRIGHT:
			if (index + area->width + 1 >= AREA(area) || (index + 1) % area->width == 0)
				return NULL;
			return &area->cells[index + area->width + 1];
		default:
			return NULL;
	}
}
