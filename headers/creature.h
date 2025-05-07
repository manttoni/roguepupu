#ifndef CREATURE_H
#define CREATURE_H

typedef struct s_area t_area;
typedef struct s_cell t_cell;

typedef enum
{
	PASS,
	MOVE_UP,
	MOVE_DOWN,
	MOVE_LEFT,
	MOVE_RIGHT,
	NONE
}	e_action;

typedef struct s_creature
{
	unsigned char	ch; // what does it look like?
	short			color; // fg color for color_pair
	e_action		action; // what is this creature going to do next
}	t_creature;

t_creature *new_creature(char ch);
void move_creature(t_cell *dest, t_cell *src);
void act(t_area *area, t_cell *cell, t_creature *creature);
e_action get_player_action(int input);

#endif
