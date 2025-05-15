#include "../headers/item.h"
#include "../headers/utils.h"
#include "../headers/globals.h"
#include <stdlib.h>

char *generate_item_name(t_item *item)
{
	switch (item->class)
	{
		case WEAPON:
		switch (item->type)
		{
			case SWORD:
				return "Sword";
			case AXE:
				return "Axe";
			case CLUB:
				return "Club";
			default:
				return "Bug smasher!";
		}
		break;
		default:
			break;
	}
	return "BUG SMASHER";
}

t_item *new_item(char ch, int area_level)
{
	if (strchr(ITEM_CHARS, ch) == NULL)
		return NULL;
	t_item *item = my_calloc(sizeof(*item));
	item->ch = ch;
	switch(ch)
	{
		case 'W':
			item->class = WEAPON;
			item->type = rand() % WEAPON_TYPES + WEAPON + 1;
			item->level = rand() % area_level + 1;
			item->damage = (t_dice){1,8};
			switch (item->type)
			{
				case AXE:
				case SWORD:
					item->damage_type = SLASHING;
					break;
				case CLUB:
					item->damage_type = BLUNT;
					break;
				default:
					break;
			}
			break;
		default:
			logger("Missing initializer");
			end_ncurses(1);
	}
	item->name = generate_item_name(item);
	return item;
}
