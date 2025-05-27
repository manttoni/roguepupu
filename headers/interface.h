#ifndef INTERFACE_H
#define INTERFACE_H

#include "area.h"
#include "creature.h"
#include <ncurses.h>

int open_inventory(t_node **inventory, int mode);
void print_selected(t_cell *cell);
void print_win(WINDOW *win, char *format, va_list args);
void print_log(char *format, ...);
void print_creature_status(t_creature *creature);
void update_stat_win(void);
void print_legend(void);

#endif
