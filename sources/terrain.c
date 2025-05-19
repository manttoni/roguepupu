#include "../headers/terrain.h"
#include "../headers/utils.h"
#include "../headers/globals.h"
#include <ncurses.h>

char *terrain_string(t_terrain *terrain)
{
	int len = 15 + strlen(terrain->name);
	char *buf = my_calloc(len + 1);
	snprintf(buf, len + 1, "{%d}%s{reset}", terrain->color, terrain->name);
	return buf;
}

t_terrain *new_terrain(char ch, int area_level)
{
	(void)area_level;
	if (strchr(TERRAIN_CHARS, ch) == NULL)
		return NULL;
	t_terrain *terrain = my_calloc(sizeof(t_terrain));
	terrain->ch = ch;
	terrain->color = COLOR_WHITE;
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
