#include "../headers/terrain.h"
#include "../headers/utils.h"
#include "../headers/globals.h"
#include <ncurses.h>

t_terrain *new_terrain(char ch)
{
	t_terrain *terrain = my_calloc(sizeof(t_terrain));
	terrain->ch = ch;
	terrain->color = color_id((t_color){0,0,0});
	switch (ch)
	{
		case '#':
			terrain->name = "Wall";
			break;
		case '0':
			terrain->name = "Locked door";
			break;
		case 'O':
			terrain->name = "Closed door";
			break;
		case '.':
			terrain->name = "Floor";
			break;
		case 'C':
			terrain->name = "Corpse";
			break;
	}
	return terrain;
}
