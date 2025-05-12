#include "../headers/action.h"
#include "../headers/cell.h"
#include "../headers/area.h"
#include "../headers/creature.h"
#include "../headers/game.h"
#include "../headers/windows.h"

#define INTERACTABLE "O0"

int select_next(t_cell **cells, int len, int selected)
{
	if (selected >= 0)
		cells[selected]->highlight &= ~SELECTED;
	for (int i = 0; i < len; ++i)
	{
		selected++;
		if (selected >= len)
			selected = 0;
		if (cells[selected]->highlight & REVERSE)
		{
			cells[selected]->highlight |= SELECTED;
			return selected;
		}
	}
	return -1;
}

void interact(t_area *area)
{
	int pi = get_player_index(area);
	t_cell *player_cell = &area->cells[pi];
	t_cell **neighbors = get_neighbors(area, player_cell);

	for (int i = 0; i < 4; ++i)
		if (strchr(INTERACTABLE, neighbors[i]->terrain->ch) != NULL)
			neighbors[i]->highlight |= REVERSE;

	int selected = select_next(neighbors, 4, -1);
	if (selected < 0)
		return;

	print_log("Interactable objects highlighted");
	print_log("Select with arrow keys");
	print_log("Esc to cancel");

	while (1)
	{
		draw(area);
		int input = getch();
		if (input == 27)
			break;
		if (input == KEY_ENTER || input == '\n')
		{
			unlock_door(neighbors[selected]->terrain);
			break;
		}
		selected = select_next(neighbors, 4, selected);
		switch(input)
		{
			case KEY_UP:
				break;
			case KEY_RIGHT:
				break;
			case KEY_DOWN:
				break;
			case KEY_LEFT:
				break;
			default:
				continue;
		}
	}
	for (int i = 0; i < 4; ++i)
		neighbors[i]->highlight = 0;
	draw(area);
}

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
			interact(area);
			break;
		default:
			break;
	}
}
