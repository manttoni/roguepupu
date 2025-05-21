#include "item.h"
#include "utils.h"
#include "globals.h"
#include "color.h"
#include <stdlib.h>
#include <ncurses.h>

int is_weapon(t_item *item)
{
	return strcmp(item->type, "weapon") == 0;
}

int has_property(t_item *item, char *property)
{
	char **ptr = item->properties;
	while (*ptr != NULL)
	{
		if (strcmp(property, *ptr) == 0)
			return 1;
		ptr++;
	}
	return 0;
}
