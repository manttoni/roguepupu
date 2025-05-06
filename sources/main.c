#include <signal.h>
#include <ncurses.h>
#include <stdlib.h>
#include "../headers/area.h"
#include "../headers/utils.h"

void handle_segfault(int sig)
{
    logger("segfault");
	end_ncurses(sig);
}

e_action get_player_action(int input)
{
	switch(input)
	{
		case KEY_UP:
			return MOVE_UP;
		case KEY_DOWN:
			return MOVE_DOWN;
		case KEY_LEFT:
			return MOVE_LEFT;
		case KEY_RIGHT:
			return MOVE_RIGHT;
		default:
			return NONE;
	}
}

void draw(t_area *area)
{
	// index of the cell where player is
	int player_index = get_player_index(area);

	// get screen size
	int y_max, x_max;
	getmaxyx(stdscr, y_max, x_max);

	// fill the screen with ASCII :D
	for (int y = 0; y < y_max; ++y)
	{
		for (int x = 0; x < x_max; ++x)
		{
			// index of cell being drawn
			int y_draw = y - y_max / 2;
			int x_draw = x - x_max / 2;
			int cell_index = player_index + y_draw * area->width + x_draw;
			if (cell_index < 0 || cell_index >= area->height * area->width)
				continue;
			t_cell cell = area->cells[cell_index];
			char ch = cell.entity == NULL ? cell.terrain->ch : cell.entity->ch;
			mvaddch(y, x, ch);
		}
	}
	return;
}

void do_actions(t_area *area)
{
	for (int i = 0; i < area->width * area->height; ++i)
	{
		//t_cell *cell = &area->cells[i];
		//t_entity *entity = cell->entity;
		//t_terrain *terrain = cell->terrain;

		// spread_fire(area, cell);
		// illuminate(area, cell);
	}
}

void start(void)
{
	t_area *area = new_area(HOUSE);
	logger("Area loaded");
	t_cell *upleft = area->cells + area->width + 1;
	upleft->entity = new_entity('@');
	t_entity *player = get_player(area);
	logger("Player found");
	while (1)
	{
		int input = getch();
		if (input == 27)
			break;
		if ((player->action = get_player_action(input)) == NONE)
			continue;


		draw(area);
	}
}

int main(void)
{
    FILE *fp = fopen("logs/debug.log", "w");
    fclose(fp);

	signal(SIGSEGV, handle_segfault);
	init_ncurses();
	start();
	end_ncurses(0);
}
