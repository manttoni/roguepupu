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
	(void)game;
	print_log("{green}Game started!{reset}");
	while(1)
	{
		update_stat_win();
		draw_area();
		if (pc_act() == QUIT_GAME)
			return;
		draw_area();
		npc_act();
		flushinp(); // empty input buffer
	}
}

void enter_area(t_game *game, t_area *area)
{
	(void)game;
	print_log("Entered area: {red}%s{reset}", area->name);
	g_area = area;
}

t_game *new_game(t_area *area)
{
	t_game *game = my_calloc(sizeof(*game));
	enter_area(game, area);
	return game;
}
