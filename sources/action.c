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
	print_log("%s attacks %s with a %s, dealing %d damage", attacker->name, defender->name, attacker->weapon.name, damage);

	if (damage < defender->health)
	{
		defender->health -= damage;
		return;
	}
	defender->health = 0;
	print_log("%s dies", defender->name);
	defender_cell->terrain->ch = 'C';

	if (defender->ch == '@')
	{
		print_log("Game over. Press ESC to quit.");
		while (getch() != ESCAPE);
		end_ncurses(0);
	}
	free(defender);
	defender_cell->creature = NULL;
	return;
}

int move_creature(t_cell *dst, t_cell *src)
{
	if (dst == NULL)
		return 0;
	if (dst->terrain->ch == '#')
		return 0;
	if (dst->terrain->ch == '0')
	{
		print_log("Door is locked");
		return 0;
	}
	if (dst->terrain->ch == 'O')
	{
		print_log("Door is closed");
		return 0;
	}
	if (dst->creature != NULL)
		return 0;
	dst->creature = src->creature;
	src->creature = NULL;
	return 1;
}

