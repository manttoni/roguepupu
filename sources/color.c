#include "../headers/color.h"
#include "../headers/utils.h"

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
	color.red = min(5, color.red);
	color.green = min(5, color.green);
	color.blue = min(5, color.blue);
	*id = color_id(color);
}

