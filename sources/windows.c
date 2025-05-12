#include "../headers/windows.h"
#include "../headers/utils.h"
#include <ncurses.h>
#include <errno.h>

void refresh_window(WINDOW *window)
{
	box(window, 0, 0);
	wrefresh(window);
}

void print_log(char *str)
{
	wprintw(log_win, "  %s\n", str);
	refresh_window(log_win);
}

WINDOW * my_newwin(t_winset winset)
{
	int y_max, x_max;
	getmaxyx(stdscr, y_max, x_max);
	int height = (y_max * winset.height) / 100;
	int width = (x_max * winset.width) / 100;
	int y = (y_max * winset.y) / 100;
	int x = (x_max * winset.x) / 100;
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

static void init_map_win(t_winset mapset)
{
	map_win = my_newwin(mapset);
	refresh_window(map_win);
}

void init_windows(void)
{
	// winsets values are relative to stdcr size
	t_winset logset, mapset;
	logset.height = 25; // 25 % of total size
	logset.width = 100;
	logset.y = 75; // 75 % of scr is above this
	logset.x = 0; // left side of stdscr
	mapset.height = 75;
	mapset.width = 100;
	mapset.y = 0; // top of stdscr
	mapset.x = 0;
	init_log_win(logset);
	init_map_win(mapset);
}

void delete_windows(void)
{
	delwin(map_win);
	delwin(log_win);
}
