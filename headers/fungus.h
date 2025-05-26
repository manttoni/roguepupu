#ifndef FUNGUS_H
#define FUNGUS_H

#include "area.h"

typedef struct s_fungus
{
	char	*name;
	short	color;
	int		glow;
	char	**properties;
	char	ch;
	int		rarity;
	char	**spawn;
}	t_fungus;

void populate_fungi(t_area *area);

#endif
