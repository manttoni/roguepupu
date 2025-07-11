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
		print_debug();
		g_allocations_made = 0;
		g_allocations_size = 0;
		update_stat_win();
		print_charactersheet(get_player());
		draw_area();
		if (pc_act() == QUIT_GAME)
			return;
		draw_area();
		npc_act();
		flushinp(); // empty input buffer
	}
}

