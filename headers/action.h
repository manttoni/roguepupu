#ifndef ACTION_H
#define ACTION_H

//#include "../headers/area.h"
//#include "../headers/cell.h"
//#include "../headers/creature.h"
#include "terrain.h"

typedef struct s_area t_area;
typedef struct s_cell t_cell;
typedef struct s_creature t_creature;

typedef enum
{
	PASS,
	MOVE_UP,
	MOVE_DOWN,
	MOVE_LEFT,
	MOVE_RIGHT,
	INTERACT,
	NONE
}	e_action;

void act(t_area *area, t_cell *cell, t_creature *creature);
void open_door(t_terrain *door);
void unlock_door(t_terrain *door);

#endif
