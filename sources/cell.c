#include "cell.h"
#include "file_reader.h"
#include "area.h"
#include "globals.h"
#include "item.h"
#include "mech.h"
#include <ncurses.h>
#include <stdbool.h>
#include <math.h>

const int dirs[8] = {UPLEFT, UP, UPRIGHT, LEFT, RIGHT, DOWNLEFT, DOWN, DOWNRIGHT};

int has_blocking_terrain(t_cell *cell)
{
	return strchr(TERRAIN_BLOCKING, cell->terrain->ch) != NULL;
}

t_cell *get_cell(size_t i)
{
	if (i >= AREA(g_area))
		return NULL;
	return &g_area->cells[i];
}

short cell_fg(t_cell *cell)
{
	switch (top_entity(cell))
	{
		case ENTITY_CREATURE:
			return cell->creature->color;
		case ENTITY_FUNGUS:
			return cell->fungus->color;
		case ENTITY_ITEM:
			return cell->item->color;
		case ENTITY_TERRAIN:
			return cell->terrain->color;
		case ENTITY_MECH:
			return cell->mech->color;
		default:
			return COLOR_WHITE;
	}
}

short cell_bg(t_cell *cell)
{
	t_node *lights = get_entities(NULL, TARGET_LIGHT_SOURCE);
	t_node *ptr = lights;
	short bg = cell->color;

	while (ptr != NULL)
	{
		t_cell *light = (t_cell *) ptr->data;
		if (distance(light, cell) > get_illumination(light) || visibility(light, cell) != VISION_FULL)
		{
			ptr = ptr->next;
			continue;
		}
		short strongest_color = color_max(cell_fg(light)); // for glowing nightlight fungus this will return 0,0,1
		bg = color_sum(bg, strongest_color);
		ptr = ptr->next;
	}
	list_clear(&lights);
	return bg;
}

char cell_char(t_cell *cell)
{
	switch (top_entity(cell))
	{
		case ENTITY_CREATURE:
			return cell->creature->ch;
		case ENTITY_FUNGUS:
			return cell->fungus->ch;
		case ENTITY_ITEM:
			return cell->item->ch;
		case ENTITY_TERRAIN:
			return cell->terrain->ch;
		case ENTITY_MECH:
			return cell->mech->ch;
		default:
			return CHAR_UNKNOWN;
	}
}

/* Real distance between cells */
double distance(t_cell *a, t_cell *b)
{
	int ida = a - g_area->cells;
	int idb = b - g_area->cells;
	int x_a = ida % g_area->width;
	int y_a = ida / g_area->width;
	int x_b = idb % g_area->width;
	int y_b = idb / g_area->width;
	return hypot(x_a - x_b, y_a - y_b);
}

e_entity top_entity(t_cell *cell)
{

	if (cell->terrain != NULL &&
		(strchr(TERRAIN_BLOCKING, cell->terrain->ch) != NULL
		 || strchr(TERRAIN_CONTAINER, cell->terrain->ch)))
		return ENTITY_TERRAIN;

	if (cell->creature != NULL)
		return ENTITY_CREATURE;
	if (cell->fungus != NULL)
		return ENTITY_FUNGUS;
	if (cell->item != NULL)
		return ENTITY_ITEM;
	if (cell->terrain != NULL)
		return ENTITY_TERRAIN;
	if (cell->mech != NULL)
		return ENTITY_MECH;
	return ENTITY_NONE;
}

int is_trapped(t_cell *cell)
{
	if (cell->mech != NULL && cell->mech->trap > 0)
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
	if (strchr(TERRAIN_BLOCKING, cell->terrain->ch) != NULL)
		return 1;
	if (cell->fungus != NULL && strchr(FUNGUS_BLOCKING, cell->fungus->ch) != NULL)
		return 1;
	if (cell->creature != NULL)
		return 1;
	return 0;
}

int is_interactable(t_cell *cell)
{
	if (cell->creature != NULL)
		return 1;
	if (cell->fungus != NULL)
		return 1;
	if (cell->item != NULL)
		return 1;
	if (cell->terrain != NULL && strchr(TERRAIN_INTERACTABLE, cell->terrain->ch) != NULL)
		return 1;
	//if (cell->mech != NULL && strchr(MECH_INTERACTABLE, cell->mech->ch) != NULL)
	//	return 1;
	return 0;
}

