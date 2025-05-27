#include "mech.h"
#include "terrain.h"
#include "item.h"
#include "utils.h"
#include "creature.h"
#include "cell.h"
#include "area.h"
#include "game.h"
#include "globals.h"
#include "equipment.h"

void free_array(char **array)
{
	logger("Freeing an array");
	char **ptr = array;
	while (ptr != NULL && *ptr != NULL)
	{
		free(*ptr);
		ptr++;
	}
	free(array);
	logger("Array freed");
}

void free_g_fungi(void)
{
	for (int i = 0; i < g_fungus_count; ++i)
	{
		t_fungus *fungus = &g_fungi[i];
		free(fungus->name);
		free_array(fungus->properties);
		free_array(fungus->spawn);
		free(fungus);
	}
	free(g_fungi);
}

void free_g_creatures(void)
{
	for (int i = 0; i < g_creature_group_count; ++i)
	{
		t_creature_group *group = &g_creature_groups[i];
		for (int j = 0; j < group->count; ++j)
		{
			t_creature *creature = &group->array[j];
			free(creature->name);
			free(creature->size);
			free(creature);
		}
		free(group);
	}
	free(g_creature_groups);
}

void free_g_items(void)
{
	for (int i = 0; i < g_item_group_count; ++i)
	{
		t_item_group *group = &g_item_groups[i];
		logger("Freeing %s", group->category);
		for (int j = 0; j < group->count; ++j)
		{
			t_item *item = &group->array[j];
			logger("Freeing %s", item->name);
			u_item_data data = item->data;
			if (strcmp(item->type, "weapon") == 0)
			{
				t_weapon_data wdata = data.weapon_data;
				free_array(wdata.properties);
				free(wdata.proficiency);
				free(wdata.damage_type);
				free(wdata.damage);
			}
			else if (strcmp(item->type, "potion") == 0)
			{
				t_potion_data pdata = data.potion_data;
				free(pdata.effect);
				free(pdata.duration);
			}
			else if (strcmp(item->type, "reagent") == 0)
			{
				t_reagent_data rdata = data.reagent_data;
				free_array(rdata.properties);
			}
			logger("item data freed");
			free(item->name);
			free(item->rarity);
			free(item);
			logger("item freed");
		}
		free(group);
		logger("item group freed");
	}
	free(g_item_groups);
	logger("item group array freed");
}

void free_globals(void)
{
	free_g_items();
	free_g_creatures();
	free_g_fungi();
}

void free_mech(t_mech *mech)
{
	free(mech);
}

void free_inventory(t_node **inventory)
{
	if (*inventory == NULL)
		return;
	logger("Freeing inventory");
	t_node *current = *inventory;
	while (current != NULL)
	{
		t_item *item = (t_item *) current->data;
		free(item);
		current = current->next;
	}
	list_clear(inventory);
	logger("Inventory freed");
}

void free_terrain(t_terrain *terrain)
{
	if (terrain != NULL)
		free_inventory(&(terrain->loot));
	free(terrain);
}

void free_item(t_item *item)
{
	free(item);
}

void free_creature(t_creature *creature)
{
	if (creature == NULL)
		return;
	free_inventory(&creature->inventory);
	free(creature);
}

void free_fungus(t_fungus *fungus)
{
	free(fungus);
}

void free_cell(t_cell *cell)
{
	free_creature(cell->creature);
	free_item(cell->item);
	free_terrain(cell->terrain);
	free_mech(cell->mech);
	free_fungus(cell->fungus);
}

void free_area(t_area *area)
{
	logger("Freeing cells of %s", area->name);
	for (size_t i = 0; i < AREA(area); ++i)
	{
		free_cell(&area->cells[i]);
	}
	free(area->cells);
	logger("Cells freed");
	free(area->name);
	free(area);
}

void free_game(void)
{
	logger("Freeing area");
	free_area(g_area);
	logger("Area freed");
}

