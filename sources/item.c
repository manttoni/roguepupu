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

