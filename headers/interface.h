#ifndef INTERFACE_H
#define INTERFACE_H

#include "area.h"
#include "creature.h"
#include <ncurses.h>

void print_win(WINDOW *win, char *format, ...);
void print_log(const char *format, ...);
void print_creature_status(t_creature *creature);
void update_stat_win(t_area *area);
void print_legend();

#endif
