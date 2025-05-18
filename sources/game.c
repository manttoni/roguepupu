#include "game.h"
#include "action.h"
#include "area.h"
#include "windows.h"
#include "draw.h"
#include "globals.h"
#include "interface.h"

void start(t_game *game)
{
	print_log("Game started!");
	t_area *area = game->area;
	while(1)
	{
		update_stat_win(area);
		draw_area(area);
		pc_act(area);
		cosmetic_effects(area);
		draw_area(area);
		npc_act(area);
		cosmetic_effects(area);
		flushinp(); // empty input buffer
	}
}

void enter_area(t_game *game, t_area *area, t_coord entry)
{
	print_log("Entered area: {red}%s{reset}", area->name);
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
