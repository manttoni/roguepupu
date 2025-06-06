#include "action.h"
#include "memory.h"
#include "area.h"
#include "globals.h"
#include "utils.h"
#include "windows.h"
#include "creature.h"
#include "status.h"
#include "cell.h"
#include "interface.h"
#include <unistd.h>

void flee(t_cell *cell)
{
	t_creature *npc = cell->creature;
	if (npc->behavior != AI_FLEE)
		print_log("%C flees for their life", npc);
	npc->behavior = AI_FLEE;

	t_cell *player_cell = get_player_cell();
	e_direction best_flee = 0;
	for (int i = 0; i < 8; ++i)
	{
		t_cell *n = neighbor(g_dirs[i], cell);
		if (n == NULL || is_blocked(n))
			continue;
		if (distance(n, player_cell) >= distance(neighbor(best_flee, cell), player_cell))
			best_flee = i;
	}
	act_move(best_flee, cell);
}

void pursue(t_cell *cell)
{
	t_creature *npc = cell->creature;
	if (npc->behavior != AI_PURSUE)
		print_log("%C charges towards %C", npc, get_player()); // <-- later they can attack f.e. the closest visible or smth
	npc->behavior = AI_PURSUE;

	t_cell *player_cell = get_player_cell();
	e_direction best_pursue = 0;
	e_direction second_best = 0;
	for (int i = 0; i < 8; ++i)
	{
		t_cell *n = neighbor(g_dirs[i], cell);
		if (n == NULL)
			continue;
		if (distance(n, player_cell) <= distance(neighbor(best_pursue, cell), player_cell))
		{
			second_best = best_pursue;
			best_pursue = i;
		}
	}
	if (act_move(best_pursue, cell) == 0)
		if (act_move(second_best, cell) == 0)
			act_move(g_dirs[rand() % 8], cell);
}

void wander(t_cell *cell)
{
	while (act_move(g_dirs[rand() % 8], cell) == 0);
}

int npc_act(void)
{
	t_node *enemies = get_entities(NULL, TARGET_CREATURE);
	t_node *ptr = enemies;
	t_cell *player_cell = get_player_cell();
	while (enemies != NULL)
	{
		t_cell *cell = (t_cell *) enemies->data;
		t_creature *enemy = cell->creature;

		if (enemy->health <= 0)
		{
			short color = cell->creature->color;
			cell->terrain = spawn_terrain(CHAR_LOOT, 0);
			cell->terrain->color = color;
			cell->terrain->loot = cell->creature->inventory;
			my_free(cell->creature);
			cell->creature = NULL;
		}
		else if (visibility(cell, player_cell) == VISION_NONE)
		{
			if (enemy->ai & AI_WANDER)
				wander(cell);
		}
		else if (visibility(cell, player_cell) == VISION_FULL)
		{
			if (enemy->ai & AI_FLEE && enemy->health < enemy->max_health / 4)
				flee(cell);
			else if (is_neighbor(cell, get_player_cell()))
				act_attack(enemy, get_player());
			else if (enemy->ai & AI_PURSUE && visibility(cell, get_player_cell()) == VISION_FULL)
				pursue(cell);
		}

		enemies = enemies->next;
	}
	list_clear(&ptr);
	return 0;
}
