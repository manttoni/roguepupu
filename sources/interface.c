#include "../headers/area.h"
#include "../headers/windows.h"
#include "../headers/creature.h"
#include "../headers/cell.h"
#include "../headers/draw.h"
#include "../headers/globals.h"
#include "../headers/utils.h"
#include <ncurses.h>

void print_creature_status(t_creature *creature)
{
	wprintw(stat_win, "  %s %c\n", creature->name, creature->ch);
	wprintw(stat_win, "  Health: %d/%d\n", creature->health, creature->max_health);
	wprintw(stat_win, "  Bleeding: %d\n", creature->bleeding);
	wprintw(stat_win, "  Stun level: %d\n", creature->stunned);
	int x, y;
	getmaxyx(stat_win, y, x);
	(void) y;
	for (int i = 0; i < x; ++i)
		wprintw(stat_win, "-");
}

void update_stat_win(t_area *area)
{
	werase(stat_win);
	wmove(stat_win, 1, 0);
	t_node *enemies = get_interactables(area, ENEMY | VISIBLE);

	logger("update_stat_win got interactables");
	print_creature_status(get_player(area));

	while (enemies != NULL)
	{
		t_cell *enemy_cell = (t_cell *) enemies->data;
		print_creature_status(enemy_cell->creature);
		enemies = enemies->next;
	}

	refresh_window(stat_win);
}
