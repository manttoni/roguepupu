#include "mech.h"
#include "terrain.h"
#include "item.h"
#include "utils.h"
#include "creature.h"
#include "cell.h"
#include "area.h"
#include "game.h"
#include "globals.h"

void free_weapons(void)
{
	for (int i = 0; i < g_weapon_count; ++i)
	{
		char **properties = g_weapons[i].data.weapon_data.properties;
		char **ptr = properties;
		while (properties != NULL && *properties != NULL)
		{
			free(*properties);
			properties++;
		}
		free(ptr);
		free(g_weapons[i].name);
		free(g_weapons[i].rarity);
		t_weapon_data data = g_weapons[i].data.weapon_data;
		free(data.proficiency);
		free(data.damage_type);
		free(data.damage);
	}
	free(g_weapons);
}

void free_globals(void)
{
	logger("freeing weapons");
	free_weapons();
	logger("weapons freed");
}

void free_mech(t_mech *mech)
{
	free(mech);
}

void free_terrain(t_terrain *terrain)
{
	if (terrain != NULL)
		list_clear(&terrain->loot);
	free(terrain);
}

void free_item(t_item *item)
{
	free(item);
}

void free_inventory(t_node *inventory)
{
	t_node *ptr = inventory;
	while (ptr != NULL)
	{
		free_item((t_item*) ptr->data);
		ptr = ptr->next;
	}
	list_clear(&inventory);
}

void free_creature(t_creature *creature)
{
	if (creature == NULL)
		return;
	free_item(creature->weapon);
	free_inventory(creature->inventory);
	free(creature);
}

void free_cell(t_cell *cell)
{
	free_creature(cell->creature);
	free_item(cell->item);
	free_terrain(cell->terrain);
	free_mech(cell->mech);
}

void free_area(t_area *area)
{
	for (int i = 0; i < AREA(area); ++i)
	{
		free_cell(&area->cells[i]);
	}
	free(area->cells);
	free(area->name);
	free(area);
}

void free_game(t_game *game)
{
	free_area(g_area);
	free(game);
}

