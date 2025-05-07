#include <signal.h>
#include <ncurses.h>
#include <stdlib.h>
#include "../headers/area.h"
#include "../headers/utils.h"
#include "../headers/game.h"
int main(void)
{
	// reset debug.log
    fclose((fopen("logs/debug.log", "w")));
	signal(SIGSEGV, handle_segfault);

	init_ncurses();
	t_game *game = new_game(new_area(HOUSE));
	start(game);
	end_ncurses(0);
}
