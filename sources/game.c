#include "../headers/game.h"
#include "../headers/area.h"

void draw_cell(int y, int x, t_cell *cell)
{
	move(y, x);
	unsigned char ch = cell->creature == NULL ? cell->terrain->ch : cell->creature->ch;
	addch(ch);
}

void draw(t_game *game)
{
	t_area *area = game->area;

	int y_max, x_max;
	getmaxyx(stdscr, y_max, x_max);
	int y_center = y_max / 2;
	int x_center = x_max / 2;

	int player_index = get_player_index(area);
	int y_player = player_index / area->width;
	int x_player = player_index % area->width;

	int y_draw = y_center - y_player;
	int x_draw = x_center - x_player;

	for (int i = 0; i < AREA(game->area); ++i)
	{
		if (y_draw < y_max && y_draw >= 0
			&& x_draw < x_max && x_draw >= 0)
			draw_cell(y_draw, x_draw, &area->cells[i]);
		x_draw++;
		if ((i + 1) % area->width == 0)
		{
			y_draw++;
			x_draw -= area->width;
		}
	}
}

void start(t_game *game)
{
	draw(game);
	while(1)
	{
		int input = getch();
		if (input == 27)
			break;
		draw(game);
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
