#include "color.h"
#include "utils.h"
#include "globals.h"
#include <ncurses.h>

short pair_id(short fg, short bg)
{
	static int pairs[256];
	static int next_free = NEXT_FREE;
	fg = min(255, fg);
	bg = min(255, bg);
	short pair_id = 256 * fg + bg;

	if (pair_id >= COLOR_PAIRS)
		return 0;

	int i = 0;
	for (; i < next_free; ++i)
		if (pairs[i] == pair_id)
			break;
	if (i == next_free) // was not initialized yet
	{
		init_pair(i, fg, bg);
		pairs[i] = pair_id;
		next_free++;
	}
	return i;
}

short get_red(short color)
{
	return convert(color).red;
}

short color_id(t_color color)
{
	return 16 + 36 * color.red + 6 * color.green + color.blue;
}

t_color convert(short id)
{
	// Given a color_id in the range 16..231:
	id = id - 16;
	int red   = id / 36;
	int green = (id / 6) % 6;
	int blue  = id % 6;
	return (t_color){red, green, blue};
}

void change_color(short *id, int dred, int dgreen, int dblue)
{
	t_color color = convert(*id);
	color.red += dred;
	color.green += dgreen;
	color.blue += dblue;
	color.red = max(0, color.red);
	color.green = max(0, color.green);
	color.blue = max(0, color.blue);
	color.red = min(5, color.red);
	color.green = min(5, color.green);
	color.blue = min(5, color.blue);
	*id = color_id(color);
}

