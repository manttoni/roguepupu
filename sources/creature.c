#include "../headers/creature.h"
#include "../headers/utils.h"
#include "../headers/cell.h"
#include "../headers/windows.h"
#include "../headers/weapon.h"

t_creature *new_creature(char ch)
{
	t_creature *creature = my_calloc(sizeof(t_creature));
	creature->ch = ch;
	creature->action = NONE;
	creature->health = 10;
	switch (ch)
	{
		case 'g':
			creature->name = "Crazy Goblin";
			creature->weapon = (t_weapon){"Wooden club", (t_dice){1, 4}};
			creature->action = CRAZY_GOBLIN;
			break;
		case 'd':
			creature->name = "Combat practice dummy";
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

