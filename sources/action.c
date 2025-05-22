#include "action.h"
#include "cell.h"
#include "weapon.h"
#include "creature.h"
#include "interface.h"
#include "dice.h"

int act_attack(t_creature *attacker, t_creature *defender)
{
	t_item *weapon = get_weapon(attacker);
	t_item *off_hand = get_off_hand(attacker);

	int weapon_damage = calculate_damage(attacker, weapon);
	if (weapon != NULL)
	{
		print_log("%C attacks %C with a %I", attacker, defender, weapon);
		take_damage(defender, weapon_damage, get_damage_type(weapon));

		int off_hand_damage = calculate_damage(attacker, off_hand);
		if (is_dual_wielding(attacker))
		{
			print_log("%C attacks %C with a %I (off-hand)", attacker, defender, off_hand);
			take_damage(defender, off_hand_damage, get_damage_type(off_hand));
		}
	}

	return 0;
}

/* Returns 1 if move was possible, 0 otherwise */
int act_move(e_direction dir, t_cell *from)
{
	t_cell *to = neighbor(dir, from);
	if (to == NULL || is_blocked(to))
		return 0;
	to->creature = from->creature;
	from->creature = NULL;
	return 1;
}

