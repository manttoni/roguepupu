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
}	u_item_data;

typedef struct s_item
{
	char **properties;
	char *name;
	char *proficiency;
	char *type;
	char *rarity;
	char ch;
	int stack;
	short color;
	u_item_data data;
}	t_item;

int is_weapon(t_item *item);
int has_property(t_item *item, char *property);

#endif
