#include "item.h"
#include "utils.h"
#include "globals.h"
#include <stdlib.h>
#include <ncurses.h>

int is_weapon(t_item *item)
{
	return item->type > WEAPON && item->type < WEAPON_END;
}

int is_potion(t_item *item)
{
	return item->type > POTION && item->type < POTION_END;
}

e_item_type randomize_type(char ch, int area_level)
{
	(void)area_level;
	switch (ch)
	{
		case 'W':
			e_weapon_category category = rand() % WEAPON_TYPES;
			switch (category)
			{
				case CATEGORY_WEAPON_SIMPLE:
					return rand() % SIMPLE_WEAPON_TYPES + SIMPLE + 1;
				default:
					logger("randomize type error");
					end_ncurses(1);
					break;
			}
			break;
		default:
			logger("randomize type error (ch)");
			end_ncurses(1);
			break;
	}
	return 0;
}

e_item_rarity randomize_rarity(int area_level)
{
	int r = rand() % area_level + 1;

	if (r > 0)
		return COMMON;
	return 0;
}

void set_weapon_stats(t_item *weapon)
{
	switch (weapon->type)
	{
		case CLUB:
			weapon->name = "Club";
			weapon->dice = new_dice(BLUDGEONING, "1d4");
			weapon->properties = LIGHT;
			break;
		case DAGGER:
			weapon->name = "Dagger";
			weapon->dice = new_dice(PIERCING, "1d4");
			weapon->properties = FINESSE | LIGHT | THROWN;
			break;
		case GREATCLUB:
			weapon->name = "Greatclub";
			weapon->dice = new_dice(BLUDGEONING, "1d8");
			weapon->properties = TWO_HANDED;
			break;
		case HANDAXE:
			weapon->name = "Handaxe";
			weapon->dice = new_dice(SLASHING, "1d6");
			weapon->properties = LIGHT | THROWN;
			break;
		case JAVELIN:
			weapon->name = "Javelin";
			weapon->dice = new_dice(PIERCING, "1d6");
			weapon->properties = THROWN;
			break;
		case LIGHT_HAMMER:
			weapon->name = "Light hammer";
			weapon->dice = new_dice(BLUDGEONING, "1d4");
			weapon->properties = LIGHT | THROWN;
			break;
		case MACE:
			weapon->name = "Mace";
			weapon->dice = new_dice(BLUDGEONING, "1d6");
			weapon->properties = NONE;
			break;
		case QUARTERSTAFF:
			weapon->name = "Quarterstaff";
			weapon->dice = new_dice(BLUDGEONING, "1d8");
			weapon->properties = VERSATILE;
			break;
		case SICKLE:
			weapon->name = "Sickle";
			weapon->dice = new_dice(SLASHING, "1d4");
			weapon->properties = LIGHT;
			break;
		case SPEAR:
			weapon->name = "Spear";
			weapon->dice = new_dice(PIERCING, "1d8");
			weapon->properties = THROWN | VERSATILE;
			break;
		default:
			break;
	}
}

short get_item_color(e_item_rarity rarity)
{
	switch (rarity)
	{
		case COMMON:
			return COLOR_WHITE;
		case UNCOMMON:
			return COLOR_GREEN;
		case RARE:
			return COLOR_BLUE;
		default:
			return COLOR_WHITE;
	}
}

t_item *new_random_item(char ch, int area_level)
{
	if (strchr(ITEM_CHARS, ch) == NULL)
		return NULL;
	t_item *item = my_calloc(sizeof(*item));
	item->ch = ch;
	switch(ch)
	{
		case 'W':
			item->type = randomize_type(ch, area_level);
			item->rarity = randomize_rarity(area_level);
			set_weapon_stats(item);
			item->color = get_item_color(item->rarity);
			break;
		default:
			logger("Missing initializer");
			end_ncurses(1);
	}
	return item;
}
