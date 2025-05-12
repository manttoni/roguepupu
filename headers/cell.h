#ifndef CELL_H
#define CELL_H

#include <ncurses.h>
#include <stdbool.h>
#include "creature.h"
#include "terrain.h"

#define REVERSE 1 // basic highlight
#define SELECTED 2 // selected highlighted thing

typedef struct s_cell
{
	int			highlight; // highlight info as bits
	t_terrain	*terrain; // ground, wall, door...
	t_creature	*creature; // player
}	t_cell;

typedef enum
{
	UPLEFT,		UP,		UPRIGHT,
	LEFT,       		RIGHT,
	DOWNLEFT, 	DOWN, 	DOWNRIGHT
}	e_direction;


void print_cell(t_cell cell);
t_cell new_cell(char ch);
t_cell **get_neighbors(t_area *area, t_cell *cell);
t_cell *neighbor(e_direction dir, t_area *area, t_cell *cell);
int is_visible(t_area *area, t_cell *eye, t_cell *target);

#endif
