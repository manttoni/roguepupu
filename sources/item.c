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

