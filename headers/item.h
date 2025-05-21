#ifndef ITEM_H
#define ITEM_H

#include "item_data.h"

typedef enum
{
	WEAPON,
	ARMOR,
	POTION
}	e_item_type;

typedef union
{
	t_weapon_data	weapon_data;
	t_potion_data	potion_data;
}	u_item_data;

typedef struct s_item
{
	char *name;
	char *type;
	char *rarity;
	char ch;
	short color;
	u_item_data data;
}	t_item;

int is_equipment(t_item *item);
int is_weapon(t_item *item);
int is_potion(t_item *item);
int has_property(t_item *item, char *property);

#endif
