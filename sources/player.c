#include "../headers/windows.h"
#include "../headers/player.h"
#include "../headers/creature.h"
#include <ncurses.h>

e_action get_player_action(int input)
{
	switch(input)
	{
		case KEY_UP:
			return MOVE_UP;
		case KEY_DOWN:
			return MOVE_DOWN;
		case KEY_LEFT:
			return MOVE_LEFT;
		case KEY_RIGHT:
			return MOVE_RIGHT;
		case 'i':
			print_log("Interacting");
			return INTERACT;
		default:
			return NONE;
	}
}
