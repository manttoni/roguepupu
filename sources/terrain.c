#include "terrain.h"
#include "utils.h"
#include "globals.h"
#include "memory.h"
#include <ncurses.h>

t_terrain *spawn_terrain(char ch, t_area *area)
{
	if (strchr(TERRAIN_CHARS, ch) == NULL)
	{
		logger("json has %c in terrain. Replacing with floor", ch);
		ch = CHAR_FLOOR;
	}
	t_terrain *terrain = my_calloc(1, sizeof(*terrain));
	terrain->ch = ch;
	terrain->color = color_id((t_color){1,1,1});
	switch (ch)
	{
		case CHAR_WALL:
			terrain->name = "wall";
			terrain->color = color_id((t_color){2,2,2});
			break;
		case CHAR_DOOR:
			terrain->name = "door";
			terrain->color = COLOR_METAL_DOOR;
			break;
		case CHAR_FLOOR:
			if (rand() % 101 <= area->moisture)
			{
				terrain->name = "wet floor";
				terrain->ch = CHAR_WET;
			}
			else
				terrain->name = "floor";
			break;
		case CHAR_LOOT:
			terrain->name = "loot";
			break;
		default:
			logger("Unknown terrain char %c", ch);
			end_ncurses(1);
			break;
	}
	return terrain;
}
