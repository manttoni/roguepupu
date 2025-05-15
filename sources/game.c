#include "../headers/game.h"
#include "../headers/area.h"
#include "../headers/windows.h"
#include "../headers/player.h"
#include "../headers/draw.h"
#include "../headers/globals.h"
#include "../headers/interface.h"

void start(t_game *game)
{
	print_log("Game started!");
	t_area *area = game->area;
	while(1)
	{
		update_stat_win(area);
		logger("stat win updated");
		draw_area(area);
		logger("area drawn");
		int input = getch();
		if (input == ESCAPE)
		{
			print_log("Do you want to quit the game? ESC to confirm, any other to cancel.");
			if (getch() == ESCAPE)
				break;
			continue;
		}
		e_action player_action = get_player_action(input);
		if (player_action == NONE) // invalid input
			continue;
		player_act(area, player_action);
		draw_area(area);
		enemy_act(area);
		flushinp(); // empty input buffer
	}
}

void enter_area(t_game *game, t_area *area, t_coord entry)
{
	print_log("Entered area: %s", area->name);
	(void)entry;
	game->area = area;
	list_clear(&game->creatures);
}

t_game *new_game(t_area *start_area)
{
	print_log("New game");
	t_game *game = my_calloc(sizeof(t_game));
	enter_area(game, start_area, (t_coord){1, 1});
	return game;
}
