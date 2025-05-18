#include "../headers/draw.h"
#include "../headers/windows.h"
#include "../headers/globals.h"
#include "../headers/file_reader.h"
#include "../headers/color.h"
#include <ncurses.h>

void cosmetic_effects(t_area *area)
{
	for (int i = 0; i < AREA(area); ++i)
	{
		t_cell *cell = &area->cells[i];
		t_creature *creature = cell->creature;
		if (creature != NULL && creature->bleeding > 0)
		{
			creature->bleeding -= rand() % 2;
			if(get_red(cell->terrain->color) < 5)
				change_color(&cell->terrain->color, 1, 0, 0);
		}
	}
	draw_area(area);
}

void draw_cell(int y, int x, t_cell *cell)
{
	wmove(map_win, y, x);

	short color_id = pair_id(cell_fg(cell), cell_bg(cell));

	// cell highlights
	if (cell->highlight & HIGHLIGHT_REVERSE)
		wattron(map_win, A_REVERSE);
	if (cell->highlight & HIGHLIGHT_SELECTED)
		wattron(map_win, COLOR_PAIR(COLOR_SELECTED));
	else
		wattron(map_win, COLOR_PAIR(color_id));

	char ch = cell_char(cell);
	waddch(map_win, ch);

	if (cell->highlight & HIGHLIGHT_REVERSE)
		wattroff(map_win, A_REVERSE);
	if (cell->highlight & HIGHLIGHT_SELECTED)
		wattroff(map_win, COLOR_PAIR(COLOR_SELECTED));
	else
		wattroff(map_win, COLOR_PAIR(color_id));
}

void draw_area(t_area *area)
{
	logger("Drawing area");
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
	wrefresh(map_win);
}
