#include "../headers/area.h"
#include "../headers/action.h"
#include "../headers/globals.h"
#include "../headers/utils.h"
#include <unistd.h>

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
