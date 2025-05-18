#include "die.h"
#include "utils.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>

t_die *new_dice(e_damage_type type, char *dice_str)
{
	int amount = atoi(dice_str);
	int sides = atoi(strchr(dice_str, 'd') + 1);
	t_die *dice = my_calloc(amount * sizeof(*dice));
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
