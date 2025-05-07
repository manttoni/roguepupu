#include "../headers/creature.h"
#include "../headers/utils.h"
#include "../headers/cell.h"

t_creature *new_creature(char ch)
{
	t_creature *creature = my_malloc(sizeof(t_creature));
	creature->ch = ch;
	creature->color = COLOR_WHITE;
	creature->action = NONE;
	logger("New creature created: %c", ch);
	return creature;
}

void attack(t_cell *dst, t_cell *src)
{
	(void) dst;
	(void) src;
	return;
}

void move_creature(t_cell *dst, t_cell *src)
{
	if (dst == NULL)
		return;
	if (is_blocking(dst))
		return;
	if (dst->creature != NULL)
		return;
	dst->creature = src->creature;
	src->creature = NULL;
}

void act(t_area *area, t_cell *cell, t_creature *creature)
{
	switch(creature->action)
	{
		case MOVE_UP:
			move_creature(neighbor(UP, area, cell), cell);
			break;
		case MOVE_DOWN:
			move_creature(neighbor(DOWN, area, cell), cell);
			break;
		case MOVE_LEFT:
			move_creature(neighbor(LEFT, area, cell), cell);
			break;
		case MOVE_RIGHT:
			move_creature(neighbor(RIGHT, area, cell), cell);
			break;
		default:
			break;
	}
}

e_action get_player_action(int input)
{
	switch(input)
	{
		case KEY_UP:
			return MOVE_UP;
		case KEY_DOWN:
			return MOVE_DOWN;
		case KEY_LEFT:
			return MOVE_LEFT;
		case KEY_RIGHT:
			return MOVE_RIGHT;
		default:
			return NONE;
	}
}
