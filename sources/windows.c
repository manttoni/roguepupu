#include "../headers/windows.h"
#include "../headers/utils.h"
#include "../headers/interface.h"
#include <ncurses.h>
#include <errno.h>

void refresh_window(WINDOW *window)
{
	box(window, 0, 0);
	wrefresh(window);
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

WINDOW *my_subwin(WINDOW *win, t_winset winset)
{
	int y_max, x_max;
	getmaxyx(win, y_max, x_max);
	int height = 0.5 + (double)(y_max * winset.height) / 100;
	int width = 0.5 + (double)(x_max * winset.width) / 100;
	int y = 0.5 + (double)(y_max * winset.y) / 100;
	int x = 0.5 + (double)(x_max * winset.x) / 100;
	WINDOW *sub = subwin(win, height, width, y, x);
	if (sub == NULL)
		end_ncurses(errno);
	return sub;
}

static void init_examine_win(void)
{
	examine_win = derwin(map_win, 3, win_width(map_win) - 2, 1, 1);
	if (examine_win == NULL)
	{
		logger("Examine win fail");
		end_ncurses(errno);
	}
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

static void init_stat_win(t_winset statset)
{
	stat_win = my_newwin(statset);
	scrollok(stat_win, TRUE);
	wmove(stat_win, 1, 0);
	refresh_window(stat_win);
}

void init_windows(void)
{
	// winsets values are relative to stdcr size
	t_winset logset, mapset, legset, statset;

	// status window
	statset.height = 100;
	statset.width = 25;
	statset.y = 0;
	statset.x = 0;

	// log window
	logset.height = 25; // 25 % of total size
	logset.width = 50;
	logset.y = 75; // 75 % of scr is above this
	logset.x = 25; // left side of stdscr

	// map window
	mapset.height = 75;
	mapset.width = 50;
	mapset.y = 0; // top of stdscr
	mapset.x = 25;

	// legend window
	legset.height = 100;
	legset.width = 25;
	legset.y = 0;
	legset.x = 75;

	init_stat_win(statset);
	init_log_win(logset);
	init_map_win(mapset);
	init_leg_win(legset);
	init_examine_win();
}

void delete_windows(void)
{
	delwin(map_win);
	delwin(log_win);
}
