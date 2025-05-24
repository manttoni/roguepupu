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

int get_AC(t_creature *creature)
{
	int AC = 10;
	AC += dexmod(creature);
	return AC;
}

int get_darkvision(t_creature *creature)
{
	// calculates by race, class, items etc
	(void) creature;
	return 120 / CELL_SIZE; // how many feet are the cells
}

static int mod(int val)
{
	return (val - 10) / 2;
}

int strmod(t_creature *creature)
{
	return mod(creature->abilities.strength);
}

int dexmod(t_creature *creature)
{
	return mod(creature->abilities.dexterity);
}

int conmod(t_creature *creature)
{
	return mod(creature->abilities.constitution);
}

int intmod(t_creature *creature)
{
	return mod(creature->abilities.intelligence);
}

int wismod(t_creature *creature)
{
	return mod(creature->abilities.wisdom);
}

int chamod(t_creature *creature)
{
	return mod(creature->abilities.charisma);
}

int is_equipped(t_creature *creature, t_item *item)
{
	if (get_weapon(creature) == item || get_offhand(creature) == item)
		return 1;
	return 0;
}

int is_dual_wielding(t_creature *creature)
{
	t_item *weapon = get_weapon(creature);
	t_item *offhand = get_offhand(creature);

	if (weapon == offhand)
		return 0;

	if (weapon == NULL || offhand == NULL)
		return 0;

	if (!is_weapon(weapon) || !is_weapon(offhand))
		return 0;

	return 1;
}

t_item *get_offhand(t_creature *creature)
{
	return creature->equipped.offhand;
}

void set_offhand(t_creature *creature, t_item *weapon)
{
	creature->equipped.offhand = weapon;
}

t_item *get_weapon(t_creature *creature)
{
	return creature->equipped.weapon;
}

void set_weapon(t_creature *creature, t_item *weapon)
{
	creature->equipped.weapon = weapon;
	if (has_property(weapon, "two-handed"))
		set_offhand(creature, weapon);
	if (has_property(weapon, "versatile"))
		set_offhand(creature, weapon);
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
	t_roll heal_roll = throw(amount, atoi(find_next_of(amount, "-+")), 0);
	int healing = heal_roll.result;
	creature->health = min(creature->health + healing, creature->max_health);
	print_log("%C is healed for {blue}%d (%s){reset} hp", creature, healing, amount);
}

void drink_potion(t_creature *drinker, t_item *potion)
{
	print_log("%C drinks %I", drinker, potion);
	t_potion_data data = potion->data.potion_data;

	// heal potion format: "heal xdy +z"
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
	if (get_offhand(creature) == item)
		set_offhand(creature, NULL);
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

int is_valid_weaponset(t_item *weapon, t_item *offhand)
{
	if (weapon == NULL || offhand == NULL)
		return 1;
	if (has_property(weapon, "light") && has_property(offhand, "light"))
		return 1;
	return 0;
}

void equip(t_creature *creature, t_item *item)
{
	if (is_equipped(creature, item) || !is_equipment(item))
		return;
	if (is_weapon(item))
	{
		if (get_weapon(creature) == NULL && is_valid_weaponset(item, get_offhand(creature)))
			set_weapon(creature, item);
		else if (get_offhand(creature) == NULL && is_valid_weaponset(get_weapon(creature), item))
			set_offhand(creature, item);
		else
		{
			print_log("Can't equip %I. Unequip something first.", item);
			return;
		}
	}
	if (creature->ch == '@')
		print_log("%C equips %I", creature, item);
}

void add_item(t_creature *creature, t_item *item)
{
	add_node_last(&creature->inventory, new_node(item));
	if (is_weapon(item))
		if (get_weapon(creature) == NULL || get_offhand(creature) == NULL)
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

int take_damage(t_creature *creature, t_roll damage_roll, char *damage_type)
{
	int damage = damage_roll.result;
	if (damage > 0)
		visual_effect(creature, COLOR_PAIR(COLOR_PAIR_RED));

	print_log("%C takes {red}%d{reset} %s damage ( %s + %d )", creature, damage, damage_type, damage_roll.dice, damage_roll.mods);
	creature->health -= damage;

	if (creature->health <= 0)
	{
		perish(creature, damage_type);
		return DAMAGE_FATAL;
	}
	return !DAMAGE_FATAL;
}

void randomize_abilities(t_abilities *a)
{
	a->strength = throw("4d6", 0, 0).result;
	a->dexterity = throw("4d6", 0, 0).result;
	a->constitution = throw("4d6", 0, 0).result;
	a->intelligence = throw("4d6", 0, 0).result;
	a->wisdom = throw("4d6", 0, 0).result;
	a->charisma = throw("4d6", 0, 0).result;
}

void set_max_health(t_creature *creature)
{
	t_roll hit_die = throw("1d8", 0, 0);
	int health = hit_die.result + conmod(creature);
	creature->health = health;
	creature->max_health = health;
}

t_creature *new_goblin(char *goblin_name)
{
	t_creature_group *goblin_group = get_creature_group("goblin");
	t_creature *goblins = goblin_group->array;
	int count = goblin_group->count;

	for (int i = 0; i < count; ++i)
	{
		if (strcmp(goblins[i].name, goblin_name) == 0)
		{
			t_creature *goblin = my_calloc(1, sizeof(t_creature));
			memmove(goblin, &goblins[i], sizeof(*goblin));
			return goblin;
		}
	}
	logger("Goblin not found: %s", goblin_name);
	end_ncurses(1);
	return NULL;
}

t_creature *new_creature(char ch, int area_level)
{
	(void)area_level;
	if (strchr(CREATURE_CHARS, ch) == NULL)
		return NULL;
	t_creature *creature;
	switch (ch)
	{
		case 'g':
			creature = new_goblin("crazy goblin");
			add_item(creature, new_random_weapon());
			break;
		case '@':
			creature = my_calloc(1, sizeof(t_creature));
			creature->name = "Rabdin";
			add_item(creature, new_weapon("light crossbow"));
			add_item(creature, new_weapon("dagger"));
			add_item(creature, new_weapon("dagger"));
			add_item(creature, new_potion("potion of healing"));
			creature->color = COLOR_BLUE;
			creature->faction = FACTION_PLAYER;
			randomize_abilities(&creature->abilities);
			break;
		default:
			break;
	}
	creature->ch = ch;
	set_max_health(creature);
	return creature;
}



