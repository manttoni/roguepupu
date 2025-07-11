#include "interface.h"
#include "area.h"
#include "windows.h"
#include "creature.h"
#include "cell.h"
#include "draw.h"
#include "globals.h"
#include "utils.h"
#include "equipment.h"
#include "weapon.h"
#include <ncurses.h>

void print_debug(void)
{
	size_t terrain_count = 0;
	size_t item_count = 0;
	size_t creature_count = 0;
	size_t mech_count = 0;
	size_t fungus_count = 0;
	size_t cell_count = AREA(g_area);
	for (size_t i = 0; i < AREA(g_area); ++i)
	{
		t_cell *cell = &g_area->cells[i];
		if (cell->terrain != NULL)
		{
			terrain_count++;
			item_count += list_len(cell->terrain->loot);
		}
		if (cell->item != NULL)
			item_count++;
		if (cell->creature != NULL)
		{
			creature_count++;
			item_count += list_len(cell->creature->inventory);
		}
		if (cell->mech != NULL)
			mech_count++;
		if (cell->fungus != NULL)
			fungus_count++;
	}
	werase(deb_win);
	wprintw(deb_win, "\n");
	wprintw(deb_win, "  Current allocations: %zu\n", list_len(g_allocations));
	wprintw(deb_win, "  Allocations made: %zu\n", g_allocations_made);
	wprintw(deb_win, "  Allocations size: %zu\n", g_allocations_size);
	wprintw(deb_win, "  Nodes allocated:  %zu\n", g_nodes_allocated);
	wprintw(deb_win, "  Cells:     %zu\n", cell_count);
	wprintw(deb_win, "  Terrain:   %zu\n", terrain_count);
	wprintw(deb_win, "  Items:     %zu\n", item_count);
	wprintw(deb_win, "  Creatures: %zu\n", creature_count);
	wprintw(deb_win, "  Mechs:     %zu\n", mech_count);
	wprintw(deb_win, "  Fungi:     %zu\n", fungus_count);
	refresh_window(deb_win);
}

void print_win(WINDOW *win, char *format, va_list args)
{
	char *ptr = format;

	while (*ptr != '\0')
	{
		short color = 0;
		char *name = NULL;
		if (*ptr == '%')
		{
			switch (ptr[1])
			{
				case 'd':
					wprintw(win, "%d", va_arg(args, int));
					break;
				case 's':
					wprintw(win, "%s", va_arg(args, char*));
					break;
				case 'c':
					wprintw(win, "%c", va_arg(args, int));
					break;
				case '%':
					wprintw(win, "%%");
					break;
				case 'C':
				{
					t_creature *creature = va_arg(args, t_creature*);
					if (creature != NULL)
					{
						color = creature->color;
						name = creature->name;
					}
					break;
				}
				case 'I':
				{
					t_item *item = va_arg(args, t_item*);
					if (item != NULL)
					{
						color = item->color;
						name = item->name;
					}
					break;
				}
				case 'T':
				{
					t_terrain *terrain = va_arg(args, t_terrain*);
					if (terrain != NULL)
					{
						color = terrain->color;
						name = terrain->name;
					}
					break;
				}
				case 'M':
				{
					t_mech *mech = va_arg(args, t_mech*);
					if (mech != NULL)
					{
						color = mech->color;
						name = mech->name;
					}
					break;
				}
				case 'F':
				{
					t_fungus *fungus = va_arg(args, t_fungus*);
					if (fungus != NULL)
					{
						color = fungus->color;
						name = fungus->name;
					}
					break;
				}
			}
			if (strchr("CITMF", ptr[1]))
			{
				short pi = pair_id(color, COLOR_BLACK, COLOR_NORMAL);
				wattron(win, COLOR_PAIR(pi));
				wprintw(win, "%s", name);
				wattroff(win, COLOR_PAIR(pi));
			}
			ptr += 2;
		}

		else if (*ptr == '{' && strchr(ptr, '}') != NULL)
		{
			char *closing_bracket = strchr(ptr, '}');
			int code_len = closing_bracket - ptr + 1;

			if (strncmp("{red}", ptr, code_len) == 0)
			{
				color = pair_id(color_id((t_color){5,0,0}), COLOR_BLACK, COLOR_NORMAL);
				wattron(win, COLOR_PAIR(color));
			}
			else if (strncmp("{green}", ptr, code_len) == 0)
			{
				color = pair_id(color_id((t_color){0,5,0}), COLOR_BLACK, COLOR_NORMAL);
				wattron(win, COLOR_PAIR(color));
			}
			else if (strncmp("{blue}", ptr, code_len) == 0)
			{
				color = pair_id(color_id((t_color){0,0,5}), COLOR_BLACK, COLOR_NORMAL);
				wattron(win, COLOR_PAIR(color));
			}
			else if (strncmp("{reset}", ptr, code_len) == 0)
				wattroff(win, A_ATTRIBUTES);
			else
			{
				color = pair_id(atoi(ptr + 1), COLOR_BLACK, COLOR_NORMAL);
				wattron(win, COLOR_PAIR(color));
			}
			ptr += code_len;
		}
		else
		{
			wprintw(win, "%c", *ptr);
			ptr++;
		}
	}
}

