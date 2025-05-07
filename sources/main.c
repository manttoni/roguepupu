#include <signal.h>
#include <ncurses.h>
#include <stdlib.h>
#include "../headers/area.h"
#include "../headers/utils.h"

void creature_actions(t_area *area)
{
	(void) area;
	// increase creatures turn counter
	// when/if it reaches a limit, its that creatures turn
	// 	creature does some action, if its the player the player is prompted
	// 	depending on action the counter decreases some amount
	// loop all creatures
}

void start(void)
{
	// main loop does an iteration once a tick
	// every round it calls creature_actions
	// and other things like fire spreading, light stuff
}

int main(void)
{
    FILE *fp = fopen("logs/debug.log", "w");
    fclose(fp);

	signal(SIGSEGV, handle_segfault);
	init_ncurses();
	start();
	end_ncurses(0);
}
