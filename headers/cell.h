#ifndef CELL_H
#define CELL_H

#include <ncurses.h>
#include <stdbool.h>
#include "creature.h"
#include "terrain.h"

typedef struct s_cell
{
	int			highlight; // highlight info as bits
	t_terrain	*terrain; // ground, wall, door, corpse...
	t_creature	*creature; // when dies, becomes terrain ('C')
}	t_cell;

/* directions value is also how much i would change with that movement
 * opposite directions are also each others negatives */
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

int mandis(t_area *area, t_cell *a, t_cell *b);
t_cell new_cell(char ch);
t_cell *neighbor(e_direction dir, t_area *area, t_cell *cell);
int is_locked(t_cell *cell);
int is_interactable(t_cell *cell);
int is_visible(t_area *area, t_cell *eye, t_cell *target);
int is_neighbor(t_area *area, t_cell *cell, t_cell *other);
int is_enemy(t_cell *cell);
char *cell_string(t_cell *cell);

#endif
