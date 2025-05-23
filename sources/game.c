#include "game.h"
#include "memory.h"
#include "action.h"
#include "area.h"
#include "windows.h"
#include "draw.h"
#include "globals.h"
#include "interface.h"

void start()
{
	print_log("{green}Game started!{reset}");
	t_abilities a = get_player()->abilities;
	print_log("str:%d dex:%d con:%d int:%d wis:%d cha:%d", a.strength, a.dexterity, a.constitution, a.intelligence, a.wisdom, a.charisma);
	while(1)
	{
		logger("Updating status window");
		update_stat_win();
		logger("Drawing area");
		draw_area();
		logger("Player acting");
		if (pc_act() == QUIT_GAME)
			return;
		logger("Drawing area");
		draw_area();
		logger("Npc acting");
		npc_act();
		logger("Round completed");
		flushinp(); // empty input buffer
	}
}

