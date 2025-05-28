#include "weapon.h"
#include "item.h"
#include "interface.h"
#include "dice.h"
#include "utils.h"
#include "globals.h"
#include "equipment.h"
#include "memory.h"

char *get_damage_type(t_item *weapon)
{
	return weapon->data.weapon_data.damage_type;
}

static int get_weapon_bonus(t_item *weapon)
{
	(void)weapon;
	return 0; // later check +1 enchantments here maybe
}

static int get_ability_mod(t_creature *creature, t_item *weapon)
{
	if (has_property(weapon, "ranged"))
		return dexmod(creature);

	int mod = strmod(creature);
	if (has_property(weapon, "finesse"))
		mod = max(mod, dexmod(creature));
	return mod;
}

char *get_dice(t_item *weapon)
{
	return weapon->data.weapon_data.damage;
}

t_roll damage_roll(t_creature *creature, t_item *weapon)
{
	int mods = get_weapon_bonus(weapon);
	if (get_weapon(creature) == weapon) // only main weapon gets ability mod bonus unless has some feat
		mods += get_ability_mod(creature, weapon);

	char *dice = get_dice(weapon);

	// if using a versatile weapon with both hands, use better dice
	if (has_property(weapon, "versatile") && get_weapon(creature) == get_offhand(creature))
		dice = strchr(dice, ';') + 1;

	return throw(dice, mods, 0);
}

t_roll attack_roll(t_creature *creature, t_item *weapon)
{
	int mods = get_weapon_bonus(weapon);
	mods += get_ability_mod(creature, weapon); // offhand gets bonus to hit also

	char *dice = "1d20";

	return throw(dice, mods, 0);
}

t_item *new_weapon(char *weapon_name)
{
	t_item_group *weapon_group = get_item_group("weapon");
	t_item *weapons = weapon_group->array;
	int count = weapon_group->count;

	for (int i = 0; i < count; ++i)
	{
		if (strcmp(weapons[i].name, weapon_name) == 0)
		{
			t_item *weapon = my_calloc(1, sizeof(*weapon));
			memmove(weapon, &weapons[i], sizeof(*weapon));
			return weapon;
		}
	}
	logger("Weapon not found: %s", weapon_name);
	end_ncurses(1);
	return NULL;
}

t_item *new_random_weapon(void)
{
	t_item_group *weapon_group = get_item_group("weapon");
	t_item *weapons = weapon_group->array;
	int count = weapon_group->count;

	int random = rand() % count;
	t_item *weapon = my_calloc(1, sizeof(*weapon));
	memmove(weapon, &weapons[random], sizeof(*weapon));
	return weapon;
}

int has_property(t_item *item, char *property)
{
	if (item == NULL)
		return 0;
	char **ptr;
	// only weapons have properties, later armor will also have
	if (is_weapon(item))
		ptr = item->data.weapon_data.properties;
	else
		return 0;
	while (ptr != NULL && *ptr != NULL)
	{
		if (strcmp(property, *ptr) == 0)
			return 1;
		ptr++;
	}
	return 0;
}
