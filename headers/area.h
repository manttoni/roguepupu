#ifndef AREA_H
#define AREA_H

#define AREA(area) ((area)->width * (area)->height)

/* MAPS */
#define DUNGEON "maps/dungeon.txt"

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
void print_area(t_area *area);
t_area *new_area(char *file);
void add_creature(t_area *area, t_creature *creature, int index);
t_node *get_creatures(t_area *area);

#endif
