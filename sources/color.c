#include "color.h"
#include "utils.h"
#include "globals.h"
#include "interface.h"
#include <ncurses.h>

short modified_color(short id, float dr, float dg, float db)
{
	t_color color = convert(id);
	color.red *= dr;
	color.green *= dg;
	color.blue *= db;
	return color_id(color);
}

// returns a new id, doesnt overwrite anything
short grey_color(short id)
{
	return modified_color(id, 0.299, 0.587, 0.114);
}

// returns id
short dark_color(short id)
{
	return modified_color(id, DARK_FACTOR, DARK_FACTOR, DARK_FACTOR);
}

short pair_id(short fg, short bg, int mod)
{
	static int pairs[256];
	static int next_free = NEXT_FREE;

	/* makes the colors not so bright (even the lowest values are bright)
	 * it has to modify the [0,1000] values inside ncurses
	if (mod & COLOR_DARKER)
	{
		fg = dark_color(fg);
		bg = dark_color(bg);
	}
	*/
	// greyscale when target cell is outside darkvision range
	if (mod & COLOR_GREYSCALE)
	{
		fg = grey_color(fg);
		bg = grey_color(fg);
	}

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
		print_log("Colors initialized: %d", i);
		pairs[i] = pair_id;
		next_free++;
		if (next_free > 255)
			next_free = NEXT_FREE; // start resetting colors except #defined
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

