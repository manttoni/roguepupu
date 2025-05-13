#ifndef CREATURE_H
#define CREATURE_H

#include "../headers/action.h"
#include "../headers/weapon.h"

typedef struct s_area t_area;
typedef struct s_cell t_cell;

typedef struct s_creature
{
	char		*name;
	t_weapon	weapon;
	char		ch; // what does it look like?
	e_action	action; // next action or some AI logic thing
	int			health;
}	t_creature;

t_creature *new_creature(char ch);
int move_creature(t_cell *dest, t_cell *src);

#endif
