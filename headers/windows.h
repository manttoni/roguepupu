#ifndef WINDOWS_H
#define WINDOWS_H

#include <ncurses.h>

extern WINDOW *stat_win;
extern WINDOW *map_win;
extern WINDOW *log_win;
extern WINDOW *exa_win;
extern WINDOW *inv_win;
extern WINDOW *cha_win;
extern WINDOW *deb_win;

typedef struct s_winset
{
	int height;
	int width;
	int y;
	int x;
}	t_winset;

WINDOW *my_newwin(t_winset winset);
void refresh_window(WINDOW *window);
void init_windows(void);
void delete_windows(void);

#endif
