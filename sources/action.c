#include "../headers/action.h"
#include "../headers/cell.h"
#include "../headers/area.h"
#include "../headers/creature.h"
#include "../headers/game.h"
#include "../headers/windows.h"
#include "../headers/draw.h"
#include "../headers/globals.h"

void attack(t_creature *attacker, t_cell *defender_cell)
{
	int damage = throw_dice(attacker->weapon.damage);
	t_creature *defender = defender_cell->creature;

	print_log("%s attacks %s with a %s", attacker->name, defender->name, attacker->weapon.name);

	if (take_damage(defender, damage, attacker->weapon.damage_type) == FATAL)
		return;
}

int move_creature(t_cell *dst, t_cell *src)
{
	if (dst == NULL)
		return 0;
	if (is_blocked(dst))
		return 0;
	dst->creature = src->creature;
	src->creature = NULL;
	return 1;
}

