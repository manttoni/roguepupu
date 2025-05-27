#include "color.h"
#include "utils.h"
#include "globals.h"
#include "interface.h"
#include <ncurses.h>

short color_max(short id)
{
	t_color color = convert(id);
	while (color.red > 1 || color.green > 1 || color.blue > 1)
	{
		id = modified_color_scalar(id, -1, -1, -1);
		color = convert(id);
	}
	return color_id(color);
}

short color_sum(short a, short b)
{
	t_color ac = convert(a);
	t_color bc = convert(b);

	t_color c;
	c.red = ac.red + bc.red;
	c.green = ac.green + bc.green;
	c.blue = ac.blue + bc.blue;
	return color_id(c);
}

short color_avg(short a, short b)
{
	short sum = color_sum(a, b);
	t_color c = convert(sum);
	c.red /= 2;
	c.green /= 2;
	c.blue /= 2;
	return color_id(c);
}

int resolve_color_macro(const char* color)
{
	if (strcmp(color, "brown") == 0)
		return MY_COLOR_BROWN;
	else if (strcmp(color, "pale blue") == 0)
		return MY_COLOR_PALE_BLUE;
	else if (strcmp(color, "soft blue") == 0)
		return MY_COLOR_SOFT_BLUE;
	return MY_COLOR_BLACK;
}

short modified_color_scalar(short id, int dr, int dg, int db)
{
	t_color color = convert(id);
	int red = color.red + dr;
	int green = color.green + dg;
	int blue = color.blue + db;

	if (red < 0)
		red = 0;
	if (green < 0)
		green = 0;
	if (blue < 0)
		blue = 0;

	if (red > 5)
		red = 5;
	if (green > 5)
		green = 5;
	if (blue > 5)
		blue = 5;

	return color_id((t_color){red, green, blue});
}

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