/* Sets the va_args which print_win() needs */
static void print_win_va(WINDOW *win, char *format, ...)
{
	va_list args;
	va_start(args, format);
	print_win(win, format, args);
	va_end(args);
}

void print_charactersheet(t_creature *creature)
{
	werase(inv_win);
	wmove(cha_win, 1, 0);

	// name, race, etc
	print_win_va(cha_win, "\t%C\n\n", creature);

	// ablities
	t_abilities abs = creature->abilities;
	print_win_va(cha_win,
			"\tStrength:    \t%d\n"
			"\tDexterity:   \t%d\n"
			"\tConstitution:\t%d\n"
			"\tIntelligence:\t%d\n"
			"\tWisdom:      \t%d\n"
			"\tCharisma:    \t%d\n\n",
			abs.strength, abs.dexterity, abs.constitution,
			abs.intelligence, abs.wisdom, abs.charisma);

	// equipped weapons
	t_item *weapon = get_weapon(creature);
	t_item *offhand = get_offhand(creature);
	if (weapon == NULL)
		print_win_va(cha_win, "\tUnarmed (%d)\n", get_AB(creature, weapon));
	else
		print_win_va(cha_win, "\tWeapon (%d): \t%I\n", get_AB(creature, weapon), weapon);
	if (is_dual_wielding(creature))
		print_win_va(cha_win, "\tOffhand (%d):\t%I\n", get_AB(creature, offhand), offhand);

	// equipped armor
	t_item *armor = get_armor(creature);
	if (armor == NULL)
		print_win_va(cha_win, "\tUnarmored\n");
	else
		print_win_va(cha_win, "\tArmor (%d):  \t%I\n", get_ac(armor), armor);

	// AC, DC etc
	print_win_va(cha_win, "\tAC: %d\n\n", get_AC(creature));


	refresh_window(cha_win);
}

void print_inventory(t_node *inventory, int selected)
{
	werase(inv_win);
	wmove(inv_win, 1, 0);

	int i = 0;
	while (inventory != NULL)
	{
		t_item *item = (t_item *) inventory->data;

		if (i == selected)
			wattron(inv_win, A_REVERSE);
		wprintw(inv_win, " ");
		if (is_equipped(get_player(), item))
			wprintw(inv_win, "  * | ");
		else
			wprintw(inv_win, "      ");
		print_win_va(inv_win, "%I\n", item);
		if (i == selected)
			wattroff(inv_win, A_REVERSE);

		inventory = inventory->next;
		i++;
	}
	refresh_window(inv_win);
}

