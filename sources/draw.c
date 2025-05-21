#include "draw.h"
#include "windows.h"
#include "globals.h"
#include "file_reader.h"
#include "color.h"
#include "interface.h"
#include <ncurses.h>

void visual_effect(t_cell *cell, chtype effect)
{
	cell->highlight = effect;
	draw_cell(cell->last_draw.y, cell->last_draw.x, cell);
	refresh_window(map_win);
	usleep(100000);
	cell->highlight = 0;
	draw_cell(cell->last_draw.y, cell->last_draw.x, cell);
	refresh_window(map_win);
}

void cosmetic_effects(t_area *area)
{
	for (int i = 0; i < AREA(area); ++i)
	{
		t_cell *cell = &area->cells[i];
		t_creature *creature = cell->creature;
		if (creature != NULL && creature->bleeding > 0)
		{
			if (rand() % 4 == 0)
				creature->bleeding--;
			t_cell *blood_cell = cell;
			int i = 0;
			while (get_red(blood_cell->color) >= 2 && i++ < 15)
				blood_cell = random_neighbor(area, cell);
			if (i >= 15)
				print_log("There is {red}blood{reset} everywhere!");
			else
				change_color(&blood_cell->color, 1, 0, 0);
		}
	}
	draw_area(area);
}

void draw_cell(int y, int x, t_cell *cell)
{
	cell->last_draw = (t_coord){y, x};
	wmove(map_win, cell->last_draw.y, cell->last_draw.x);

	short color_id = pair_id(cell_fg(cell), cell_bg(cell));
	chtype highlight_color = cell->highlight & A_COLOR;

	wattron(map_win, cell->highlight);
	if (highlight_color == 0)
		wattron(map_win, COLOR_PAIR(color_id));

	waddch(map_win, cell_char(cell));

	wattroff(map_win, cell->highlight);
	if (highlight_color == 0)
		wattroff(map_win, COLOR_PAIR(color_id));
}

void draw_area(t_area *area)
{
	int y_max, x_max;
	getmaxyx(map_win, y_max, x_max);
	int y_center = y_max / 2;
	int x_center = x_max / 2;

	int player_index = get_player_index(area);
	int y_player = player_index / area->width;
	int x_player = player_index % area->width;

	int y_draw = y_center - y_player;
	int x_draw = x_center - x_player;

	werase(map_win);
	for (int i = 0; i < AREA(area); ++i)
	{
		t_cell *cell = &area->cells[i];
		if (y_draw < y_max - 1 && y_draw >= 1 && x_draw < x_max - 1 && x_draw >= 1)
			if (is_visible(area, &area->cells[player_index], cell))
				draw_cell(y_draw, x_draw, cell);
		x_draw++;
		if ((i + 1) % area->width == 0)
		{
			y_draw++;
			x_draw -= area->width;
		}
	}
	refresh_window(map_win);
	//wrefresh(map_win);
}
