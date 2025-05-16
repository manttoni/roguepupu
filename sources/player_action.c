#include "../headers/terrain.h"
#include "../headers/utils.h"
#include "../headers/cell.h"
#include "../headers/globals.h"
#include "../headers/draw.h"
#include "../headers/interface.h"
#include "../headers/action.h"
#include "../headers/status.h"

void open_door(t_cell *door_cell)
{
	t_terrain *door = door_cell->terrain;
	if (door_cell->mech->lock > 0)
	{
		print_log("Door is locked");
		return;
	}
	print_log("Door opened");
	free(door);
	door_cell->terrain = new_terrain('.', 0);
	if (door_cell->mech->trap > 0)
		print_log("Trap sprung!");
}

void unlock_door(t_cell *door_cell)
{
	print_log("Door unlocked");
	door_cell->mech->lock = 0;
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

	if (closed_cell->terrain->ch == 'D')
		open_door(closed_cell);
	return 1; // 1 AP for now
}

static int player_unlock(t_area *area)
{
	t_cell *locked_cell = scan(area, PLAYER_UNLOCK);
	if (locked_cell == NULL)
		return 0;

	if (locked_cell->terrain->ch == 'D')
		unlock_door(locked_cell);
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

static int player_pick_up(t_area *area)
{
	t_cell *item_cell = scan(area, NEIGHBOR | ITEM);
	if (item_cell == NULL)
		return 0;

	pick_up(get_player(area), item_cell);
	item_cell->item = NULL;
	return 0;
}

/* Return some int, will probably be AP cost of action or something later */
int player_act(t_area *area, e_action action)
{
	int pi = get_player_index(area);
	t_cell *cell = &area->cells[pi];
	t_creature *player = cell->creature;

	if (apply_status_effects(cell) == STUN)
		return 0;

	switch(action)
	{
		case MOVE_UPLEFT:
			return move_creature(neighbor(UPLEFT, area, cell), cell);
		case MOVE_UPRIGHT:
			return move_creature(neighbor(UPRIGHT, area, cell), cell);
		case MOVE_DOWNLEFT:
			return move_creature(neighbor(DOWNLEFT, area, cell), cell);
		case MOVE_DOWNRIGHT:
			return move_creature(neighbor(DOWNRIGHT, area, cell), cell);
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
		case PICK_UP:
			return player_pick_up(area);
		case PASS:
			print_log("%s does nothing", player->name);
			return 0;
		default:
			return 0;
	}
}

e_action get_player_action(int input)
{
	logger("Key pressed: %c", input);
	switch(input)
	{
		case '7':
			return MOVE_UPLEFT;
		case '9':
			return MOVE_UPRIGHT;
		case '1':
			return MOVE_DOWNLEFT;
		case '3':
			return MOVE_DOWNRIGHT;
		case '8':
		case KEY_UP:
			return MOVE_UP;
		case '2':
		case KEY_DOWN:
			return MOVE_DOWN;
		case '4':
		case KEY_LEFT:
			return MOVE_LEFT;
		case '6':
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
		case 'p':
			return PICK_UP;
		case ' ':
			return PASS;
		default:
			return NONE;
	}
}
