#include "action.h"
#include "cell.h"
#include "area.h"
#include "creature.h"
#include "game.h"
#include "draw.h"
#include "interface.h"
#include "globals.h"
#include "die.h"
#include "windows.h"

int act_attack(t_cell *attacker_cell, t_cell *defender_cell)
{
	t_creature *attacker = attacker_cell->creature;
	t_creature *defender = defender_cell->creature;
	t_item *weapon = attacker->weapon;

	print_log("%s attacks %s with a %s", creature_string(attacker), creature_string(defender), item_string(weapon));
	t_die *dice = weapon->dice;
	while (dice->sides != 0)
	{
		take_damage(defender_cell, throw_die(*dice), dice->type);
		dice++;
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

