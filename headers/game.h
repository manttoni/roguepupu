#ifndef GAME_H
#define GAME_H

#include "utils.h"

// used for storing the current state of the game
typedef struct s_game
{
	t_node	*creatures;
	t_area	*area;
}	t_game;

void start(t_game *game);
t_game *new_game(t_area *area);

#endif
