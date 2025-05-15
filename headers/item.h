#ifndef ITEM_H
#define ITEM_H

#include "dice.h"

#define WEAPON_TYPES (WEAPON_END - WEAPON - 1)

typedef enum
{
	SLASHING,
	BLUNT,
	PIERCING,
	BLEEDING
}	e_damage_type;

typedef enum
{
	WEAPON,
	SWORD,
	AXE,
	CLUB,
	WEAPON_END
}	e_item;

typedef struct s_item
{
	char			*name;
	char			ch;
	e_item			class;
	e_item			type;
	int				level;
	t_dice			damage;
	e_damage_type	damage_type;
}	t_item;

t_item *new_item(char ch, int area_level);
char *generate_item_name(t_item *item);

#endif
