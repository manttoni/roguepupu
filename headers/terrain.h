#ifndef TERRAIN_H
#define TERRAIN_H

#include "color.h"

typedef struct s_terrain
{
	char	*name;
	char	ch; // what it looks like on map
	short	color;
}	t_terrain;

t_terrain *new_terrain(char ch, int area_level);

#endif
