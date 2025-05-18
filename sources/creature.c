#include "creature.h"
#include "utils.h"
#include "cell.h"
#include "windows.h"
#include "globals.h"
#include "interface.h"
#include "die.h"

void equip(t_creature *creature, t_item *item)
{
	if (is_weapon(item))
		creature->weapon = item;
	print_log("%s equips %s", creature->name, item->name);
}

void add_item(t_creature *creature, t_item *item)
{
	add_node_last(&creature->inventory, new_node(item));
	if (creature->weapon == NULL && is_weapon(item))
		equip(creature, item);
}

int is_enemy(t_creature *creature)
{
	return creature != NULL && strchr(ENEMY_CHARS, creature->ch) != NULL;
}

void perish(t_creature *creature, e_damage_type damage_type)
{
	switch (damage_type)
	{
		case SLASHING:
			print_log("%s gets cut to pieces", creature->name);
			break;
		case BLUDGEONING:
			print_log("%s is crushed to death", creature->name);
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

int take_damage(t_cell *creature_cell, int damage, e_damage_type damage_type)
{
	t_creature *creature = creature_cell->creature;
	print_log("%s takes {red}%d{reset} %s damage", creature->name, damage, dmg_str(damage_type));
	creature->health -= damage;

	if (is_physical(damage_type))
		creature->bleeding += damage / 2; // this is a cosmetic feature

	if (creature->health <= 0)
	{
		if (creature->bleeding > 0)
			change_color(&creature_cell->terrain->color, 1, 0, 0);
		perish(creature, damage_type);
		return DAMAGE_FATAL;
	}
	return !DAMAGE_FATAL;
}

t_creature *new_creature(char ch, int area_level)
{
	(void)area_level;
	if (strchr(CREATURE_CHARS, ch) == NULL)
		return NULL;
	t_creature *creature = my_calloc(sizeof(t_creature));
	creature->ch = ch;
	creature->health = 20;
	creature->max_health = 20;
	creature->color = COLOR_WHITE;
	switch (ch)
	{
		case 'g':
			creature->name = "Crazy Goblin";
			creature->weapon = new_random_item('W', 1);
			creature->ai = AI_CRAZY_GOBLIN;
			creature->color = COLOR_GREEN;
			creature->faction = FACTION_GOBLIN;
			break;
		case '@':
			creature->color = COLOR_BLUE;
			creature->faction = FACTION_PLAYER;
			break;
		default:
			creature->name = "Bob the bug";
			break;
	}
	return creature;
}



