#ifndef WINDOWS_H
#define WINDOWS_H

#include <ncurses.h>

extern WINDOW *map_win;
extern WINDOW *log_win;

typedef struct s_winset
{
	int height;
	int width;
	int y;
	int x;
}	t_winset;

void refresh_window(WINDOW *window);
void print_log(char *str);
void init_windows();
void delete_windows();

#endif
