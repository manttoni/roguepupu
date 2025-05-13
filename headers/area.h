#ifndef AREA_H
#define AREA_H

#include "cell.h"

typedef struct s_node t_node;

typedef struct s_area
{
    char    *name;
	int		height;
	int		width;
	t_cell	*cells;
}	t_area;

t_creature *get_player(t_area *area);
int	get_player_index(t_area *area);
t_cell *get_player_cell(t_area *area);
void print_area(t_area *area);
t_area *new_area(char *file);
void add_creature(t_area *area, t_creature *creature, int index);
t_node *get_creatures(t_area *area);
t_node *get_interactables(t_area *area, int flags);

#endif
