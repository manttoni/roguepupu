#include "action.h"
#include "cell.h"
#include "weapon.h"
#include "creature.h"
#include "interface.h"
#include "dice.h"

void print_attack_roll(char *hit_miss, t_creature *attacker, t_creature *defender, t_item *weapon, t_roll roll)
{
	char *off = "";
	if (get_offhand(attacker) == weapon && is_dual_wielding(attacker))
		off = "(offhand) ";
	print_log("%C {red}%s{reset} %C with a %I %s( %s + %d = %d vs %d )",
		attacker, hit_miss, defender, weapon, off, roll.dice, roll.mods, roll.result, get_AC(defender));
}

int act_attack(t_creature *attacker, t_creature *defender)
{
	t_item *weapon = get_weapon(attacker);
	t_item *offhand = get_offhand(attacker);

	if (weapon != NULL)
	{
		t_roll dmg = damage_roll(attacker, weapon);
		t_roll atk = attack_roll(attacker, weapon);

		if (atk.result >= get_AC(defender))
		{
			print_attack_roll("hits", attacker, defender, weapon, atk);
			take_damage(defender, dmg, get_damage_type(weapon));
		}
		else
			print_attack_roll("misses", attacker, defender, weapon, atk);


		// attack with offhand if it is same type as main weapon and creature is dual wielding
		if (is_dual_wielding(attacker) && has_property(weapon, "ranged") == has_property(offhand, "ranged") && defender->health > 0)
		{
			dmg = damage_roll(attacker, offhand);
			atk = attack_roll(attacker, offhand);

			if (atk.result >= get_AC(defender))
			{
				print_attack_roll("hits", attacker, defender, offhand, atk);
				take_damage(defender, dmg, get_damage_type(offhand));
			}
			else
				print_attack_roll("misses", attacker, defender, offhand, atk);
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

