#include "../headers/dice.h"
#include <stdlib.h>
#include <time.h>

unsigned int throw(t_dice dice)
{
	srand(time(NULL));
	unsigned int result = 0;
	for (unsigned int i = 0; i < dice.amount; ++i)
		result += 1 + (rand() % dice.sides);
	return result;
}
