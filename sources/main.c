#include <signal.h>
#include <ncurses.h>
#include <stdlib.h>
#include "../headers/area.h"
#include "../headers/utils.h"
#include "../headers/game.h"
#include "../headers/windows.h"

WINDOW *map_win = NULL;
WINDOW *log_win = NULL;

t_game *select_area(void)
{
	t_winset selset;
	selset.height = 25;
	selset.width = 25;
	selset.y = 37;
	selset.x = 37;
	WINDOW *sel_win = my_newwin(selset);
	refresh_window(sel_win);
	getch();
	return NULL;
}

int main(void)
{
	// reset debug.log
    fclose((fopen("logs/debug.log", "w")));
	signal(SIGSEGV, handle_segfault);

	init_ncurses();
	//t_game *game = select_area();
	init_windows();
	t_game *game = new_game(new_area(DUNGEON));
	start(game);
	delete_windows();
	end_ncurses(0);
}
