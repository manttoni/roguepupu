#include "../headers/area.h"
#include "../headers/action.h"
#include "../headers/globals.h"
#include "../headers/utils.h"
#include "../headers/windows.h"
#include <unistd.h>

void flee(t_area *area, t_cell *cell)
{
	t_creature *creature = cell->creature;
	const int dirs[8] = {UPLEFT, UP, UPRIGHT, LEFT, RIGHT, DOWNLEFT, DOWN, DOWNRIGHT};
	t_cell *player_cell = get_player_cell(area);
	t_cell *best_flee = cell;
	for (int i = 0; i < 8; ++i)
	{
		t_cell *n = neighbor(dirs[i], area, cell);
		if (n == NULL || is_blocked(n))
			continue;
		if (mandis(area, n, player_cell) >= mandis(area, best_flee, player_cell))
			best_flee = n;
	}
	move_creature(best_flee, cell);
	print_log("%s flees for its life", creature->name);
}

void pursue(t_area *area, t_cell *cell)
{
	const int dirs[8] = {UPLEFT, UP, UPRIGHT, LEFT, RIGHT, DOWNLEFT, DOWN, DOWNRIGHT};
	t_cell *player_cell = get_player_cell(area);
	t_cell *best_pursue = cell;
	t_cell *second_best = cell;
	for (int i = 0; i < 8; ++i)
	{
		t_cell *n = neighbor(dirs[i], area, cell);
		if (n == NULL)
			continue;
		if (mandis(area, n, player_cell) <= mandis(area, best_pursue, player_cell))
		{
			second_best = best_pursue;
			best_pursue = n;
		}
	}
	if (move_creature(best_pursue, cell) == 0)
		move_creature(second_best, cell);
}

void wander(t_area *area, t_cell *cell)
{
	const int dirs[8] = {UPLEFT, UP, UPRIGHT, LEFT, RIGHT, DOWNLEFT, DOWN, DOWNRIGHT};
	while (move_creature(neighbor(dirs[rand() % 8], area, cell), cell) == 0);
}

int enemy_act(t_area *area)
{
	t_node *enemies = get_interactables(area, ENEMY);
	t_cell *player_cell = get_player_cell(area);
	while (enemies != NULL)
	{
		t_cell *cell = (t_cell *) enemies->data;
		t_creature *enemy = cell->creature;

		// idle actions
		if (!is_visible(area, cell, player_cell))
		{
			if (enemy->ai & WANDER)
				wander(area, cell);
		}
		else // combat actions
		{
			if (enemy->ai & FLEE && enemy->health < enemy->max_health / 2)
				flee(area, cell);
			else if (is_neighbor(area, cell, get_player_cell(area)))
				attack(enemy, get_player_cell(area));
			else if (enemy->ai & PURSUE && is_visible(area, cell, get_player_cell(area)))
				pursue(area, cell);
		}
		enemies = enemies->next;
	}
	return 0;
}
