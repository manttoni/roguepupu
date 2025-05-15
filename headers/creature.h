#ifndef CREATURE_H
#define CREATURE_H

#include "weapon.h"
#include "color.h"

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
	int			bleeding; // bleeding does damage equal to remaining turns
	int			stunned;
}	t_creature;

int take_damage(t_creature *creature, int damage, e_damage_type damage_type);
void perish(t_creature *creature, e_damage_type damage_type);
t_creature *new_creature(char ch);
int move_creature(t_cell *dest, t_cell *src);

#endif
