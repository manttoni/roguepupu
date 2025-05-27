#include "item.h"
#include "utils.h"
#include "globals.h"
#include "color.h"
#include <stdlib.h>
#include <ncurses.h>

t_item *new_item(char *category, char *name)
{
	t_item_group *group = get_item_group(category);
	t_item *array = group->array;
	size_t count = group->count;

	for (size_t i = 0; i < count; ++i)
	{
		if (strcmp(array[i].name, name) == 0)
		{
			t_item *item = my_calloc(1, sizeof(*item));
			memmove(item, &array[i], sizeof(*item));
			return item;
		}
	}
	logger("Item not found: %s : %s", category, name);
	end_ncurses(1);
}

int is_equipment(t_item *item)
{
	return is_weapon(item);
}

int is_potion(t_item *item)
{
	return strcmp(item->type, "potion") == 0;
}

int is_weapon(t_item *item)
{
	return strcmp(item->type, "weapon") == 0;
}

