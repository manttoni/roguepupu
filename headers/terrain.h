#ifndef TERRAIN_H
#define TERRAIN_H

typedef struct s_terrain
{
	char	*name;
	char	ch; // what it looks like on map
	short	color;
	int		is_locked;
}	t_terrain;

t_terrain *new_terrain(char ch);
char *terrain_string(t_terrain *terrain);

#endif
