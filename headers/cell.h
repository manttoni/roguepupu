#ifndef CELL_H
#define CELL_H

#include <ncurses.h>
#include <stdbool.h>
#include "creature.h"
#include "terrain.h"
#include "mech.h"
#include "item.h"

typedef struct s_cell
{
	int			highlight; // highlight info as bits
	t_terrain	*terrain;
	t_mech		*mech;
	t_item		*item;
	t_creature	*creature;
}	t_cell;

typedef enum
{
	UPLEFT,
	UP,
	UPRIGHT,
	LEFT,
	RIGHT,
	DOWNLEFT,
	DOWN,
	DOWNRIGHT
}	e_direction;

char cell_char(t_cell *cell);
double distance(t_area *area, t_cell *a, t_cell *b);
int mandis(t_area *area, t_cell *a, t_cell *b);
t_cell new_cell(char terrain, char mech, char item, char creature, int area_level);
t_cell *neighbor(e_direction dir, t_area *area, t_cell *cell);
int is_blocked(t_cell *cell);
int is_interactable(t_cell *cell);
int is_visible(t_area *area, t_cell *eye, t_cell *target);
int is_neighbor(t_area *area, t_cell *cell, t_cell *other);
char *cell_string(t_cell *cell);

#endif
