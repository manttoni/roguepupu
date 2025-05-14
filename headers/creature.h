#ifndef CREATURE_H
#define CREATURE_H

#include "../headers/action.h"
#include "../headers/weapon.h"

typedef struct s_area t_area;
typedef struct s_cell t_cell;

typedef struct s_creature
{
	char		ch; // what does it look like?
	short		color;
	char		*name;
	t_weapon	weapon;
	int			ai;
	int			health;
	int			max_health;
}	t_creature;

t_creature *new_creature(char ch);
int move_creature(t_cell *dest, t_cell *src);

#endif
