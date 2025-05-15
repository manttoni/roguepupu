#include "../headers/creature.h"
#include "../headers/utils.h"
#include "../headers/cell.h"
#include "../headers/windows.h"
#include "../headers/weapon.h"
#include "../headers/globals.h"
#include "../headers/interface.h"

void perish(t_creature *creature, e_damage_type damage_type)
{
	switch (damage_type)
	{
		case SLASHING:
			print_log("%s gets cut to pieces", creature->name);
			break;
		case BLUNT:
			print_log("%s is crushed to death", creature->name);
			break;
		case BLEEDING:
			print_log("%s bleeds to death", creature->name);
			break;
		default:
			print_log("%s perishes", creature->name);
			break;
	}
	if (creature->ch == '@')
	{
		wmove(stat_win, 1, 0);
		print_creature_status(creature);
		refresh_window(stat_win);
		print_log("Game over. Press esc to quit.");
		while (getch() != ESCAPE);
		end_ncurses(0);
	}
}

char *dmg_str(e_damage_type damage_type)
{
	switch (damage_type)
	{
		case SLASHING:
			return "slashing";
		case BLUNT:
			return "blunt";
		case BLEEDING:
			return "bleeding";
		default:
			return "";
	}
}

int take_damage(t_creature *creature, int damage, e_damage_type damage_type)
{
	print_log("%s takes %d %s damage", creature->name, damage, dmg_str(damage_type));
	creature->health -= damage;

	// apply statuses
	switch (damage_type)
	{
		case SLASHING:
			creature->bleeding += damage / 2;
			if (creature->bleeding > 0)
				print_log("%s gets a bleeding wound", creature->name);
			break;
		case BLUNT:
			creature->stunned += damage;
			if (creature->stunned > 0)
				print_log("%s is hit with a stunning blow", creature->name);
			break;
		default:
			break;
	}

	if (creature->health <= 0)
	{
		perish(creature, damage_type);
		return FATAL;
	}
	return !FATAL;
}



t_creature *new_creature(char ch)
{
	logger("new_creature(%c)", ch);
	t_creature *creature = my_calloc(sizeof(t_creature));
	creature->ch = ch;
	creature->health = 20;
	creature->max_health = 20;
	creature->color = COLOR_WHITE;
	switch (ch)
	{
		case 'g':
			creature->name = "Crazy Goblin";
			creature->weapon = (t_weapon){"Wooden club", (t_dice){1, 4}, BLUNT};
			creature->ai = CRAZY_GOBLIN;
			creature->color = COLOR_GREEN;
			break;
		case '@':
			creature->name = "Rabdin";
			creature->weapon = (t_weapon){"Sword", (t_dice){1, 8}, SLASHING};
			break;
		default:
			creature->name = "Bob the bug";
			break;
	}
	return creature;
}



