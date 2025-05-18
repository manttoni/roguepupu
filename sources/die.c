#include "die.h"
#include "utils.h"
#include "interface.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>

t_die *new_dice(e_damage_type type, char *dice_str)
{
	int amount = atoi(dice_str);
	int sides = atoi(strchr(dice_str, 'd') + 1);
	t_die *dice = my_calloc((amount + 1) * sizeof(*dice));
	for (int i = 0; i < amount; ++i)
	{
		dice[i].type = type;
		dice[i].sides = sides;
		print_log("New die: %dd%d", 1, sides);
	}
	return dice;
}

int throw_die(t_die die)
{
	print_log("Throwing die: 1d%d", die.sides);
	srand(time(NULL));
	return rand() % die.sides + 1;
}
