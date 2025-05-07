#ifndef AREA_H
#define AREA_H

#define HOUSE "maps/house.txt"

#include "cell.h"

typedef struct s_area
{
    char    *name;
	int		height;
	int		width;
	t_cell	*cells;
}	t_area;

t_creature *get_player(t_area *area);
int	get_player_index(t_area *area);
void print_area(t_area *area);
void populate(t_area *area, char *raw);
t_area *new_area(char *file);

#endif
