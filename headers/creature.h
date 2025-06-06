#ifndef CREATURE_H
#define CREATURE_H

#include "item.h"
#include "color.h"
#include "utils.h"
#include "dice.h"
#include "equipment.h"
#include "abilities.h"

typedef struct s_creature
{
	size_t		id;
	char		ch; // what does it look like?
	short		color;
	char		*name;
	t_equipped	equipped;
	t_abilities	abilities;
	t_node		*inventory;
	int			ai;
	int			health;
	int			max_health;
	int			faction;
	int			behavior;
	int			level;
}	t_creature;

int creature_equals(t_creature *a, t_creature *b);
int get_attack_range(t_creature *creature);
int enemy_factions(t_creature *creature);
int is_in_combat(t_creature *creature);
int get_darkvision(t_creature *creature);
int get_movement(t_creature *creature);
int get_actions(t_creature *creature);
int get_bonus_actions(t_creature *creature);
int loot_item(t_creature *looter, t_node **inventory, int i);
int use_item(t_creature *user, t_node **inventory, int i);
void add_item(t_creature *creature, t_item *item);
int take_damage(t_creature *creature, t_roll damage_roll, char *damage_type);
void perish(t_creature *creature, char *damage_type);
t_creature *new_creature(char *name);
t_creature *spawn_creature(char ch, t_area *area);

#endif
