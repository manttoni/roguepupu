#ifndef ITEM_H
#define ITEM_H

#include "item_data.h"
#include "fungus.h"

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
	t_fungus_data	fungus_data; // probably will rename
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

t_item *new_harvest(t_fungus *fungus);
int is_equipment(t_item *item);
int is_weapon(t_item *item);
int is_potion(t_item *item);

#endif