int open_inventory(t_node **inventory, int mode)
{
	list_sort(*inventory, &compare_item_name);
	print_log("%C opens %s", get_player(), mode == INVENTORY_LOOT ? "remains" : "inventory");
	size_t selected = 0;
	int input = 0;
	int action = 0;
	while (list_len(*inventory) > 0 && input != ESCAPE && input != 'i')
	{
		print_charactersheet(get_player());
		if (selected >= list_len(*inventory))
			selected = list_len(*inventory) - 1;
		print_inventory(*inventory, selected);
		input = getch();
		switch (input)
		{

			case 'i':
			case ESCAPE:
				break;
			case ENTER:
				switch (mode)
				{
					case INVENTORY_PLAYER:
						action = use_item(get_player(), inventory, selected);
						break;
					case INVENTORY_LOOT:
						action = loot_item(get_player(), inventory, selected);
						break;
					default:
						break;
				}
				if (action > 0 && is_in_combat(get_player()))
					input = ESCAPE;
				break;
			case KEY_DOWN:
				if (selected < list_len(*inventory) - 1)
					selected++;
				break;
			case KEY_UP:
				if (selected > 0)
					selected--;
				break;
			default:
				break;
		}
	}
	if (mode == INVENTORY_PLAYER)
		print_log("%C closes inventory", get_player());
	else if (mode == INVENTORY_LOOT)
		print_log("%C closes remains", get_player());
	werase(inv_win);
	refresh_window(inv_win);
	return action;
}

void print_selected(t_cell *cell)
{
	werase(exa_win);
	wmove(exa_win, 1, 2);
	switch (top_entity(cell))
	{
		case ENTITY_CREATURE:
			print_win_va(exa_win, "%C", cell->creature);
			break;
		case ENTITY_ITEM:
			print_win_va(exa_win, "%I", cell->item);
			break;
		case ENTITY_TERRAIN:
			print_win_va(exa_win, "%T", cell->terrain);
			break;
		case ENTITY_MECH:
			print_win_va(exa_win, "%M", cell->mech);
			break;
		case ENTITY_FUNGUS:
			print_win_va(exa_win, "%F", cell->fungus);
		default:
			break;
	}
	wrefresh(exa_win);
}

void print_log(char *format, ...)
{
	va_list args;
	va_start(args, format);
	wprintw(log_win, "  ");
	print_win(log_win, format, args);
	wprintw(log_win, "\n");
	refresh_window(log_win);
	va_end(args);
}

void print_stat(char *format, ...)
{
	va_list args;
	va_start(args, format);
	wprintw(stat_win, "  ");
	print_win(stat_win, format, args);
	wprintw(stat_win, "\n");
	va_end(args);
}

void print_creature_status(t_creature *creature)
{
	// Print creature name
	print_stat("%C", creature);

	// Print creature health bar
	size_t x = win_width(stat_win);
	size_t bar_width = x - 3;
	char buf[x];
	snprintf(buf, sizeof(buf), "Health: %d / %d", creature->health, creature->max_health);
	wprintw(stat_win, "  ");
	short pi = pair_id(COLOR_WHITE, color_id((t_color){1,0,0}), COLOR_NORMAL);
	wattron(stat_win, COLOR_PAIR(pi));
	for (size_t i = 0; i < bar_width; ++i)
	{
		if ((double) i / (bar_width - 1) >= (double) creature->health / creature->max_health)
			wattroff(stat_win, COLOR_PAIR(pi));
		if (i < strlen(buf))
			waddch(stat_win, buf[i]);
		else
			waddch(stat_win, ' ');
	}

	for (size_t i = 0; i < x; ++i)
		wprintw(stat_win, "-");
}

void update_stat_win(void)
{
	werase(stat_win);
	wmove(stat_win, 1, 0);
	t_node *creatures = get_entities(get_player_cell(), TARGET_CREATURE | TARGET_VISIBLE);
	t_node *ptr = creatures;

	while (ptr != NULL)
	{
		t_cell *cell = (t_cell *) ptr->data;
		print_creature_status(cell->creature);
		ptr = ptr->next;
	}
	list_clear(&creatures);
	refresh_window(stat_win);
}
