#ifndef ITEM_H
#define ITEM_H

#include "die.h"

#define WEAPON_TYPES (CATEGORY_WEAPON_END)
#define SIMPLE_WEAPON_TYPES (SIMPLE_END - SIMPLE - 1)

#define POTION_TYPES (POTION_END - POTION - 1)

typedef enum
{
	CATEGORY_WEAPON_SIMPLE,
	CATEGORY_WEAPON_END
}	e_weapon_category;

typedef enum
{
	WEAPON,
		SIMPLE,
			CLUB, DAGGER, GREATCLUB, HANDAXE, JAVELIN, LIGHT_HAMMER, MACE, QUARTERSTAFF, SICKLE, SPEAR,
		SIMPLE_END,
	WEAPON_END,
	POTION,
		POTION_HEALING,
	POTION_END
}	e_item_type;

typedef enum
{
	COMMON,
	UNCOMMON,
	RARE,
	VERY_RARE,
	LEGENDARY
}	e_item_rarity;

typedef struct s_item
{
	char			*name;
	char			ch;
	short			color;
	e_item_type		type;
	e_item_rarity	rarity;
	// t_enchant		enchantment; // magical properties
	int				properties; // f.e. FINESSE | THROWN | MELEE or TWO_HANDED | RANGED or STACKABLE or READABLE etc
	t_die			*dice; // these have how much and what type of dmg they do. or how much healing
}	t_item;

int add_dice(t_item *item);
int is_weapon(t_item *item);
int is_potion(t_item *item);
t_item *new_random_item(char ch, int area_level);

#endif
