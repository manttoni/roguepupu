#include "../headers/terrain.h"
#include "../headers/utils.h"
#include "../headers/globals.h"
#include <ncurses.h>

int is_closed(t_terrain *terrain)
{
	return terrain != NULL && strchr(CLOSED_CHARS, terrain->ch) != NULL;
}

t_terrain *new_terrain(char ch, int area_level)
{
	(void)area_level;
	if (strchr(TERRAIN_CHARS, ch) == NULL)
		return NULL;
	t_terrain *terrain = my_calloc(sizeof(t_terrain));
	terrain->ch = ch;
	terrain->color = color_id((t_color){0,0,0});
	switch (ch)
	{
		case '#':
			terrain->name = "Wall";
			break;
		case 'D':
			terrain->name = "Door";
			break;
		case '.':
			terrain->name = "Floor";
			break;
		case 'C':
			terrain->name = "Chest";
			break;
		case 'R':
			terrain->name = "Remains";
			break;
	}
	return terrain;
}
