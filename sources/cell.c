#include <ncurses.h>
#include <stdbool.h>
#include "../headers/cell.h"
#include "../headers/file_reader.h"
#include "../headers/area.h"
#include "../headers/globals.h"

char *cell_string(t_cell *cell)
{
	if (cell->creature == NULL)
		return terrain_string(cell->terrain);
	return NULL;
}

int is_locked(t_cell *cell)
{
	t_terrain *terrain = cell->terrain;
	if (terrain->ch == '0')
		return 1;
	return 0;
}

int is_interactable(t_cell *cell)
{
	if (cell->creature != NULL && cell->creature->ch != '@')
		return 1;
	if (strchr(INTERACTABLE_TERRAIN, cell->terrain->ch) != NULL)
		return 1;
	return 0;
}

int is_neighbor(t_area *area, t_cell *cell, t_cell *other)
{
	int diff = cell - other;
	diff = abs(diff);
	if (diff == area->width)
		return 1;
	if (diff == 1 && (cell - area->cells) / area->width == (other - area->cells) / area->width)
		return 1;
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
			if (cell->terrain && strchr("#O0", cell->terrain->ch)) {
                return 0; // Blocked
            }
        }

        int e2 = 2 * err;
        if (e2 >= dy) { err += dy; x += sx; }
        if (e2 <= dx) { err += dx; y += sy; }
    }

    return 1; // Fallback, though unreachable
}

t_cell new_cell(char ch)
{
	t_cell cell;
	cell.highlight = 0;
	cell.creature = NULL;
	cell.terrain = new_terrain(ch);
	return cell;
}

t_cell **get_neighbors(t_area *area, t_cell *cell)
{
	t_cell **neighbors = my_calloc(4 * sizeof(*neighbors));
	neighbors[0] = neighbor(UP, area, cell);
	neighbors[1] = neighbor(RIGHT, area, cell);
	neighbors[2] = neighbor(DOWN, area, cell);
	neighbors[3] = neighbor(LEFT, area, cell);
	return neighbors;
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
		default:
			return NULL;
	}
}
