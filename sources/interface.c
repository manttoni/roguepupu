#include "interface.h"
#include "area.h"
#include "windows.h"
#include "creature.h"
#include "cell.h"
#include "draw.h"
#include "globals.h"
#include "utils.h"
#include <ncurses.h>

void print_legend(void)
{
	wprintw(leg_win, " Cells:\n");
	wprintw(leg_win, "  . is floor\n");
	wprintw(leg_win, "  # is wall\n");
	wprintw(leg_win, "  D is door\n");
	wprintw(leg_win, "  R is remains\n");
	wprintw(leg_win, "  W is weapon\n");
	wprintw(leg_win, "  @ is player\n");
	wprintw(leg_win, " Controls:\n");
	wprintw(leg_win, "  Arrows move\n");
	wprintw(leg_win, "  numpad diagonal move\n");
	wprintw(leg_win, "  e is examine\n");
	wprintw(leg_win, "  u is unlock\n");
	wprintw(leg_win, "  o is open\n");
	wprintw(leg_win, "  a is attack\n");
	wprintw(leg_win, "  p is pick up\n");
	wprintw(leg_win, "  i is inventory\n");
	wprintw(leg_win, "  SPACE is pass\n");
	wprintw(leg_win, " Scanning:\n");
	wprintw(leg_win, "  any key selects next object\n");
	wprintw(leg_win, "  enter confirms selected object\n");
	wprintw(leg_win, "  ESCAPE cancels interaction\n");
	wprintw(leg_win, " ESCAPE exit\n");
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
			}
			if (strchr("CITM", ptr[1]))
			{
				short pi = pair_id(color, COLOR_BLACK);
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
				color = pair_id(COLOR_RED, COLOR_BLACK);
				wattron(win, COLOR_PAIR(color));
			}
			else if (strncmp("{green}", ptr, code_len) == 0)
			{
				color = pair_id(COLOR_GREEN, COLOR_BLACK);
				wattron(win, COLOR_PAIR(color));
			}
			else if (strncmp("{reset}", ptr, code_len) == 0)
				wattroff(win, A_ATTRIBUTES);
			else
			{
				color = pair_id(atoi(ptr + 1), COLOR_BLACK);
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

void print_inventory(t_node *inventory, int selected)
{
	WINDOW *win = inventory_win;
	werase(win);
	wmove(win, 1, 0);

	int i = 0;
	while (inventory != NULL)
	{
		t_item *item = (t_item *) inventory->data;

		if (i == selected)
			wattron(win, A_REVERSE);
		print_win_va(win, "  %I\n", item);
		if (i == selected)
			wattroff(win, A_REVERSE);

		inventory = inventory->next;
		i++;
	}
	refresh_window(inventory_win);
}

void open_inventory(t_node **inventory, int mode)
{
	print_log("Inventory has %d items", list_len(*inventory));
	int selected = 0;
	int input = 0;
	while (list_len(*inventory) > 0 && input != ESCAPE && input != 'i')
	{
		print_inventory(*inventory, selected);
		if (selected >= list_len(*inventory))
			selected = list_len(*inventory) - 1;
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
						use_item(get_player(g_area), inventory, selected);
						break;
					case INVENTORY_LOOT:
						loot_item(get_player(g_area), inventory, selected);
						break;
					default:
						break;
				}
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
		print_log("%C closes inventory", get_player(g_area));
	else if (mode == INVENTORY_LOOT)
		print_log("%C closes remains", get_player(g_area));
	werase(inventory_win);
	refresh_window(inventory_win);
}

void print_selected(t_cell *cell)
{
	werase(examine_win);
	wmove(examine_win, 1, 2);
	switch (top_entity(cell))
	{
		case CREATURE:
			print_win_va(examine_win, "%C", cell->creature);
			break;
		case ITEM:
			print_win_va(examine_win, "%I", cell->item);
			break;
		case TERRAIN:
			print_win_va(examine_win, "%T", cell->terrain);
			break;
		case MECH:
			print_win_va(examine_win, "%M", cell->mech);
			break;
		default:
			break;
	}
	wrefresh(examine_win);
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
	usleep(100000);
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
	short pi = pair_id(COLOR_WHITE, color_id((t_color){1,0,0}));
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

void update_stat_win(t_area *area)
{
	werase(stat_win);
	wmove(stat_win, 1, 0);
	t_node *enemies = get_interactables(area, SCAN_ENEMY | SCAN_VISIBLE);
	t_node *ptr = enemies;

	print_creature_status(get_player(area));
	while (enemies != NULL)
	{
		t_cell *enemy_cell = (t_cell *) enemies->data;
		print_creature_status(enemy_cell->creature);
		enemies = enemies->next;
	}
	list_clear(&ptr);
	refresh_window(stat_win);
}
