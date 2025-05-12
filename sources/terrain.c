#include "../headers/terrain.h"
#include "../headers/utils.h"
#include <ncurses.h>

t_terrain *new_terrain(char ch)
{
	t_terrain *terrain = my_calloc(sizeof(t_terrain));
	terrain->ch = ch;
	if (ch == '^')
		terrain->burning = 10;
	return terrain;
}
