#ifndef ITEM_H
#define ITEM_H

#include "item_data.h"
#include <stdlib.h>
typedef union
{
	t_weapon_data	weapon_data;
	t_armor_data	armor_data;
	t_potion_data	potion_data;
	t_reagent_data	reagent_data;
}	u_item_data;

typedef struct s_item
{
	size_t id;
	char *name;
	char *type;
	char *rarity;
	char ch;
	short color;
	u_item_data data;
}	t_item;

int item_equals(t_item *a, t_item *b);
t_item *new_item(char *name);
int is_equipment(t_item *item);
int is_weapon(t_item *item);
int is_potion(t_item *item);

#endif
