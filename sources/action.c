#include "../headers/action.h"
#include "../headers/cell.h"
#include "../headers/area.h"
#include "../headers/creature.h"
#include "../headers/game.h"
#include "../headers/windows.h"
#include "../headers/draw.h"

#define INTERACTABLE "O0"
#define VISIBLE 1
#define NEIGHBOR 2
#define ESCAPE 27

void open_door(t_terrain *door)
{
	print_log("Door opened");
	door->ch = '.';
}

void unlock_door(t_terrain *door)
{
	print_log("Door unlocked");
	door->ch = 'O';
}

t_node *select_next(t_node *list, t_node *selected)
{
	if (list == NULL)
		return NULL;
	if (selected == NULL)
	{
		((t_cell*)(list->data))->highlight |= SELECTED;
		return list;
	}
	((t_cell*)(selected->data))->highlight &= ~SELECTED;
	if (selected->next == NULL)
		selected = list;
	else
		selected = selected->next;
	((t_cell*)(selected->data))->highlight |= SELECTED;
	return selected;
}

void scan(t_area *area, int flags)
{
	t_node *interactables = get_interactables(area, flags);
	t_node *ptr = interactables;
	if (interactables == NULL)
		print_log("Cant see anything interactable");
	while (interactables != NULL)
	{
		((t_cell*)(interactables->data))->highlight |= REVERSE;
		interactables = interactables->next;
	}
	interactables = ptr;
	t_node *selected = select_next(interactables, NULL);
	while (selected != NULL)
	{
		draw_area(area);
		int input = getch();
		if (input == ESCAPE)
			break;
		if (input == KEY_ENTER || input == '\n')
		{
			if (flags & VISIBLE)
				print_log(cell_string(selected->data));
			else if (flags & NEIGHBOR)
			{
				t_terrain *t = ((t_cell*)(selected->data))->terrain;
				if (t->ch == 'O')
					open_door(t);
				else if (t->ch == '0')
					unlock_door(t);
			}
			break;
		}
		selected = select_next(interactables, selected);
	}
	while (ptr != NULL)
	{
		((t_cell*)(ptr->data))->highlight = 0;
		ptr = ptr->next;
	}
	draw_area(area);
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
		case INTERACT:
			scan(area, NEIGHBOR);
			break;
		case EXAMINE:
			scan(area, VISIBLE);
			break;
		default:
			break;
	}
}

