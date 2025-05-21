#include "action.h"
#include "cell.h"
#include "creature.h"
#include "interface.h"
#include "dice.h"

int act_attack(t_creature *attacker, t_creature *defender)
{
	t_item *weapon = get_weapon(attacker);
	t_item *off_hand = get_off_hand(attacker);

	if (weapon != NULL)
	{
		t_weapon_data main = get_weapon(attacker)->data.weapon_data;
		print_log("%C attacks %C with a %I", attacker, defender, weapon);
		take_damage(defender, get_main_damage(attacker), main.damage_type);

		if (is_dual_wielding(attacker))
		{
			t_weapon_data off = get_off_hand(attacker)->data.weapon_data;
			print_log("%C attacks %C with a %I", attacker, defender, off_hand);
			take_damage(defender, throw_dice(off.damage), off.damage_type);
		}
	}

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

