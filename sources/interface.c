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

void print_log(const char *format, ...)
{
	va_list args;
	va_start(args, format);

	wprintw(log_win, "  ");
	vw_printw(log_win, format, args);
	wprintw(log_win, "\n");

	refresh_window(log_win);
	va_end(args);
	usleep(100000);
}

void print_creature_status(t_creature *creature)
{
	wprintw(stat_win, "  %s %c\n", creature->name, creature->ch);
	wprintw(stat_win, "  Health: %d/%d\n", creature->health, creature->max_health);
	wprintw(stat_win, "  Bleeding: %d\n", creature->bleeding);
	wprintw(stat_win, "  Stun level: %d\n", creature->stunned);
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
	t_node *enemies = get_interactables(area, ENEMY | VISIBLE);

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
