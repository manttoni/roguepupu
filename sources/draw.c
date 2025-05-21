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
	for (int i = 0; i < AREA(g_area); ++i)
	{
		t_cell *cell = &g_area->cells[i];
		if (y_draw < y_max - 1 && y_draw >= 1 && x_draw < x_max - 1 && x_draw >= 1)
		{
			if (is_visible(&g_area->cells[player_index], cell))
				draw_cell(y_draw, x_draw, cell);
			else if (was_seen(cell) && cell->terrain->ch != '.')
			{
				t_cell ghost;
				memset(&ghost, 0, sizeof(t_cell));
				t_terrain t;
				memset(&t, 0, sizeof(t_terrain));
				t.ch = cell->terrain->ch;
				t.color = color_id((t_color){1,1,1});
				ghost.terrain = &t;
				ghost.color = COLOR_BLACK;
				draw_cell(y_draw, x_draw, &ghost);
			}
		}
		x_draw++;
		if ((i + 1) % g_area->width == 0)
		{
			y_draw++;
			x_draw -= g_area->width;
		}
	}
	refresh_window(map_win);
}
