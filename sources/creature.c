#include "../headers/creature.h"
#include "../headers/utils.h"
#include "../headers/cell.h"
#include "../headers/windows.h"

t_creature *new_creature(char ch)
{
	t_creature *creature = my_calloc(sizeof(t_creature));
	creature->ch = ch;
	creature->color = COLOR_WHITE;
	creature->action = NONE;
	logger("New creature created: %c", ch);
	return creature;
}

void move_creature(t_cell *dst, t_cell *src)
{
	if (dst == NULL)
		return;
	if (dst->terrain->ch == '#')
		return;
	if (dst->terrain->ch == '0')
	{
		print_log("Door is locked");
		return;
	}
	if (dst->terrain->ch == 'O')
	{
		print_log("Door is closed");
		return;
	}
	if (dst->creature != NULL)
		return;
	dst->creature = src->creature;
	src->creature = NULL;
}

