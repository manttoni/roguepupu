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
	wprintw(leg_win, "  O is door\n");
	wprintw(leg_win, "  0 is locked door\n");
	wprintw(leg_win, "  @ is player\n");
	wprintw(leg_win, " Controls:\n");
	wprintw(leg_win, "  Arrows move\n");
	wprintw(leg_win, "  numpad diagonal move\n");
	wprintw(leg_win, "  e is examine\n");
	wprintw(leg_win, "  u is unlock\n");
	wprintw(leg_win, "  o is open\n");
	wprintw(leg_win, "  a is attack\n");
	wprintw(leg_win, "  p is pick up\n");
	wprintw(leg_win, "  SPACE is pass\n");
	wprintw(leg_win, " Scanning:\n");
	wprintw(leg_win, "  Arrows select object\n");
	wprintw(leg_win, "  enter confirms selected object\n");
	wprintw(leg_win, "  esc cancels interaction\n");
	wprintw(leg_win, " esc exit\n");
}

/* Can be called directly if args == NULL.
 * A better idea is to call this through another function
 * Like print_log, you will get more control */
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
					t_creature *creature = va_arg(args, t_creature*);
					color = creature->color;
					name = creature->name;
					break;
				case 'I':
					t_item *item = va_arg(args, t_item*);
					color = item->color;
					name = item->name;
					break;
				case 'T':
					t_terrain *terrain = va_arg(args, t_terrain*);
					color = terrain->color;
					name = terrain->name;
					break;
				case 'M':
					t_mech *mech = va_arg(args, t_mech*);
					color = mech->color;
					name = mech->name;
					break;
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
	refresh_window(stat_win);
	va_end(args);
}

void print_creature_status(t_creature *creature)
{
	print_stat("%C", creature);

	wattron(stat_win, COLOR_PAIR(COLOR_PAIR_RED));
	wprintw(stat_win, "  Health: %d/%d\n", creature->health, creature->max_health);
	wattroff(stat_win, COLOR_PAIR(COLOR_PAIR_RED));
	int x, y;
	getmaxyx(stat_win, y, x);
	(void) y;
	for (int i = 0; i < x; ++i)
		wprintw(stat_win, "-");
}

void update_stat_win(t_area *area)
{
	werase(stat_win);
	wmove(stat_win, 1, 0);
	t_node *enemies = get_interactables(area, SCAN_ENEMY | SCAN_VISIBLE);

	logger("update_stat_win got interactables");
	print_creature_status(get_player(area));
	logger("player printed");
	while (enemies != NULL)
	{
		t_cell *enemy_cell = (t_cell *) enemies->data;
		print_creature_status(enemy_cell->creature);
		enemies = enemies->next;
	}

	refresh_window(stat_win);
}
