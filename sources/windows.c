#include "../headers/windows.h"
#include "../headers/utils.h"
#include <ncurses.h>
#include <errno.h>

void refresh_window(WINDOW *window)
{
	box(window, 0, 0);
	wrefresh(window);
}

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
	wprintw(leg_win, "  i interact\n");
	wprintw(leg_win, "    Arrows select object\n");
	wprintw(leg_win, "    enter confirms selected object\n");
	wprintw(leg_win, "    esc cancels interaction\n");
	wprintw(leg_win, "  esc exit\n");
}

void print_log(char *str)
{
	logger(str);
	wprintw(log_win, "  %s\n", str);
	refresh_window(log_win);
}

WINDOW *my_newwin(t_winset winset)
{
	int y_max, x_max;
	getmaxyx(stdscr, y_max, x_max);
	int height = 0.5 + (double)(y_max * winset.height) / 100;
	int width = 0.5 + (double)(x_max * winset.width) / 100;
	int y = 0.5 + (double)(y_max * winset.y) / 100;
	int x = 0.5 + (double)(x_max * winset.x) / 100;
	WINDOW *window = newwin(height, width, y, x);
	if (window == NULL)
		end_ncurses(errno);
	return window;
}

static void init_log_win(t_winset logset)
{
	log_win = my_newwin(logset);
	scrollok(log_win, TRUE);
	wmove(log_win, 1, 0);
	refresh_window(log_win);
}

static void init_leg_win(t_winset legset)
{
	leg_win = my_newwin(legset);
	scrollok(leg_win, TRUE);
	wmove(leg_win, 1, 0);
	print_legend();
	refresh_window(leg_win);
}

static void init_map_win(t_winset mapset)
{
	map_win = my_newwin(mapset);
	refresh_window(map_win);
}

void init_windows(void)
{
	// winsets values are relative to stdcr size
	t_winset logset, mapset, legset;

	// log window
	logset.height = 25; // 25 % of total size
	logset.width = 75;
	logset.y = 75; // 75 % of scr is above this
	logset.x = 0; // left side of stdscr

	// map window
	mapset.height = 75;
	mapset.width = 75;
	mapset.y = 0; // top of stdscr
	mapset.x = 0;

	// legend window
	legset.height = 100;
	legset.width = 25;
	legset.y = 0;
	legset.x = 75;

	init_log_win(logset);
	init_map_win(mapset);
	init_leg_win(legset);
}

void delete_windows(void)
{
	delwin(map_win);
	delwin(log_win);
}
