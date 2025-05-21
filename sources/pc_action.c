#include "action.h"
#include "memory.h"
#include "terrain.h"
#include "utils.h"
#include "cell.h"
#include "globals.h"
#include "draw.h"
#include "interface.h"
#include "windows.h"

void pc_disarm_trap(t_creature *pc, t_cell *trapped_cell)
{
	print_log("%C disarms a trap on %T", pc, trapped_cell->terrain);
	trapped_cell->mech->trap = 0;
}

static void clear_highlights(t_node *list)
{
	while (list)
	{
		t_cell *cell = (t_cell *) list->data;
		cell->highlight = 0;
		list = list->next;
	}
}

/* Select a cell from list of cells */
t_cell *select_cell(t_node *list)
{
	t_node *selected_node = list;
	while (1)
	{
		t_cell *selected = (t_cell *) selected_node->data;
		selected->highlight |= A_BLINK;
		draw_area();
		print_selected(selected);

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
				selected->highlight &= ~A_BLINK;
				selected_node = selected_node->next;
				if (selected_node == NULL)
					selected_node = list;
				break;
		}
	}
	return NULL;
}

/* Scan surroundings, highlight cells, return a selected cell */
t_cell *scan(int flags)
{
	// get list
	t_node *list = get_interactables(flags);
	if (list == NULL)
	{
		print_log("Nothing to see here");
		return NULL;
	}
	t_node *ptr = list;

	// highlight interactables with A_REVERSE
	while (ptr != NULL)
	{
		t_cell *cell = (t_cell *) ptr->data;
		cell->highlight |= A_REVERSE;
		ptr = ptr->next;
	}
	t_cell *selected = select_cell(list);
	list_clear(&list);
	return selected;
}

/* Scan surroundings within vision */
int pc_examine(void)
{
	t_cell *examine_cell = scan(PLAYER_EXAMINE);
	if (examine_cell == NULL) // esc pressed or nothing to examine
		return 0;

	// enter pressed
	return 0;
}

static void pc_open_container(t_terrain *container)
{
	open_inventory(&container->loot, INVENTORY_LOOT);
	if (list_len(container->loot) == 0)
		container->ch = '.';
}

static void pc_open_door(t_cell *door_cell)
{
	print_log("%C opens %T", get_player(), door_cell->terrain);
	free_terrain(door_cell->terrain);
	door_cell->terrain = new_terrain('.', 0);
}

int pc_open(void)
{
	t_cell *closed_cell = scan(PLAYER_OPEN);
	if (closed_cell == NULL)
		return 0;

	t_terrain *closed = closed_cell->terrain;
	if (closed_cell->mech != NULL && closed_cell->mech->lock > 0)
	{
		print_log("%T is locked", closed);
		return 0;
	}
	if (closed_cell->mech != NULL && closed_cell->mech->trap > 0)
	{
		print_log("Trap!");
		return 0;
	}
	// Remains or Chest
	if (strchr(TERRAIN_CONTAINER, closed->ch))
		pc_open_container(closed);
	// Door
	else if (strchr(TERRAIN_DOOR, closed->ch))
		pc_open_door(closed_cell);


	return 1; // 1 AP for now
}

int pc_unlock(void)
{
	t_cell *locked_cell = scan(PLAYER_UNLOCK);
	if (locked_cell == NULL)
		return 0;

	print_log("%C unlocks %T", get_player(), locked_cell->terrain);
	locked_cell->mech->lock = 0; // for now just unlock everything without skill check

	return 0;
}

int pc_attack(void)
{
	int attack_flag = MELEE_ATTACK;
	if (has_ranged_weapon(get_player()))
		attack_flag = RANGED_ATTACK;
	t_cell *defender_cell = scan(attack_flag);
	if (defender_cell == NULL)
		return 0;

	act_attack(get_player(), defender_cell->creature);
	return 0;
}

int pc_pick_up(void)
{
	t_cell *item_cell = scan(SCAN_NEIGHBOR | SCAN_ITEM);
	if (item_cell == NULL)
		return 0;

	add_item(get_player(), item_cell->item);
	print_log("%C picked up %I", get_player(), item_cell->item);
	item_cell->item = NULL;

	return 0;
}
e_action get_player_action(int input)
{

	switch(input)
	{
		case '7':
			return ACTION_MOVE_UPLEFT;
		case '9':
			return ACTION_MOVE_UPRIGHT;
		case '1':
			return ACTION_MOVE_DOWNLEFT;
		case '3':
			return ACTION_MOVE_DOWNRIGHT;
		case '8':
		case KEY_UP:
			return ACTION_MOVE_UP;
		case '2':
		case KEY_DOWN:
			return ACTION_MOVE_DOWN;
		case '4':
		case KEY_LEFT:
			return ACTION_MOVE_LEFT;
		case '6':
		case KEY_RIGHT:
			return ACTION_MOVE_RIGHT;
		case 'e':
			return ACTION_EXAMINE;
		case 'o':
			return ACTION_OPEN;
		case 'u':
			return ACTION_UNLOCK;
		case 'a':
			return ACTION_ATTACK;
		case 'p':
			return ACTION_PICK_UP;
		case ' ':
			return ACTION_PASS;
		case 'i':
			return ACTION_INVENTORY;
		default:
			return ACTION_NONE;
	}
}
/* Return some int, will probably be AP cost of action or something later */
int pc_act(void)
{
	int pi = get_player_index();
	t_cell *cell = &g_area->cells[pi];
	t_creature *player = cell->creature;

	int input = getch();
	if (input == ESCAPE)
	{
		print_log("{red}Press ESC again to quit or C to cancel.{reset}");
		while (1)
		{
			input = getch();
			if (input == ESCAPE) // later this could open a menu to save game etc
				return QUIT_GAME;
			if (input == 'c')
			{
				print_log("{green}Quit canceled{reset}");
				input = getch();
				break;
			}
		}
	}
	e_action action = get_player_action(input);

	switch(action)
	{

		case ACTION_MOVE_UPLEFT:
			return act_move(neighbor(UPLEFT, cell), cell);
		case ACTION_MOVE_UPRIGHT:
			return act_move(neighbor(UPRIGHT, cell), cell);
		case ACTION_MOVE_DOWNLEFT:
			return act_move(neighbor(DOWNLEFT, cell), cell);
		case ACTION_MOVE_DOWNRIGHT:
			return act_move(neighbor(DOWNRIGHT, cell), cell);
		case ACTION_MOVE_UP:
			return act_move(neighbor(UP, cell), cell);
		case ACTION_MOVE_DOWN:
			return act_move(neighbor(DOWN, cell), cell);
		case ACTION_MOVE_LEFT:
			return act_move(neighbor(LEFT, cell), cell);
		case ACTION_MOVE_RIGHT:
			return act_move(neighbor(RIGHT, cell), cell);
		case ACTION_EXAMINE:
			return pc_examine();
		case ACTION_OPEN:
			return pc_open();
		case ACTION_UNLOCK:
			return pc_unlock();
		case ACTION_ATTACK:
			return pc_attack();
		case ACTION_PICK_UP:
			return pc_pick_up();
		case ACTION_PASS:
			print_log("%s does nothing", player->name);
			return 0;
		case ACTION_INVENTORY:
			open_inventory(&player->inventory, INVENTORY_PLAYER);
			return 0;
		default:
			return 0;
	}
}


