#ifndef WEAPON_H
#define WEAPON_H

#include "dice.h"

typedef enum
{
	SLASHING,
	BLUNT
}	e_damage_type;

typedef struct s_weapon
{
	char *name;
	t_dice damage;
	int damage_type;
}	t_weapon;

#endif
