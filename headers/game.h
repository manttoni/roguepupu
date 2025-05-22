#ifndef GAME_H
#define GAME_H

#include "utils.h"
#include "area.h"

// holds values specific to one run
typedef struct s_game
{
	t_area		*area; // this will be an array probably
	t_creature	*player; // when changing areas, game still remembers player
}	t_game;

void draw(t_area *area);
void start(void);
t_game *new_game(t_area *area);

#endif
