#include "../headers/entity.h"
#include "../headers/utils.h"
#include "../headers/cell.h"

t_entity *new_entity(char ch)
{
	t_entity *entity = my_malloc(sizeof(t_entity));
	entity->ch = ch;
	entity->color = COLOR_WHITE;
	entity->action = NONE;
	logger("New entity created: %c", ch);
	return entity;
}

void attack(t_cell *dst, t_cell *src)
{
	(void) dst;
	(void)src;
	return;
}

void move_entity(t_cell *dst, t_cell *src)
{
	if (dst == NULL)
		return;
	if (is_blocking(dst))
		return;
	if (dst->entity != NULL)
	{
		attack(dst, src);
		return;
	}
	dst->entity = src->entity;
	src->entity = NULL;
}

void act(t_area *area, t_cell *cell, t_entity *entity)
{
	switch(entity->action)
	{
		case MOVE_UP:
			move_entity(neighbor(UP, area, cell), cell);
			break;
		case MOVE_DOWN:
			move_entity(neighbor(DOWN, area, cell), cell);
			break;
		case MOVE_LEFT:
			move_entity(neighbor(LEFT, area, cell), cell);
			break;
		case MOVE_RIGHT:
			move_entity(neighbor(RIGHT, area, cell), cell);
			break;
		default:
			break;
	}
}
