#include "item.h"
#include "utils.h"
#include "globals.h"
#include "color.h"
#include <stdlib.h>
#include <ncurses.h>

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

int has_property(t_item *item, char *property)
{
	if (item == NULL)
		return 0;
	char **ptr;
	if (is_weapon(item))
		ptr = item->data.weapon_data.properties;
	else
		return 0;
	while (*ptr != NULL)
	{
		if (strcmp(property, *ptr) == 0)
			return 1;
		ptr++;
	}
	return 0;
}
