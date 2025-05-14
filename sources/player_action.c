#include "../headers/terrain.h"
#include "../headers/utils.h"
#include "../headers/cell.h"
#include "../headers/globals.h"
#include "../headers/windows.h"
#include "../headers/draw.h"

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

void clear_highlights(t_node *list)
{
	while (list)
	{
		t_cell *cell = (t_cell *) list->data;
		cell->highlight = 0;
		list = list->next;
	}
}

/* Select a cell from list of cells */
t_cell *select_cell(t_node *list, t_area *area)
{
	t_node *selected_node = list;
	while (1)
	{
		t_cell *selected = (t_cell *) selected_node->data;
		selected->highlight |= SELECTED;
		// draw area, highlights are showing
		draw_area(area);

		// esc quits, enter returns selected, other keys select next
		int input = getch();
		switch (input)
		{
			case ESCAPE:
				clear_highlights(list);
				return NULL;
			case ENTER:
				clear_highlights(list);
				return selected;
			default:
				selected->highlight &= ~SELECTED;
				selected_node = selected_node->next;
				if (selected_node == NULL)
					selected_node = list;
				break;
		}
	}
	return NULL;
}

/* Scan surroundings, highlight cells, return a selected cell */
t_cell *scan(t_area *area, int flags)
{
	// get list
	t_node *list = get_interactables(area, flags);
	if (list == NULL)
	{
		print_log("Nothing to see here");
		return NULL;
	}
	t_node *ptr = list;

	// highlight interactables with REVERSE
	while (ptr != NULL)
	{
		t_cell *cell = (t_cell *) ptr->data;
		cell->highlight |= REVERSE;
		ptr = ptr->next;
	}
	return select_cell(list, area);
}

/* Scan surroundings within vision */
static int player_examine(t_area *area)
{
	t_cell *examine_cell = scan(area, PLAYER_EXAMINE);
	if (examine_cell == NULL) // esc pressed
		return 0;

	// enter pressed
	print_log(cell_string(examine_cell));
	return 0; // 0 AP
}

static int player_open(t_area *area)
{
	t_cell *closed_cell = scan(area, PLAYER_OPEN);
	if (closed_cell == NULL)
		return 0;

	t_terrain *closed = closed_cell->terrain;
	if (closed->ch == 'O')
		open_door(closed);
	return 1; // 1 AP for now
}

static int player_unlock(t_area *area)
{
	t_cell *locked_cell = scan(area, PLAYER_UNLOCK);
	if (locked_cell == NULL)
		return 0;

	t_terrain *locked = locked_cell->terrain;
	if (locked->ch == '0')
		unlock_door(locked);
	return 2;
}

static int player_attack(t_area *area)
{
	t_cell *defender_cell = scan(area, PLAYER_MELEE_ATTACK);
	if (defender_cell == NULL)
		return 0;

	t_creature *player = get_player(area);
	attack(player, defender_cell);
	return 0;
}

/* Return some int, will probably be AP cost of action or something later */
int player_act(t_area *area, e_action action)
{
	int pi = get_player_index(area);
	t_cell *cell = &area->cells[pi];
	t_creature *player = cell->creature;

	switch(action)
	{
		case MOVE_UP:
			return move_creature(neighbor(UP, area, cell), cell);
		case MOVE_DOWN:
			return move_creature(neighbor(DOWN, area, cell), cell);
		case MOVE_LEFT:
			return move_creature(neighbor(LEFT, area, cell), cell);
		case MOVE_RIGHT:
			return move_creature(neighbor(RIGHT, area, cell), cell);
		case EXAMINE:
			return player_examine(area);
		case OPEN:
			return player_open(area);
		case UNLOCK:
			return player_unlock(area);
		case ATTACK:
			return player_attack(area);
		case PASS:
			print_log("%s does nothing", player->name);
			return 0;
		default:
			return 0;
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
		case 'e':
			return EXAMINE;
		case 'o':
			return OPEN;
		case 'u':
			return UNLOCK;
		case 'a':
			return ATTACK;
		case ' ':
			return PASS;
		default:
			return NONE;
	}
}
