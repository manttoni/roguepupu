#include "weapon.h"
#include "item.h"
#include "utils.h"
#include "globals.h"

t_item *new_weapon(char *weapon_name)
{
	t_item *weapon = my_calloc(sizeof(*weapon));
	for (int i = 0; i < g_weapon_count; ++i)
	{
		if (strcmp(g_weapons[i].name, weapon_name) == 0)
		{
			memmove(weapon, &g_weapons[i], sizeof(*weapon));
			return weapon;
		}
	}
	logger("Weapon not found: %s", weapon_name);
	end_ncurses(1);
	return NULL;
}
