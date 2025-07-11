#ifndef WEAPON_H
#define WEAPON_H

#include "item.h"
#include "creature.h"
#include "dice.h"

t_item *new_weapon(char *weapon_name);
t_item *new_random_weapon(void);
t_roll damage_roll(t_creature *creature, t_item *weapon);
t_roll attack_roll(t_creature *creature, t_item *weapon);
char *get_damage_type(t_item *weapon);
int has_property(t_item *item, char *property);
int get_AB(t_creature *creature, t_item *weapon);

#endif
