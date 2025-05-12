#ifndef WINDOWS_H
#define WINDOWS_H

#include <ncurses.h>

extern WINDOW *map_win;
extern WINDOW *log_win;

void init_windows();
void delete_windows();

#endif
