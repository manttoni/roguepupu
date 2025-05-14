#include "../headers/creature.h"
#include "../headers/utils.h"
#include "../headers/cell.h"
#include "../headers/windows.h"
#include "../headers/weapon.h"
#include "../headers/globals.h"

void perish(t_creature *creature)
{
	print_log("%s perishes", creature->name);
	if (creature->ch == '@')
	{
		print_log("Game over. Press ESC to quit.");
		while (getch() != ESCAPE);
		end_ncurses(0);
	}
}

int take_damage(t_creature *creature, int damage, char *damage_type)
{
	print_log("%s takes %d %s damage", creature->name, damage, damage_type);
	creature->health -= damage;
	if (creature->health <= 0)
	{
		perish(creature);
		return FATAL;
	}
	return !FATAL;
}

int bleed(t_creature *creature)
{
	int damage = creature->bleeding--;
	if (damage > 0)
		take_damage(creature, damage, "bleeding");
	return damage;
}

t_creature *new_creature(char ch)
{
	t_creature *creature = my_calloc(sizeof(t_creature));
	creature->ch = ch;
	creature->health = 10;
	creature->max_health = 10;
	creature->color = COLOR_WHITE;
	switch (ch)
	{
		case 'g':
			creature->name = "Crazy Goblin";
			creature->weapon = (t_weapon){"Wooden club", (t_dice){1, 4}, BLUNT};
			creature->ai = CRAZY_GOBLIN;
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



