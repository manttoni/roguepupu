#include "weapon.h"
#include "item.h"
#include "interface.h"
#include "dice.h"
#include "utils.h"
#include "globals.h"

char *get_damage_type(t_item *weapon)
{
	return weapon->data.weapon_data.damage_type;
}

int weapon_AB(t_item *weapon)
{
	(void)weapon;
	return 0; // later check +1 enchantments here maybe
}

int AB(t_creature *creature, t_item *weapon)
{
	int ab = weapon_AB(weapon);
	if (has_property(weapon, "finesse"))
		ab += max(strmod(creature), dexmod(creature));
	else
		ab += strmod(creature);
	return ab;
}

char *get_dice(t_item *weapon)
{
	return weapon->data.weapon_data.damage;
}

int calculate_damage(t_creature *creature, t_item *weapon)
{
	if (weapon == NULL)
		return 0;
	int damage = AB(creature, weapon);
	char *dice = get_dice(weapon);
	if (has_property(weapon, "versatile") && get_off_hand(creature) == NULL)
		dice = strchr(dice, ';') + 1;
	damage += throw_dice(dice);
	return max(1, damage);
}

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
