#ifndef DICE_H
#define DICE_H

typedef struct s_dice
{
	unsigned int amount;
	unsigned int sides;
}	t_dice;

unsigned int throw(t_dice dice);

#endif
