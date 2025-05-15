#ifndef TERRAIN_H
#define TERRAIN_H

#include "color.h"

typedef struct s_terrain
{
	char	*name;
	char	ch; // what it looks like on map
	short	color;
	int		is_locked;
}	t_terrain;

int is_closed(t_terrain *terrain);
t_terrain *new_terrain(char ch, int area_level);
char *terrain_string(t_terrain *terrain);

#endif
