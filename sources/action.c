#include "../headers/action.h"
#include "../headers/cell.h"
#include "../headers/area.h"
#include "../headers/creature.h"
#include "../headers/game.h"
#include "../headers/windows.h"
#include "../headers/draw.h"
#include "../headers/globals.h"

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

void attack(t_creature *attacker, t_cell *defender_cell)
{
	int damage = throw_dice(attacker->weapon.damage);
	print_log("%s attacks %s with a %s, dealing %d damage", attacker->name, defender_cell->creature->name, attacker->weapon.name, damage);

	t_creature *defender = defender_cell->creature;
	if (damage >= defender->health)
	{
		damage = defender->health;
		print_log("%s dies", defender->name);
		if (defender->ch == '@')
		{
			print_log("Game over");
			getch();
			end_ncurses(0);
		}
		defender_cell->terrain->ch = 'C';
		free(defender);
		defender_cell->creature = NULL;
		return;
	}
	defender->health -= damage;
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
int player_act(t_area *area)
{
	int pi = get_player_index(area);
	t_cell *cell = &area->cells[pi];
	t_creature *player = cell->creature;

	switch(player->action)
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
		default:
			return 0;
	}
}

void crazy_goblin_act(t_area *area, t_cell *cell)
{
	int pi = get_player_index(area);
	t_cell *player_cell = &area->cells[pi];
	if (!is_visible(area, cell, player_cell))
		return;
	if (is_neighbor(area, cell, player_cell))
	{
		usleep(100000);
		attack(cell->creature, player_cell);
	}
}

int enemy_act(t_area *area)
{
	t_node *enemies = get_interactables(area, ENEMY);

	while (enemies != NULL)
	{
		t_cell *cell = (t_cell *) enemies->data;
		t_creature *enemy = cell->creature;
		switch (enemy->action)
		{
			case CRAZY_GOBLIN:
				crazy_goblin_act(area, cell);
				break;
			default:
				break;
		}
		enemies = enemies->next;
	}
	return 0;
}





















