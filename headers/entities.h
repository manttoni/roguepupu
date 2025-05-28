#ifndef ENTITIES_H
#define ENTITIES_H

#include "item.h"
#include "creature.h"

typedef struct s_item_group
{
	char	*category;
	t_item	*array;
	size_t	count;
}	t_item_group;

typedef struct s_creature_group
{
	char		*category;
	t_creature	*array;
	size_t		count;
}	t_creature_group;

t_item_group *get_item_group(char *group);
t_creature_group *get_creature_group(char *group);

#endif
