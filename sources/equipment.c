#include "equipment.h"
#include "creature.h"
#include "item.h"
#include "weapon.h"
#include "interface.h"

int is_valid_weaponset(t_item *weapon, t_item *offhand)
{
	if (weapon == NULL || offhand == NULL)
		return 1;
	if (has_property(weapon, "light") && has_property(offhand, "light"))
		return 1;
	return 0;
}

int is_equipped(t_creature *creature, t_item *item)
{
	if (get_weapon(creature) == item || get_offhand(creature) == item)
		return 1;
	return 0;
}

int is_dual_wielding(t_creature *creature)
{
	t_item *weapon = get_weapon(creature);
	t_item *offhand = get_offhand(creature);

	if (weapon == offhand)
		return 0;

	if (weapon == NULL || offhand == NULL)
		return 0;

	if (!is_weapon(weapon) || !is_weapon(offhand))
		return 0;

	return 1;
}

t_item *get_offhand(t_creature *creature)
{
	return creature->equipped.offhand;
}

void set_offhand(t_creature *creature, t_item *weapon)
{
	creature->equipped.offhand = weapon;
}

t_item *get_weapon(t_creature *creature)
{
	return creature->equipped.weapon;
}

void set_weapon(t_creature *creature, t_item *weapon)
{
	creature->equipped.weapon = weapon;
	if (has_property(weapon, "two-handed") || has_property(weapon, "versatile"))
	{
		set_offhand(creature, weapon);
	}
}

int unequip(t_creature *creature, t_item *item)
{
	if (get_weapon(creature) == item)
		set_weapon(creature, NULL);
	if (get_offhand(creature) == item)
		set_offhand(creature, NULL);
	if (creature == get_player())
		print_log("%C unequips %I", creature, item);
	return 1;
}

int equip(t_creature *creature, t_item *item)
{
	if (is_equipped(creature, item) || !is_equipment(item))
		return 0;
	if (is_weapon(item))
	{
		if (get_weapon(creature) == NULL && is_valid_weaponset(item, get_offhand(creature)))
		{
			set_weapon(creature, item);
		}
		else if (get_offhand(creature) == NULL && is_valid_weaponset(get_weapon(creature), item))
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
	if (creature->ch == '@')
		print_log("%C equips %I", creature, item);
	return 1;
}