int is_neighbor(t_cell *cell, t_cell *other)
{
	const int dirs[8] = {UPLEFT, UP, UPRIGHT, LEFT, RIGHT, DOWNLEFT, DOWN, DOWNRIGHT};
	for (int i = 0; i < 8; ++i)
	{
		t_cell *n = neighbor(dirs[i], cell);
		if (n == other)
			return 1;
	}
	return 0;
}

/* Gets the illumination coming from a cell */
int get_illumination(t_cell *cell)
{
	switch (top_entity(cell))
	{
		case ENTITY_FUNGUS:
			return cell->fungus->glow;
		default:
			return 0;
	}
}

int get_cell_flags(t_cell *cell)
{
	int flags = 0;
	if (get_illumination(cell) > 0)
		flags |= TARGET_LIGHT_SOURCE;
	if (cell->creature != NULL)
		flags |= cell->creature->faction;
	if (is_interactable(cell))
		flags |= TARGET_INTERACTABLE;
	return flags;
}

int is_illuminated(t_cell *cell)
{
	// get all light sources in area
	t_node *lights = get_entities(NULL, TARGET_LIGHT_SOURCE);
	t_node *ptr = lights;
	while (ptr != NULL)
	{
		t_cell *light = (t_cell *) ptr->data;
		if (distance(light, cell) <= get_illumination(light) && visibility(light, cell) == VISION_FULL)
		{
			list_clear(&lights);
			return 1;
		}
		ptr = ptr->next;
	}
	list_clear(&lights);
	return 0;
}

int blocks_vision(t_cell *cell)
{
	return strchr(CHAR_BLOCK_VISION, cell_char(cell)) != NULL;
}

int visibility(t_cell *eye, t_cell *view_cell)
{
	int darkvision = get_darkvision(eye->creature);
	if (eye == view_cell)
	{
		if (darkvision > 0 || is_illuminated(view_cell))
			return VISION_FULL;
		return VISION_NONE;
	}

	int w = g_area->width, h = g_area->height;

	int ei = eye - g_area->cells;
	int vi = view_cell - g_area->cells;

	int ex = ei % w, ey = ei / w;
	int vx = vi % w, vy = vi / w;

	int dx = abs(vx - ex), sx = ex < vx ? 1 : -1;
	int dy = -abs(vy - ey), sy = ey < vy ? 1 : -1;
	int err = dx + dy;

	int x = ex, y = ey;

	while (1)
	{
		if (!(x == ex && y == ey))
		{
			if (x < 0 || y < 0 || x >= w || y >= h)
			{
				if (was_seen(view_cell))
					return VISION_GHOST;
				return VISION_NONE;
			}

			t_cell *cell = &g_area->cells[y * w + x];

			if (x == vx && y == vy)
			{
				if (darkvision >= distance(eye, view_cell) || is_illuminated(view_cell))
					return VISION_FULL;
				if (was_seen(view_cell))
					return VISION_GHOST;
				return VISION_NONE;
			}
			if (blocks_vision(cell))
			{
				if (was_seen(view_cell))
					return VISION_GHOST;
				return VISION_NONE;
			}
		}

		int e2 = 2 * err;
		if (e2 >= dy) { err += dy; x += sx; }
		if (e2 <= dx) { err += dx; y += sy; }
	}

	return VISION_GHOST; // Fallback, though unreachable
}

int was_seen(t_cell *cell)
{
	return cell->last_draw.y > -1;
}

t_cell new_cell(char terrain, char mech, char item, char creature, t_area *area)
{
	(void)item;
	(void)mech;
	t_cell cell;
	memset(&cell, 0, sizeof(cell));
	cell.color = MY_COLOR_BLACK;
	cell.last_draw = (t_coord){-1,-1};

	cell.terrain = spawn_terrain(terrain, area);
	cell.creature = spawn_creature(creature, area);

	return cell;
}

t_cell *neighbor(e_direction dir, t_cell *cell)
{
	t_area *area = g_area;
	size_t index = cell - g_area->cells;
	switch(dir)
	{
		case UP:
			if (index < area->width)
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
			if (index < area->width + 1 || index % area->width == 0)
				return NULL;
			return &area->cells[index - area->width - 1];
		case UPRIGHT:
			if (index < area->width - 1 || (index + 1) % area->width == 0)
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
