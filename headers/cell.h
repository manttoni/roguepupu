#ifndef CELL_H
#define CELL_H

#include <ncurses.h>
#include <stdbool.h>
#include "creature.h"
#include "terrain.h"

typedef struct s_cell
{
	t_terrain	*terrain; // ground, walls, obstacles...
	t_creature	*creature; // player, goblin, spider...
	// can have traps, doors, chests, light sources...
}	t_cell;

typedef enum
{
	UPLEFT,		UP,		UPRIGHT,
	LEFT,       		RIGHT,
	DOWNLEFT, 	DOWN, 	DOWNRIGHT
}	e_direction;


void print_cell(t_cell cell);
t_cell new_cell(char ch);
t_cell *neighbor(e_direction dir, t_area *area, t_cell *cell);
int is_visible(t_area *area, t_cell *eye, t_cell *target);

#endif
