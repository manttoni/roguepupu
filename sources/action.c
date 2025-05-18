#include "action.h"
#include "cell.h"
#include "area.h"
#include "creature.h"
#include "game.h"
#include "draw.h"
#include "interface.h"
#include "globals.h"
#include "die.h"

int act_attack(t_cell *attacker_cell, t_cell *defender_cell)
{
	t_creature *attacker = attacker_cell->creature;
	logger("attack(%s, %s)", attacker->name, cell_string(defender_cell));
	t_creature *defender = defender_cell->creature;

	print_log("%s attacks %s with a %s", attacker->name, defender->name, attacker->weapon->name);
	t_die *dice = attacker->weapon->dice;
	while (dice != NULL)
	{
		take_damage(defender, throw_die(*dice), dice->type);
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

