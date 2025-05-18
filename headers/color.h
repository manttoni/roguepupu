#ifndef COLOR_H
#define COLOR_H

typedef struct s_color
{
	short red;
	short green;
	short blue;
}	t_color;

short get_red(short color);
short pair_id(short id1, short id2);
short color_id(t_color color);
t_color convert(short id);
void change_color(short *id, int dred, int dgreen, int dblue);

#endif
