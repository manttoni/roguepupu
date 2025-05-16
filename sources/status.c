#include "../headers/cell.h"
#include "../headers/creature.h"
#include "../headers/globals.h"
#include "../headers/interface.h"
#include "../headers/status.h"

int apply_status_effects(t_cell *cell)
{
	t_creature *creature = cell->creature;

	if (creature->bleeding > 0 && creature->health > 0)
		change_color(&cell->terrain->color, bleed(creature) / 2, 0, 0);

	if (creature->health <= 0)
		return FATAL;

	if (creature->stunned > creature->health)
	{
		print_log("%s is stunned", creature->name);
		creature->stunned -= creature->health;
		return STUN;
	}

	if (creature->stunned > 0)
		creature->stunned--;
	return 0;
}

int bleed(t_creature *creature)
{
	int damage = creature->bleeding--;
	if (damage > 0)
		take_damage(creature, damage, BLEEDING);
	return damage;
}

