#include "../headers/area.h"
#include "../headers/action.h"
#include "../headers/globals.h"
#include "../headers/utils.h"
#include "../headers/windows.h"
#include <unistd.h>

static e_direction get_player_direction(t_area *area, t_cell *cell)
{
	t_cell *player_cell = get_player_cell(area);
	int diff = player_cell - cell;
	int dx = diff % area->width;
	int dy = diff / area->width;

	// check if should move horizontally
	if (abs(dx) > abs(dy))
	{
		if (dx < 0)
			return LEFT;
		return RIGHT;
	}

	if (dy < 0)
		return UP;
	return DOWN;
}

void flee(t_area *area, t_cell *cell)
{
	t_creature *creature = cell->creature;
	e_direction pldir = get_player_direction(area, cell);
	if (move_creature(neighbor(-pldir, area, cell), cell) == 0
		&& is_neighbor(area, cell, get_player_cell(area)))
		attack(cell->creature, get_player_cell(area));
	print_log("%s flees for their life", creature->name);
}

void wander(t_area *area, t_cell *cell)
{
	const int dirs[8] = {UPLEFT, UP, UPRIGHT, LEFT, RIGHT, DOWNLEFT, DOWN, DOWNRIGHT};
	while (move_creature(neighbor(dirs[rand() % 8], area, cell), cell) == 0);
}

void pursue(t_area *area, t_cell *cell)
{
	e_direction pldir = get_player_direction(area, cell);
	move_creature(neighbor(pldir, area, cell), cell);
}

int enemy_act(t_area *area)
{
	t_node *enemies = get_interactables(area, ENEMY);

	while (enemies != NULL)
	{
		t_cell *cell = (t_cell *) enemies->data;
		t_creature *enemy = cell->creature;

		if (enemy->action & FLEE && enemy->health < enemy->max_health / 2)
			flee(area, cell);
		else if (is_neighbor(area, cell, get_player_cell(area)))
			attack(enemy, get_player_cell(area));
		else if (enemy->action & PURSUE && is_visible(area, cell, get_player_cell(area)))
			pursue(area, cell);
		else if (enemy->action & WANDER)
			wander(area, cell);

		enemies = enemies->next;
	}
	return 0;
}
