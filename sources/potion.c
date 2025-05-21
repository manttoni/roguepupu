#include "potion.h"
#include "item.h"
#include "utils.h"
#include "globals.h"

t_item *new_potion(char *potion_name)
{
	t_item *potion = my_calloc(sizeof(*potion));
	for (int i = 0; i < g_potion_count; ++i)
	{
		if (strcmp(g_potions[i].name, potion_name) == 0)
		{
			memmove(potion, &g_potions[i], sizeof(*potion));
			return potion;
		}
	}
	logger("Potion not found: %s", potion_name);
	end_ncurses(1);
	return NULL;
}
