#include "../headers/creature.h"
#include "../headers/utils.h"
#include "../headers/cell.h"
#include "../headers/windows.h"
#include "../headers/weapon.h"
#include "../headers/globals.h"

t_creature *new_creature(char ch)
{
	t_creature *creature = my_calloc(sizeof(t_creature));
	creature->ch = ch;
	creature->health = 10;
	creature->max_health = 10;
	switch (ch)
	{
		case 'g':
			creature->name = "Crazy Goblin";
			creature->weapon = (t_weapon){"Wooden club", (t_dice){1, 4}};
			creature->ai = CRAZY_GOBLIN;
			break;
		case '@':
			creature->name = "Rabdin";
			creature->weapon = (t_weapon){"Sword", (t_dice){1, 8}};
			break;
		default:
			creature->name = "Bob the bug";
			break;
	}
	return creature;
}



