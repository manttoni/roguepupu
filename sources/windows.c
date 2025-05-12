#include "../headers/windows.h"
#include "../headers/utils.h"
#include <ncurses.h>
#include <errno.h>

void init_windows(void)
{
	int y_max, x_max;
	getmaxyx(stdscr, y_max, x_max);
	map_win = newwin((y_max * 3) / 4, x_max, 0, 0);
	log_win = newwin((y_max * 1) / 4, x_max, (y_max * 3 / 4), 0);
	if (map_win == NULL || log_win == NULL)
		end_ncurses(errno);
	box(map_win, 0, 0);
	box(log_win, 0, 0);
	mvwprintw(log_win, 1, 1,  "Hello wolrd");
	refresh();
	wrefresh(map_win);
	wrefresh(log_win);
}

void delete_windows(void)
{
	delwin(map_win);
	delwin(log_win);
}
