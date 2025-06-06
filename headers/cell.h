#ifndef CELL_H
#define CELL_H

#include "creature.h"
#include "terrain.h"
#include "mech.h"
#include "item.h"
#include "fungus.h"
#include <ncurses.h>
#include <stdbool.h>

typedef struct s_cell
{
	chtype		highlight; // highlight info as bits using ncurses attributes
	t_terrain	*terrain;
	t_mech		*mech;
	t_item		*item;
	t_creature	*creature;
	t_fungus	*fungus;
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
	DOWNRIGHT,
	DIRECTION_END
}	e_direction;

typedef enum
{
	ENTITY_CREATURE,
	ENTITY_ITEM,
	ENTITY_TERRAIN,
	ENTITY_MECH,
	ENTITY_FUNGUS,
	ENTITY_NONE
}	e_entity;

int get_illumination(t_cell *cell);
t_cell *get_cell(size_t i);
e_entity top_entity(t_cell *cell);
char cell_char(t_cell *cell);
short cell_fg(t_cell *cell);
short cell_bg(t_cell *cell);

double distance(t_cell *a, t_cell *b);

t_cell new_cell(char terrain, char mech, char item, char creature, t_area *area);
t_cell *random_neighbor(t_cell *cell);
t_cell *neighbor(e_direction dir, t_cell *cell);

int get_cell_flags(t_cell *cell);
int has_blocking_terrain(t_cell *cell);
int is_blocked(t_cell *cell);
int is_interactable(t_cell *cell);
int is_locked(t_cell *cell);
int is_trapped(t_cell *cell);
int is_closed(t_cell *cell);
int has_item(t_cell *cell);
int has_npc(t_cell *cell);
int has_enemy(t_cell *cell, t_creature *of);

int was_seen(t_cell *cell);
int visibility(t_cell *eye, t_cell *target);
int is_neighbor(t_cell *cell, t_cell *other);

#endif
