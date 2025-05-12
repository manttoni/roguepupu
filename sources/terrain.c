#include "../headers/terrain.h"
#include "../headers/utils.h"
#include <ncurses.h>

char *terrain_string(t_terrain *terrain)
{
	switch(terrain->ch)
	{
		case 'O':
			return "A closed door";
		case '0':
			return "A locked door";
		default:
			return "Some bug";
	}
}

t_terrain *new_terrain(char ch)
{
	t_terrain *terrain = my_calloc(sizeof(t_terrain));
	terrain->ch = ch;
	if (ch == '0')
		terrain->is_locked = 1;
	return terrain;
}
