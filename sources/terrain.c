#include "../headers/terrain.h"
#include "../headers/utils.h"
#include "../headers/globals.h"
#include <ncurses.h>

t_terrain *new_terrain(char ch, int area_level)
{
	(void)area_level;
	if (strchr(TERRAIN_CHARS, ch) == NULL)
		return NULL;
	t_terrain *terrain = my_calloc(1, sizeof(t_terrain));
	terrain->ch = ch;
	terrain->color = color_id((t_color){1,1,1});
	switch (ch)
	{
		case '#':
			terrain->name = "Wall";
			terrain->color = COLOR_CAVE_WALL;
			break;
		case 'D':
			terrain->name = "Door";
			terrain->color = COLOR_METAL_DOOR;
			break;
		case '.':
			terrain->name = "Floor";
			//terrain->color = COLOR_CAVE_FLOOR;
			break;
		case 'C':
			terrain->name = "Chest";
			terrain->color = COLOR_METAL_CHEST;
			break;
		case 'R':
			terrain->name = "Remains";
			// color comes from victim
			break;
	}
	return terrain;
}
