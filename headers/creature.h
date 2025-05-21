#ifndef CREATURE_H
#define CREATURE_H

#include "item.h"
#include "color.h"
#include "utils.h"

typedef struct s_area t_area;
typedef struct s_cell t_cell;

typedef struct s_creature
{
	char		ch; // what does it look like?
	short		color;
	char		*name;
	t_item		*weapon;
	t_node		*inventory;
	int			ai;
	int			health;
	int			max_health;
	int			bleeding; // a cosmetic feature
	int			stunned;
	int			faction;
	int			behavior;
}	t_creature;


int has_ranged_weapon(t_creature *creature);
void add_item(t_creature *creature, t_item *item);
int take_damage(t_cell *creature_cell, int damage, char *damage_type);
void perish(t_creature *creature, char *damage_type);
t_creature *new_creature(char ch, int area_level);
int move_creature(t_cell *dest, t_cell *src);
int is_enemy(t_creature *creature);

#endif
