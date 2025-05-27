#include "../headers/terrain.h"
#include "../headers/utils.h"
#include "../headers/globals.h"
#include <ncurses.h>

t_terrain *new_terrain(char ch, t_area *area)
{
	if (strchr(TERRAIN_CHARS, ch) == NULL)
		return NULL;
	t_terrain *terrain = my_calloc(1, sizeof(*terrain));
	terrain->ch = ch;
	terrain->color = color_id((t_color){1,1,1});
	switch (ch)
	{
		case '#':
			terrain->name = "Wall";
			terrain->color = color_id((t_color){2,2,2});
			break;
		case 'D':
			terrain->name = "Door";
			terrain->color = COLOR_METAL_DOOR;
			break;
		case '.':
			if (rand() % 101 <= area->moisture)
			{
				terrain->name = "wet floor";
				terrain->ch = '~';
			}
			else
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
