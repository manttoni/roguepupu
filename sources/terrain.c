#include "../headers/terrain.h"
#include "../headers/utils.h"
#include <ncurses.h>

t_terrain *new_terrain(char ch)
{
	t_terrain *terrain = my_malloc(sizeof(t_terrain));
	terrain->ch = ch;
	terrain->color = COLOR_WHITE;
	return terrain;
}
