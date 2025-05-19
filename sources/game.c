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

// temporary version
void character_creation(t_creature *player)
{
	t_winset cc_set;
	cc_set.height = 25;
	cc_set.width = 25;
	cc_set.y = (100 - cc_set.height) / 2;
	cc_set.x = (100 - cc_set.width) / 2;
	WINDOW *cc = my_newwin(cc_set);

	wmove(cc, 1, 0);
	echo();
	nocbreak();
	curs_set(1);

	wprintw(cc, " Character Creation\n\n");
	wprintw(cc, " Name: ");
	refresh_window(cc);

	player->name = my_calloc(20);
	wgetnstr(cc, player->name, 19);
	noecho();
	curs_set(0);
	cbreak();

	print_win(cc, " Select weapon with {red}LEFT{reset} or {red}RIGHT{reset}: ", NULL);
	int y, x;
	getyx(cc, y, x);
	e_item_type type = WEAPON_SIMPLE + 1;
	while (1)
	{
		t_item *weapon = new_weapon(type, COMMON);
		wprintw(cc, "%s", weapon->name);
		refresh_window(cc);

		int input = getch();
		if (input == ESCAPE)
		{
			free_item(weapon);
			return;
		}
		if (input == ENTER)
		{
			player->weapon = weapon;
			break;
		}
		if (input == KEY_LEFT)
		{
			type--;
			if (type == WEAPON_SIMPLE)
				type = WEAPON_SIMPLE_END - 1;
		}
		if (input == KEY_RIGHT)
		{
			type++;
			if (type == WEAPON_SIMPLE_END)
				type = WEAPON_SIMPLE + 1;
		}
		wmove(cc, y, x);
		wprintw(cc, "                          ");
		wmove(cc, y, x);
		flushinp();
		free_item(weapon);
	}
}

t_game *new_game(t_area *area)
{
	t_game *game = my_calloc(sizeof(*game));
	game->player = get_player(area);
	character_creation(game->player);
	enter_area(game, area);
	return game;
}
