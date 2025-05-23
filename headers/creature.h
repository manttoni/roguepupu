#ifndef CREATURE_H
#define CREATURE_H

#include "item.h"
#include "color.h"
#include "utils.h"
#include "dice.h"

typedef struct s_area t_area;
typedef struct s_cell t_cell;

typedef struct s_equipped
{
	t_item	*weapon;
	t_item	*offhand;
}	t_equipped;

typedef struct s_abilities
{
	int strength;
	int dexterity;
	int constitution;
	int intelligence;
	int wisdom;
	int charisma;
}	t_abilities;

typedef struct s_creature
{
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
}	t_creature;

int get_darkvision(t_creature *creature);
int get_movement(t_creature *creature);
int get_AC(t_creature *creature);
int get_actions(t_creature *creature);
int get_bonus_actions(t_creature *creature);
int get_saving_throw(t_creature *creature, char *saving_throw);
void reset_actions(t_creature *creature);
int strmod(t_creature *creature);
int dexmod(t_creature *creature);
int conmod(t_creature *creature);
int intmod(t_creature *creature);
int wismod(t_creature *creature);
int chamod(t_creature *creature);
void loot_item(t_creature *looter, t_node **inventory, int i);
void use_item(t_creature *user, t_node **inventory, int i);
void equip(t_creature *creature, t_item *item);
int has_ranged_weapon(t_creature *creature);
void add_item(t_creature *creature, t_item *item);
int take_damage(t_creature *creature, t_roll damage_roll, char *damage_type);
void perish(t_creature *creature, char *damage_type);
t_creature *new_creature(char ch, int area_level);
int move_creature(t_cell *dest, t_cell *src);
int is_enemy(t_creature *creature);
t_item *get_weapon(t_creature *creature);
void set_weapon(t_creature *creature, t_item *weapon);
t_item *get_offhand(t_creature *creature);
void set_offhand(t_creature *creature, t_item *weapon);
int get_main_damage(t_creature *creature);
int is_dual_wielding(t_creature *creature);
int is_equipped(t_creature *creature, t_item *item);

#endif
