#include "game.h"
#include "memory.h"
#include "action.h"
#include "area.h"
#include "windows.h"
#include "draw.h"
#include "globals.h"
#include "interface.h"

void start(t_game *game)
{
	print_log("{green}Game started!{reset}");
	t_area *area = game->area;
	while(1)
	{
		update_stat_win(area);
		draw_area(area);
		if (pc_act(area) == QUIT_GAME)
			return;
		cosmetic_effects(area);
		draw_area(area);
		npc_act(area);
		cosmetic_effects(area);
		flushinp(); // empty input buffer
	}
}

void enter_area(t_game *game, t_area *area)
{
	print_log("Entered area: {red}%s{reset}", area->name);
	game->area = area;
}

t_game *new_game(t_area *area)
{
	t_game *game = my_calloc(sizeof(*game));
	game->player = get_player(area);
	enter_area(game, area);
	return game;
}
