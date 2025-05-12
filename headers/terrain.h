#ifndef TERRAIN_H
#define TERRAIN_H

typedef struct s_terrain
{
	unsigned char	ch; // what it looks like on map
	int				is_locked;
}	t_terrain;

t_terrain *new_terrain(char ch);
char *terrain_string(t_terrain *terrain);

#endif
