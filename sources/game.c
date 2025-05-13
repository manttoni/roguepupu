#include "../headers/game.h"
#include "../headers/area.h"
#include "../headers/windows.h"
#include "../headers/player.h"
#include "../headers/draw.h"

void start(t_game *game)
{
	print_log("Game started!");
	while(1)
	{
		t_creature *player = get_player(game->area);
		t_area *area = game->area;
		int player_index = get_player_index(game->area);
		t_cell *cell = &area->cells[player_index];

		draw_area(area);

		int input = getch();
		if (input == 27)
		{
			break;
		}
		player->action = get_player_action(input);
		if (player->action == NONE)
			continue;
		player_act(area, cell, player);
		player->action = NONE;
		usleep(100000);
		flushinp(); // empty input buffer
	}
}

void enter_area(t_game *game, t_area *area, t_coord entry_point)
{
	game->area = area;
	add_creature(area, new_creature('@'), entry_point.y * area->width + entry_point.x);
	add_creature(area, new_creature('d'), 19 * area->width + 10);
	list_clear(&game->creatures);
}

t_game *new_game(t_area *start_area)
{
	t_game *game = my_calloc(sizeof(t_game));
	enter_area(game, start_area, (t_coord){1, 1});
	return game;
}
