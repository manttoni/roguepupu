#include "../headers/game.h"
#include "../headers/area.h"
#include "../headers/windows.h"
#include "../headers/player.h"
#include "../headers/draw.h"
#include "../headers/globals.h"

void start(t_game *game)
{
	print_log("Game started!");
	t_creature *player = get_player(game->area);
	t_area *area = game->area;
	while(1)
	{
		draw_area(area);

		int input = getch();
		if (input == ESCAPE)
		{
			print_log("Do you want to quit the game? ESC to confirm, any other to cancel.");
			if (getch() == ESCAPE)
				break;
			continue;
		}
		// get a valid player action
		player->action = get_player_action(input);
		if (player->action == NONE)
			continue;
		player_act(area);
		player->action = NONE;
		usleep(100000);
		enemy_act(area);
		flushinp(); // empty input buffer
	}
}

void enter_area(t_game *game, t_area *area, t_coord entry_point)
{
	game->area = area;
	add_creature(area, new_creature('@'), entry_point.y * area->width + entry_point.x);
	add_creature(area, new_creature('d'), 19 * area->width + 10);
	add_creature(area, new_creature('g'), 21 * area->width + 10);
	add_creature(area, new_creature('g'), 21 * area->width + 8);
	list_clear(&game->creatures);
}

t_game *new_game(t_area *start_area)
{
	t_game *game = my_calloc(sizeof(t_game));
	enter_area(game, start_area, (t_coord){1, 1});
	return game;
}
