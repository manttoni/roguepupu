#ifndef COLOR_H
#define COLOR_H

typedef struct s_color
{
	short red;
	short green;
	short blue;
}	t_color;

short color_sum(short a, short b);
short color_avg(short a, short b);
short modified_color_scalar(short color, int dr, int dg, int db);
short modified_color(short color, float dr, float dg, float db);
int resolve_color_macro(const char *color);
short greyscale(short fg, short bg);
short get_red(short color);
short pair_id(short id1, short id2, int mod);
short color_id(t_color color);
t_color convert(short id);
void change_color(short *id, int dred, int dgreen, int dblue);

#endif
