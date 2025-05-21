#ifndef CELL_H
#define CELL_H

#include "creature.h"
#include "terrain.h"
#include "mech.h"
#include "item.h"
#include <ncurses.h>
#include <stdbool.h>

typedef struct s_cell
{
	chtype		highlight; // highlight info as bits using ncurses attributes
	t_terrain	*terrain;
	t_mech		*mech;
	t_item		*item;
	t_creature	*creature;
	t_coord		last_draw;
	short		color;
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

typedef enum
{
	CREATURE,
	ITEM,
	TERRAIN,
	MECH,
	NONE
}	e_cell_type;

e_cell_type top_entity(t_cell *cell);
char cell_char(t_cell *cell);
short cell_fg(t_cell *cell);
short cell_bg(t_cell *cell);

double distance(t_area *area, t_cell *a, t_cell *b);
int mandis(t_area *area, t_cell *a, t_cell *b);

t_cell new_cell(char terrain, char mech, char item, char creature, int area_level);
t_cell *random_neighbor(t_area *area, t_cell *cell);
t_cell *neighbor(e_direction dir, t_area *area, t_cell *cell);

int is_blocked(t_cell *cell);
int is_interactable(t_cell *cell);
int is_locked(t_cell *cell);
int is_trapped(t_cell *cell);
int is_closed(t_cell *cell);
int has_item(t_cell *cell);
int has_npc(t_cell *cell);
int has_enemy(t_cell *cell, t_creature *of);

int was_seen(t_cell *cell);
int is_visible(t_area *area, t_cell *eye, t_cell *target);
int is_neighbor(t_area *area, t_cell *cell, t_cell *other);

#endif
