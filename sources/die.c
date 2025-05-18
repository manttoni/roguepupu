#include "die.h"
#include "utils.h"
#include "interface.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
int is_physical(e_damage_type damage_type)
{
	if (damage_type == SLASHING)
		return 1;
	if (damage_type == PIERCING)
		return 1;
	if (damage_type == BLUDGEONING)
		return 1;
	return 0;
}
char *dmg_str(e_damage_type damage_type)
{
	switch (damage_type)
	{
		case SLASHING:
			return "slashing";
		case BLUDGEONING:
			return "bludgeoning";
		case PIERCING:
			return "piercing";
		default:
			return "";
	}
}
t_die *new_dice(e_damage_type type, char *dice_str)
{
	int amount = atoi(dice_str);
	int sides = atoi(strchr(dice_str, 'd') + 1);
	t_die *dice = my_calloc((amount + 1) * sizeof(*dice));
	for (int i = 0; i < amount; ++i)
	{
		dice[i].type = type;
		dice[i].sides = sides;
	}
	return dice;
}

int throw_die(t_die die)
{
	srand(time(NULL));
	return rand() % die.sides + 1;
}
