#include "item.h"
#include "utils.h"
#include "globals.h"
#include "color.h"
#include <stdlib.h>
#include <ncurses.h>

int is_potion(t_item *item)
{
	return strcmp(item->type, "potion") == 0;
}

int is_weapon(t_item *item)
{
	return strcmp(item->type, "weapon") == 0;
}

int weapon_has_property(t_item *item, char *property)
{
	if (!is_weapon(item))
		return 0;
	char **ptr = item->data.weapon_data.properties;
	while (*ptr != NULL)
	{
		if (strcmp(property, *ptr) == 0)
			return 1;
		ptr++;
	}
	return 0;
}
