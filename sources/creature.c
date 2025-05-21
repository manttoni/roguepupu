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

int is_equipped(t_creature *creature, t_item *item)
{
	if (get_weapon(creature) == item || get_off_hand(creature) == item)
		return 1;
	return 0;
}

int is_dual_wielding(t_creature *creature)
{
	t_item *weapon = get_weapon(creature);
	t_item *off_hand = get_off_hand(creature);

	if (has_property(weapon, "two-handed"))
		return 0;

	if (has_property(weapon, "light") && has_property(off_hand, "light"))
		return 1;
	return 0;
}

int get_main_damage(t_creature *creature)
{
	t_item *weapon = get_weapon(creature);
	t_item *off_hand = get_off_hand(creature);
	char *weapon_damage = weapon->data.weapon_data.damage;

	if (weapon == off_hand && has_property(weapon, "versatile"))
		weapon_damage = strchr(weapon_damage, ';') + 1;
	return throw_dice(weapon_damage);
}

t_item *get_off_hand(t_creature *creature)
{
	return creature->equipped.off_hand;
}

void set_off_hand(t_creature *creature, t_item *weapon)
{
	creature->equipped.off_hand = weapon;
}

t_item *get_weapon(t_creature *creature)
{
	return creature->equipped.weapon;
}

void set_weapon(t_creature *creature, t_item *weapon)
{
	creature->equipped.weapon = weapon;
	if (has_property(weapon, "two-handed"))
		set_off_hand(creature, weapon);
}

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

void unequip(t_creature *creature, t_item *item)
{
	if (get_weapon(creature) == item)
		set_weapon(creature, NULL);
	if (get_off_hand(creature) == item)
		set_off_hand(creature, NULL);
	if (creature == get_player())
		print_log("%C unequips %I", creature, item);
}

void use_item(t_creature *user, t_node **inventory_ptr, int i)
{
	t_node *inventory = *inventory_ptr;
	t_node *node = get_node(inventory, i);
	t_item *item = (t_item *) node->data;

	if (is_equipment(item))
	{
		if (!is_equipped(user, item))
			equip(user, item);
		else
			unequip(user, item);
	}
	else if (is_potion(item))
		drink_potion(user, item);
}

int has_ranged_weapon(t_creature *creature)
{
	if (get_weapon(creature) == NULL)
		return 0;
	return has_property(get_weapon(creature), "ranged");
}

int can_wield_both(t_item *a, t_item *b)
{
	if (a == NULL || b == NULL)
		return 1;
	if (has_property(a, "light") || a == NULL)
		if (has_property(b, "light") || b == NULL)
			return 1;
	print_log("cant wield both");
	return 0;
}

void equip(t_creature *creature, t_item *item)
{
	if (is_equipped(creature, item) || !is_equipment(item))
		return;
	if (is_weapon(item))
	{
		if (get_weapon(creature) == NULL && can_wield_both(get_off_hand(creature), item))
			set_weapon(creature, item);
		else if (get_off_hand(creature) == NULL && can_wield_both(get_weapon(creature), item))
			set_off_hand(creature, item);
		else
			return;
	}
	if (creature->ch == '@')
		print_log("%C equips %I", creature, item);
}

void add_item(t_creature *creature, t_item *item)
{
	add_node_last(&creature->inventory, new_node(item));
	if (is_weapon(item))
		if (get_weapon(creature) == NULL || get_off_hand(creature) == NULL)
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

int take_damage(t_creature *creature, int damage, char *damage_type)
{
	if (damage > 0)
		visual_effect(creature, COLOR_PAIR(COLOR_PAIR_RED));
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
			add_item(creature, new_weapon("dagger"));
			add_item(creature, new_weapon("dagger"));
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



