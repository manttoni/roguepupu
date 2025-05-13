#include "../headers/dice.h"
#include <stdlib.h>
#include <time.h>

int throw_dice(t_dice dice)
{
	srand(time(NULL));
	unsigned int result = 0;
	for (int i = 0; i < dice.amount; ++i)
		result += 1 + (rand() % dice.sides);
	return result;
}
