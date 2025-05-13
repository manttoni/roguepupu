#ifndef DICE_H
#define DICE_H

typedef struct s_dice
{
	int amount;
	int sides;
}	t_dice;

int throw_dice(t_dice dice);

#endif
