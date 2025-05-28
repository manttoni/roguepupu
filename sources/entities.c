#include "entities.h"
#include "globals.h"

t_creature_group *get_creature_group(char *group)
{
	for (size_t i = 0; i < g_creature_group_count; ++i)
	{
		t_creature_group *creature_group = &g_creature_groups[i];
		if (strcmp(group, creature_group->category) == 0)
			return creature_group;
	}
	return NULL;
}

t_item_group *get_item_group(char *group)
{
	// potions are in item_groups
	for (size_t i = 0; i < g_item_group_count; ++i)
	{
		t_item_group *item_group = &g_item_groups[i];
		if (strcmp(group, item_group->category) == 0)
			return item_group;
	}
	return NULL;
}
