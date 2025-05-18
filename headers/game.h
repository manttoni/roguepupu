#ifndef GAME_H
#define GAME_H

#include "utils.h"
#include "area.h"

typedef struct s_game
{
	t_area		*area;
	t_creature *player;
}	t_game;

void draw(t_area *area);
void start(t_game *game);
t_game *new_game(t_area *area);

#endif
