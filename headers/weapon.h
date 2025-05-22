#ifndef WEAPON_H
#define WEAPON_H

#include "item.h"
#include "creature.h"

t_item *new_weapon(char *weapon_name);
int calculate_damage(t_creature *creature, t_item *weapon);
char *get_damage_type(t_item *weapon);
int has_property(t_item *item, char *property);

#endif
