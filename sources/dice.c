#include "dice.h"
#include "utils.h"
#include "interface.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>

int throw_dice(char *dice)
{
	srand(time(NULL));
	int n = atoi(dice);
	int d = atoi(strchr(dice, 'd') + 1);
	int result = 0;
	for (int i = 0; i < n; ++i)
		result += rand() % d + 1;
	return result;
}
