#ifndef TERRAIN_H
#define TERRAIN_H

typedef struct s_terrain
{
	unsigned char	ch; // what it looks like on map
	short			burning; // fire in terrain. burning bush,
	short			wet; // amount of liquid on terrain, water, blood, grease..
	short			color; // color
}	t_terrain;

t_terrain *new_terrain(char ch);

#endif
