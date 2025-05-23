#ifndef DICE_H
#define DICE_H

typedef struct s_roll
{
	char	*dice;
	int		result; // result of dice roll
	int		mods; // modifiers like creature AB and weapon enchantments
	int		advantage; // -1 disadvantage, 0 normal, 1 advantage
}	t_roll;

t_roll throw(char *dice, int modifiers, int advantage);

#endif
