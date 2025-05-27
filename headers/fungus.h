#ifndef FUNGUS_H
#define FUNGUS_H

#include "area.h"

typedef struct s_fungus
{
	char	**properties;
	char	**spawn;
	char	*name;
	char	*rarity;
	char	ch;
	int		glow;
	short	color;
}	t_fungus;

void populate_fungi(t_area *area);

#endif
