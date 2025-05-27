#include "action.h"
#include "memory.h"
#include "terrain.h"
#include "utils.h"
#include "cell.h"
#include "globals.h"
#include "draw.h"
#include "interface.h"
#include "windows.h"
#include "weapon.h"
#include "equipment.h"

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
t_cell *select_target(int flags)
{
	// get list
	t_node *list = get_entities(get_player(), flags);
	if (list == NULL)
	{
		print_log("Nothing to see here");
		return NULL;
	}
	list_sort(list, &compare_distance_player);
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

static void interact_harvest_fungus(t_cell *interactable)
{
	t_fungus *fungus = interactable->fungus;
	print_log("%C harvests %F", get_player(), fungus);
	add_item(get_player(), new_item("reagent", fungus->name));
	free(fungus);
	interactable->fungus = NULL;
}

static void interact_pick_up(t_cell *interactable)
{
	t_item *item = interactable->item;
	print_log("%C picks up %I", get_player(), item);
	add_item(get_player(), item);
	interactable->item = NULL;
}

static void interact_open_door(t_cell *interactable)
{
	t_terrain *door = interactable->terrain;
	if (is_locked(interactable))
	{
		print_log("%T is locked", door);
		return;
	}
	print_log("%C opens %T", get_player(), door);
	interactable->terrain->ch = CHAR_FLOOR;
}

static void interact_open_container(t_cell *interactable)
{
	t_terrain *container = interactable->terrain;
	if (is_locked(interactable))
	{
		print_log("%T is locked", container);
		return;
	}
	print_log("%C opens %T", get_player(), container);
	open_inventory(&container->loot, INVENTORY_LOOT);
	if (container->loot != NULL)
		return;
	interactable->terrain->ch = CHAR_FLOOR;
}

/*static void interact_unlock(t_cell *interactable)
{
	t_mech *lock = interactable->mech;
	print_log("%C unlocks %T", get_player(), interactable->terrain);
	free(lock);
	interactable->mech = NULL;
}*/

int pc_interact(void)
{
	t_cell *interactable = select_target(RANGE_MELEE | TARGET_INTERACTABLE);
	if (interactable == NULL)
		return 0;

	char ch = cell_char(interactable);
	switch (top_entity(interactable))
	{
		case ENTITY_CREATURE:
			// this part handles npc conversation, trade etc
			// pickpocket?
			break;
		case ENTITY_FUNGUS:
			interact_harvest_fungus(interactable);
			break;
		case ENTITY_ITEM:
			interact_pick_up(interactable);
			break;
		case ENTITY_TERRAIN:
			if (ch == CHAR_DOOR)
				interact_open_door(interactable);
			else if (strchr(TERRAIN_CONTAINER, ch) != NULL)
				interact_open_container(interactable);
			break;
		/*case ENTITY_MECH:
			if (ch == 'L')
				interact_unlock(interactable);
			else if (is_trap(ch))
				interact_disarm_trap(interactable);*/
			break;
		default:
			break;
	}
	return 1;
}

// lets the user control ui if they want. if non ui input is given, it is returned
static int ui_controls(void)
{
	while (1)
	{
		int input = getch();
		switch (input)
		{
			case ESCAPE:
				print_log("{red}Press ESC again to quit or C to cancel.{reset}");
				while (1)
				{
					input = getch();
					if (input == ESCAPE)
						return QUIT_GAME;
					if (input == 'c')
					{
						print_log("{green}Quit canceled{reset}");
						input = getch();
						break;
					}
				}
				break;
			default:
				return input;
		}
	}
}

static e_direction get_move_direction(int input)
{
	switch (input)
	{
		case '8': case KEY_UP: return UP;
		case '2': case KEY_DOWN: return DOWN;
		case '4': case KEY_LEFT: return LEFT;
		case '6': case KEY_RIGHT: return RIGHT;
		case '7': return UPLEFT;
		case '9': return UPRIGHT;
		case '1': return DOWNLEFT;
		case '3': return DOWNRIGHT;
		default: break;
	}
	return DIRECTION_END;
}

int pc_act(void)
{
	t_creature *player = get_player();
	while (1)
	{
		int input = ui_controls();
		if (input == QUIT_GAME)
			return QUIT_GAME;
		if (input == ' ')
			return END_TURN;

		e_direction dir = get_move_direction(input);
		if (dir != DIRECTION_END)
		{
			if (act_move(dir, get_player_cell()) == 0)
				continue;
			break;
		}

		switch(input)
		{
			case 'e':
				select_target(TARGET_VISIBLE | TARGET_INTERACTABLE);
				continue;
			case 'a':
				t_cell *target = select_target(ATTACK_FLAGS(player));
				if (target != NULL)
				{
					act_attack(player, target->creature);
					break;
				}
				continue;
			case 'i':
				if (open_inventory(&get_player()->inventory, INVENTORY_PLAYER) == 1)
					break;
				continue;
			case ENTER:
				if (pc_interact() == 1)
					break;
				continue;
			default:
				continue;
		}
		break;
	}
	return 0;
}

