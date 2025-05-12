#ifndef PLAYER_H
#define PLAYER_H

#include "action.h"

typedef struct s_player
{
	int	keys;
}	t_player;

e_action get_player_action(int input);


#endif
