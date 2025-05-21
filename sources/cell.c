#include "cell.h"
#include "file_reader.h"
#include "area.h"
#include "globals.h"
#include "item.h"
#include "mech.h"
#include <ncurses.h>
#include <stdbool.h>
#include <math.h>

short cell_fg(t_cell *cell)
{
	if (cell->creature != NULL)
		return cell->creature->color;
	if (cell->item != NULL)
		return cell->item->color;
	if (cell->terrain != NULL)
		return cell->terrain->color;
	return COLOR_WHITE;
}

short cell_bg(t_cell *cell)
{
	return cell->color;
}

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

e_cell_type top_entity(t_cell *cell)
{
	if (cell->creature != NULL)
		return CREATURE;
	if (cell->item != NULL)
		return ITEM;
	if (cell->terrain != NULL)
		return TERRAIN;
	if (cell->mech != NULL)
		return MECH;
	return NONE;
}

int is_trapped(t_cell *cell)
{
	if (cell->mech != NULL && cell->mech->trap > 1)
		return 1;
	return 0;
}

int is_locked(t_cell *cell)
{
	if (cell->mech != NULL && cell->mech->lock > 0)
		return 1;
	return 0;
}

int is_closed(t_cell *cell)
{
	return cell->terrain != NULL && strchr(TERRAIN_CLOSED, cell->terrain->ch) != NULL;
}

int is_blocked(t_cell *cell)
{
	if (strchr(TERRAIN_BLOCKED, cell->terrain->ch) != NULL)
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
	if (cell->terrain != NULL && strchr(TERRAIN_INTERACTABLE, cell->terrain->ch) != NULL)
		return 1;
	if (cell->mech != NULL && strchr(MECH_INTERACTABLE, cell->mech->ch) != NULL)
		return 1;
	return 0;
}

int has_item(t_cell *cell)
{
	return cell->item != NULL;
}

int has_enemy(t_cell *cell, t_creature *of_this_creature)
{
	if (cell->creature == NULL)
		return 0;
	return cell->creature->faction & ENEMY_FACTION(of_this_creature->faction);
}

int is_neighbor(t_area *area, t_cell *cell, t_cell *other)
{
	const int dirs[8] = {UPLEFT, UP, UPRIGHT, LEFT, RIGHT, DOWNLEFT, DOWN, DOWNRIGHT};
	for (int i = 0; i < 8; ++i)
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

int was_seen(t_cell *cell)
{
	return cell->last_draw.y > -1;
}

t_cell new_cell(char terrain, char mech, char item, char creature, int area_level)
{
	t_cell cell;
	memset(&cell, 0, sizeof(t_cell));
	cell.last_draw = (t_coord){-1,-1};

	cell.terrain = new_terrain(terrain, area_level);
	if (terrain == '#')
		cell.color = color_id((t_color){1,1,1});
	else
		cell.color = color_id((t_color){0,0,0});
	cell.mech = new_mech(mech, area_level);
	(void)item;
	//cell.item = new_random_item(item, area_level);
	cell.creature = new_creature(creature, area_level);

	if (strchr(TERRAIN_BLOCKED, terrain) != NULL)
	{
		if (cell.item != NULL || cell.creature != NULL)
		{
			logger("Item or creature spawned in bad place");
			end_ncurses(1);
		}
	}

	return cell;
}

t_cell *random_neighbor(t_area *area, t_cell *cell)
{
	e_direction dir = rand() % 8;
	return neighbor(dir, area, cell);
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
