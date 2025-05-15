#include "../headers/draw.h"
#include "../headers/windows.h"
#include "../headers/globals.h"
#include "../headers/file_reader.h"
#include "../headers/color.h"
#include <ncurses.h>

short pair_id(t_cell *cell)
{
	static int pairs[256];
	static int next_free = 3;
	short creature = cell->creature == NULL ? COLOR_WHITE : cell->creature->color;
	short terrain = cell->terrain->color;
	creature = min(255, creature);
	terrain = min(255, terrain);
	short pair_id = 256 * creature + terrain;

	if (pair_id >= COLOR_PAIRS)
		return 0;

	int i = 0;
	for (; i < next_free; ++i)
		if (pairs[i] == pair_id)
			break;
	if (i == next_free) // was not initialized yet
	{
		init_pair(i, creature, terrain);
		pairs[i] = pair_id;
		next_free++;
	}
	return i;
}

void draw_cell(int y, int x, t_cell *cell)
{
	wmove(map_win, y, x);

	// cell highlights
	if (cell->highlight & REVERSE)
		wattron(map_win, A_REVERSE);
	if (cell->highlight & SELECTED)
		wattron(map_win, COLOR_PAIR(SELECTED_HIGHLIGHT));
	else
		wattron(map_win, COLOR_PAIR(pair_id(cell)));

	char ch = cell->creature == NULL ? cell->terrain->ch : cell->creature->ch;
	waddch(map_win, ch);

	if (cell->highlight & REVERSE)
		wattroff(map_win, A_REVERSE);
	if (cell->highlight & SELECTED)
		wattroff(map_win, COLOR_PAIR(SELECTED_HIGHLIGHT));
	else
		wattroff(map_win, COLOR_PAIR(pair_id(cell)));
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
	wrefresh(map_win);
}
