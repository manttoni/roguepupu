#include "dice.h"
#include "utils.h"
#include "interface.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>

t_roll get_roll(char *dice, int mods)
{
	t_roll roll;
	roll.result = mods;
	roll.dice = dice;
	int n = atoi(dice);
	int d = atoi(strchr(dice, 'd') + 1);
	for (int i = 0; i < n; ++i)
		roll.result += rand() % d + 1;
	roll.mods = mods;
	return roll;
}

t_roll worse_roll(t_roll a, t_roll b)
{
	if (a.result < b.result)
		return a;
	return b;
}

t_roll better_roll(t_roll a, t_roll b)
{
	if (a.result > b.result)
		return a;
	return b;
}

t_roll throw(char *dice, int modifiers, int advantage)
{
	t_roll roll = get_roll(dice, modifiers);
	roll.advantage = advantage;

	if (advantage == 0)
		return roll;

	t_roll other = get_roll(dice, modifiers);
	other.advantage = advantage;

	if (advantage < 0)
		return worse_roll(roll, other);
	return better_roll(roll, other);
}
