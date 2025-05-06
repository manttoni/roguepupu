#ifndef ENTITY_H
#define ENTITY_H

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

typedef struct s_entity
{
	unsigned char	ch; // what does it look like?
	short			color; // fg color for color_pair
	e_action		action; // what is this entity going to do next
}	t_entity;

t_entity *new_entity(char ch);
void move_entity(t_cell *dest, t_cell *src);
void act(t_area *area, t_cell *cell, t_entity *entity);

#endif
