#include "potion.h"
#include "item.h"
#include "utils.h"
#include "globals.h"

t_item *new_potion(char *potion_name)
{
	t_item_group *potion_group = get_item_group("potion");
	t_item *potions = potion_group->array;
	int count = potion_group->count;

	for (int i = 0; i < count; ++i)
	{
		if (strcmp(potions[i].name, potion_name) == 0)
		{
			t_item *potion = my_calloc(1, sizeof(*potion));
			memmove(potion, &potions[i], sizeof(*potion));
			return potion;
		}
	}
	logger("Potion not found: %s", potion_name);
	end_ncurses(1);
}
