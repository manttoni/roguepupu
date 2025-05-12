#include "../headers/game.h"
#include "../headers/area.h"
#include "../headers/windows.h"
#include "../headers/player.h"

void draw_cell(int y, int x, t_cell *cell)
{
	wmove(map_win, y, x);
	if (cell->highlight & REVERSE)
		wattron(map_win, A_REVERSE);
	if (cell->highlight & SELECTED)
		wattron(map_win, COLOR_PAIR(SELECTED));
	unsigned char ch = cell->creature == NULL ? cell->terrain->ch : cell->creature->ch;
	waddch(map_win, ch);
	wattroff(map_win, COLOR_PAIR(SELECTED) | A_REVERSE);
}

void draw(t_area *area)
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

void start(t_game *game)
{
	print_log("Game started!");
	while(1)
	{
		t_creature *player = get_player(game->area);
		t_area *area = game->area;
		int player_index = get_player_index(game->area);
		t_cell *cell = &area->cells[player_index];

		draw(area);

		int input = getch();
		player->action = get_player_action(input);
		if (player->action == NONE)
			break;
		act(area, cell, player);
		player->action = NONE;
	}
}

void enter_area(t_game *game, t_area *area, t_coord entry_point)
{
	game->area = area;
	add_creature(area, new_creature('@'), entry_point.y * area->width + entry_point.x);
	list_clear(&game->creatures);
	game->creatures = get_creatures(area);
}

t_game *new_game(t_area *start_area)
{
	t_game *game = my_calloc(sizeof(t_game));
	enter_area(game, start_area, (t_coord){1, 1});
	return game;
}
