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
	exa_win = derwin(map_win, 3, win_width(map_win) - 2, 1, 1);
	if (exa_win == NULL)
	{
		logger("Examine win fail");
		end_ncurses(errno);
	}
}

static WINDOW *init_window(int height, int width, int y, int x)
{
	t_winset winset = (t_winset){height, width, y, x};
	WINDOW *window = my_newwin(winset);
	refresh_window(window);
	return window;
}

void init_windows(void)
{
	stat_win = init_window(50, 25, 0, 0);
	log_win = init_window(25, 50, 75, 25);
	scrollok(log_win, TRUE);
	wmove(log_win, 1, 0);
	map_win = init_window(75, 50, 0, 25);
	cha_win = init_window(50, 25, 0, 75);
	init_examine_win();
	inv_win = init_window(50, 25, 50, 75);
	deb_win = init_window(50, 25, 50, 0);
}

void delete_windows(void)
{
	delwin(map_win);
	delwin(log_win);
}
