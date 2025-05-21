#include "creature.h"
#include "utils.h"
#include "dice.h"
#include "potion.h"
#include "cell.h"
#include "windows.h"
#include "globals.h"
#include "interface.h"
#include "draw.h"
#include "item.h"
#include "weapon.h"

void loot_item(t_creature *looter, t_node **inventory, int i)
{
	t_node *node = get_node(*inventory, i);
	if (node == NULL)
		return;
	t_item *item = (t_item *) node->data;

	add_item(looter, item);
	print_log("%C loots %I", looter, item);
	remove_node(inventory, node);
}

void heal_creature(t_creature *creature, char *amount)
{
	int healing = throw_dice(amount);
	if (creature->health + healing > creature->max_health)
		healing = creature->max_health - creature->health;
	creature->health += healing;
	print_log("%C is healed for {blue}%d{reset} hp", creature, healing);
}

void drink_potion(t_creature *drinker, t_item *potion)
{
	print_log("%C drinks %I", drinker, potion);
	t_potion_data data = potion->data.potion_data;
	if (strncmp(data.effect, "heal", 4) == 0)
		heal_creature(drinker, strchr(data.effect, ' ') + 1);
	free(potion);
	remove_node(&drinker->inventory, get_node_data(drinker->inventory, potion));
	update_stat_win();
}

void use_item(t_creature *user, t_node **inventory_ptr, int i)
{
	t_node *inventory = *inventory_ptr;
	t_node *node = get_node(inventory, i);
	t_item *item = (t_item *) node->data;

	if (is_weapon(item) && user->weapon != item)
		equip(user, item);
	else if (is_potion(item))
		drink_potion(user, item);
}

int has_ranged_weapon(t_creature *creature)
{
	if (creature->weapon == NULL)
		return 0;
	return weapon_has_property(creature->weapon, "ranged");
}

void equip(t_creature *creature, t_item *item)
{
	if (is_weapon(item))
		creature->weapon = item;
	if (creature->ch == '@')
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
			add_item(creature, new_weapon("club"));
			add_item(creature, new_potion("potion of healing"));
			creature->ai = AI_CRAZY_GOBLIN;
			creature->color = COLOR_CREATURE_GOBLIN;
			creature->faction = FACTION_GOBLIN;
			break;
		case '@':
			creature->name = "Rabdin";
			add_item(creature, new_weapon("light crossbow"));
			add_item(creature, new_potion("potion of healing"));
			creature->color = COLOR_BLUE;
			creature->faction = FACTION_PLAYER;
			break;
		default:
			creature->name = "Bob the bug";
			break;
	}
	return creature;
}



