#include "action.h"
#include "cell.h"
#include "creature.h"
#include "interface.h"
#include "dice.h"

int act_attack(t_cell *attacker_cell, t_cell *defender_cell)
{
	t_creature *attacker = attacker_cell->creature;
	t_creature *defender = defender_cell->creature;
	t_item *weapon = attacker->weapon;
	u_item_data item_data = weapon->data;
	t_weapon_data data = item_data.weapon_data;

	print_log("%C attacks %C with a %I", attacker, defender, weapon);
	take_damage(defender_cell, throw_dice(data.damage), data.damage_type);
	return 0;
}

int act_move(t_cell *dst, t_cell *src)
{
	if (dst == NULL)
		return 0;
	if (is_blocked(dst))
		return 0;
	dst->creature = src->creature;
	src->creature = NULL;
	return 1;
}

