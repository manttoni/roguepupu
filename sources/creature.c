#include "creature.h"
#include "utils.h"
#include "cell.h"
#include "windows.h"
#include "globals.h"
#include "interface.h"
#include "draw.h"
#include "item.h"
#include "weapon.h"

void equip(t_creature *creature, t_item *item)
{
	if (is_weapon(item))
		creature->weapon = item;
	print_log("%C equips %I", creature, item);
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

void perish(t_creature *creature, char *damage_type)
{
	print_log("%C perishes (%s)", creature, damage_type);
	if (creature->ch == '@')
	{
		wmove(stat_win, 1, 0);
		print_creature_status(creature);
		refresh_window(stat_win);
		print_log("{red}Game over. Press esc to quit.{reset}");
		while (getch() != ESCAPE);
		end_ncurses(0);
	}
}

int take_damage(t_cell *creature_cell, int damage, char *damage_type)
{
	if (damage > 0)
		visual_effect(creature_cell, COLOR_PAIR(COLOR_PAIR_RED));
	t_creature *creature = creature_cell->creature;
	print_log("%C takes {red}%d{reset} %s damage", creature, damage, damage_type);
	creature->health -= damage;

	if (creature->health <= 0)
	{
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
			creature->weapon = new_weapon("club");
			creature->ai = AI_CRAZY_GOBLIN;
			creature->color = COLOR_CREATURE_GOBLIN;
			creature->faction = FACTION_GOBLIN;
			break;
		case '@':
			creature->name = "Rabdin";
			creature->weapon = new_weapon("spear");
			creature->color = COLOR_BLUE;
			creature->faction = FACTION_PLAYER;
			break;
		default:
			creature->name = "Bob the bug";
			break;
	}
	return creature;
}



