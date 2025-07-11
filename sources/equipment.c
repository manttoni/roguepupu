#include "equipment.h"
#include "creature.h"
#include "item.h"
#include "weapon.h"
#include "interface.h"

int get_ac(t_item *item)
{
	if (item == NULL)
		return 0;

	// later anything can increase AC (enchants etc)
	if (strcmp(item->type, "armor") != 0)
		return 0;
	return item->data.armor_data.ac;
}

void swap_weapon_set(t_creature *creature)
{
	t_weapon_set tmp = creature->equipped.weapon_set;
	creature->equipped.weapon_set = creature->equipped.other_set;
	creature->equipped.other_set = tmp;
}

int is_valid_weapon_set(t_item *weapon, t_item *offhand)
{
	if (weapon == NULL || offhand == NULL)
		return 1;
	if (has_property(weapon, "light") && has_property(offhand, "light"))
		return 1;
	return 0;
}

int is_equipped(t_creature *creature, t_item *item)
{
	if (item_equals(get_weapon(creature), item)
		|| item_equals(get_offhand(creature), item)
		|| item_equals(get_armor(creature), item))
		return 1;
	return 0;
}

int is_dual_wielding(t_creature *creature)
{
	t_item *weapon = get_weapon(creature);
	t_item *offhand = get_offhand(creature);

	if (item_equals(weapon, offhand))
		return 0;

	if (weapon == NULL || offhand == NULL)
		return 0;

	if (!is_weapon(weapon) || !is_weapon(offhand))
		return 0;

	if (!has_property(weapon, "light") || !has_property(offhand, "light"))
		return 0;

	return 1;
}

t_item *get_armor(t_creature *creature)
{
	return creature->equipped.armor;
}

void set_armor(t_creature *creature, t_item *armor)
{
	creature->equipped.armor = armor;
}

t_item *get_offhand(t_creature *creature)
{
	return creature->equipped.weapon_set.offhand;
}

void set_offhand(t_creature *creature, t_item *weapon)
{
	creature->equipped.weapon_set.offhand = weapon;
}

t_item *get_weapon(t_creature *creature)
{
	return creature->equipped.weapon_set.weapon;
}

void set_weapon(t_creature *creature, t_item *weapon)
{
	creature->equipped.weapon_set.weapon = weapon;
	if (has_property(weapon, "two-handed") || has_property(weapon, "versatile"))
	{
		set_offhand(creature, weapon);
	}
}

int unequip(t_creature *creature, t_item *item)
{
	if (item_equals(get_weapon(creature), item))
		set_weapon(creature, NULL);
	if (item_equals(get_offhand(creature), item))
		set_offhand(creature, NULL);
	if (item_equals(get_armor(creature), item))
		set_armor(creature, NULL);
	if (creature_equals(creature, get_player()))
		print_log("%C unequips %I", creature, item);
	return 1;
}

int equip(t_creature *creature, t_item *item)
{
	if (is_equipped(creature, item) || !is_equipment(item))
		return 0;
	if (is_weapon(item))
	{
		if (get_weapon(creature) == NULL && is_valid_weapon_set(item, get_offhand(creature)))
		{
			set_weapon(creature, item);
		}
		else if (get_offhand(creature) == NULL && is_valid_weapon_set(get_weapon(creature), item))
		{
			set_offhand(creature, item);
		}
		else
		{
			if (creature->ch == '@')
				print_log("Can't equip %I. Unequip something first.", item);
			return 0;
		}
	}
	if (is_armor(item))
	{
		set_armor(creature, item);
	}
	if (creature->ch == '@')
		print_log("%C equips %I", creature, item);
	return 1;
}

