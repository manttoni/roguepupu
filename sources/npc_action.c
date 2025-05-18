#include "action.h"
#include "area.h"
#include "globals.h"
#include "utils.h"
#include "windows.h"
#include "creature.h"
#include "status.h"
#include "cell.h"
#include "interface.h"
#include <unistd.h>

const int dirs[8] = {UPLEFT, UP, UPRIGHT, LEFT, RIGHT, DOWNLEFT, DOWN, DOWNRIGHT};

void flee(t_area *area, t_cell *cell)
{
	t_creature *npc = cell->creature;
	if (npc->behavior != AI_FLEE)
		print_log("%s flees for their life", npc->name);
	npc->behavior = AI_FLEE;

	t_cell *player_cell = get_player_cell(area);
	t_cell *best_flee = cell;
	for (int i = 0; i < 8; ++i)
	{
		t_cell *n = neighbor(dirs[i], area, cell);
		if (n == NULL || is_blocked(n))
			continue;
		if (distance(area, n, player_cell) >= distance(area, best_flee, player_cell))
			best_flee = n;
	}
	act_move(best_flee, cell);
}

void pursue(t_area *area, t_cell *cell)
{
	t_creature *npc = cell->creature;
	if (npc->behavior != AI_PURSUE)
		print_log("%s charges towards %s", npc->name, get_player(area)->name);
	npc->behavior = AI_PURSUE;

	t_cell *player_cell = get_player_cell(area);
	t_cell *best_pursue = cell;
	t_cell *second_best = cell;
	for (int i = 0; i < 8; ++i)
	{
		t_cell *n = neighbor(dirs[i], area, cell);
		if (n == NULL)
			continue;
		if (distance(area, n, player_cell) <= distance(area, best_pursue, player_cell))
		{
			second_best = best_pursue;
			best_pursue = n;
		}
	}
	if (act_move(best_pursue, cell) == 0)
		if (act_move(second_best, cell) == 0)
			act_move(neighbor(dirs[rand() % 8], area, cell), cell);
}

void wander(t_area *area, t_cell *cell)
{
	while (act_move(neighbor(dirs[rand() % 8], area, cell), cell) == 0);
}

int npc_act(t_area *area)
{
	t_node *enemies = get_interactables(area, SCAN_ENEMY);
	t_cell *player_cell = get_player_cell(area);
	while (enemies != NULL)
	{
		t_cell *cell = (t_cell *) enemies->data;
		t_creature *enemy = cell->creature;

		if (enemy->health <= 0)
		{
			short color = cell->terrain->color;
			free(cell->terrain);
			cell->terrain = new_terrain('R', 0);
			cell->terrain->color = color;
			free(cell->creature);
			cell->creature = NULL;
		}
		else if (!is_visible(area, cell, player_cell))
		{
			if (enemy->ai & AI_WANDER)
				wander(area, cell);
		}
		else
		{
			if (enemy->ai & AI_FLEE && enemy->health < enemy->max_health / 4)
				flee(area, cell);
			else if (is_neighbor(area, cell, get_player_cell(area)))
				act_attack(cell, get_player_cell(area));
			else if (enemy->ai & AI_PURSUE && is_visible(area, cell, get_player_cell(area)))
				pursue(area, cell);
		}

		enemies = enemies->next;
	}
	return 0;
}
