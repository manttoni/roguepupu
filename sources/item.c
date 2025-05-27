#include "item.h"
#include "utils.h"
#include "globals.h"
#include "color.h"
#include <stdlib.h>
#include <ncurses.h>

t_item *new_harvest(t_fungus *fungus)
{
	t_item *harvest = my_calloc(1, sizeof(*harvest));
	harvest->name = fungus->name;
	harvest->type = "fungus";
	harvest->rarity = fungus->rarity;
	harvest->ch = 'F';
	harvest->color = fungus->color;
	return harvest;
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

