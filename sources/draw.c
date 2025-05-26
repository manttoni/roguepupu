#include "draw.h"
#include "windows.h"
#include "globals.h"
#include "file_reader.h"
#include "color.h"
#include "interface.h"
#include <ncurses.h>

void visual_effect(t_creature *creature, chtype effect)
{
	t_cell *cell = get_creature_cell(creature);
	cell->highlight = effect;
	draw_cell(cell->last_draw.y, cell->last_draw.x, cell);
	refresh_window(map_win);
	usleep(100000);
	cell->highlight = 0;
	draw_cell(cell->last_draw.y, cell->last_draw.x, cell);
	refresh_window(map_win);
}

void draw_cell(int y, int x, t_cell *cell)
{
	int vision = visibility(get_player_cell(), cell);
	if (vision == VISION_NONE)
		return;

	char ch = cell_char(cell);
	short color_id;
	chtype highlight_color = cell->highlight & A_COLOR;
	cell->last_draw = (t_coord){y, x};

	switch (vision)
	{
		case VISION_GHOST:
			if (strchr(CELL_GHOST_CHARS, ch) == NULL)
				return;
			color_id = pair_id(COLOR_CELL_GHOST, COLOR_BLACK, 0);
			break;
		case VISION_FULL:
			color_id = pair_id(cell_fg(cell), cell_bg(cell), COLOR_NORMAL);
			break;
		default:
			break;
	}

	wattron(map_win, cell->highlight);
	if (highlight_color == 0)
		wattron(map_win, COLOR_PAIR(color_id));

	wmove(map_win, y, x);
	waddch(map_win, ch);

	wattroff(map_win, cell->highlight);
	if (highlight_color == 0)
		wattroff(map_win, COLOR_PAIR(color_id));
}

void draw_area(void)
{
	int y_max, x_max;
	getmaxyx(map_win, y_max, x_max);
	int y_center = y_max / 2;
	int x_center = x_max / 2;

	int player_index = get_player_index();
	int y_player = player_index / g_area->width;
	int x_player = player_index % g_area->width;

	int y_draw = y_center - y_player;
	int x_draw = x_center - x_player;

	werase(map_win);
	for (size_t i = 0; i < AREA(g_area); ++i)
	{
		if (y_draw < y_max - 1 && y_draw >= 1 && x_draw < x_max - 1 && x_draw >= 1)
			draw_cell(y_draw, x_draw, get_cell(i));

		x_draw++;
		if ((i + 1) % g_area->width == 0)
		{
			y_draw++;
			x_draw -= g_area->width;
		}
	}
	refresh_window(map_win);
}
