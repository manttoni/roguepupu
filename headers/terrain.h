#ifndef TERRAIN_H
#define TERRAIN_H

#include "color.h"
#include "utils.h"

typedef struct s_terrain
{
	char	*name;
	char	ch; // what it looks like on map
	short	color;
	t_node	*loot;
}	t_terrain;

char *terrain_string(t_terrain *terrain);
t_terrain *new_terrain(char ch, t_area *area);

#endif
