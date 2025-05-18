#include "../headers/area.h"
#include "../headers/windows.h"
#include "../headers/creature.h"
#include "../headers/cell.h"
#include "../headers/draw.h"
#include "../headers/globals.h"
#include "../headers/utils.h"
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

void print_win(WINDOW *win, char *format, va_list args)
{
	char *ptr = format;
	short color = 0;

	while (*ptr != '\0')
	{
		if (*ptr == '%')
		{
			if (ptr[1] == 'd')
				wprintw(win, "%d", va_arg(args, int));
			else if (ptr[1] == 's')
				wprintw(win, "%s", va_arg(args, char *));
			else
			{
				logger("Printing some unimplemented types");
				end_ncurses(1);
			}
			ptr += 2;
		}
		else if (*ptr == '{' && strchr(ptr, '}') != NULL)
		{
			char *closing_bracket = strchr(ptr, '}');
			int code_len = closing_bracket - ptr + 1;
			if (strncmp("{red}", ptr, code_len) == 0)
			{
				color = COLOR_RED;
				wattron(win, COLOR_PAIR(COLOR_RED));
			}
			else if (strncmp("{green}", ptr, code_len) == 0)
			{
				color = COLOR_GREEN;
				wattron(win, COLOR_PAIR(COLOR_GREEN));
			}
			else if (strncmp("{reset}", ptr, code_len) == 0)
			{
				wattroff(win, COLOR_PAIR(color));
				color = 0;
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

void print_creature_status(t_creature *creature)
{
	int pairid = pair_id(creature->color, COLOR_BLACK);
	wattron(stat_win, COLOR_PAIR(pairid));
	wprintw(stat_win, "  %s %c\n", creature->name, creature->ch);
	wattroff(stat_win, COLOR_PAIR(pairid));

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
