#include "item.h"
#include "utils.h"
#include "globals.h"
#include "color.h"
#include "memory.h"
#include <stdlib.h>
#include <ncurses.h>

int item_equals(t_item *a, t_item *b)
{
	if (a == NULL || b == NULL)
		return 0;
	return a->id == b->id;
}

static t_item *copy_item(t_item *item)
{
	t_item *copy = my_calloc(1, sizeof(*copy));
	memmove(copy, item, sizeof(*copy));
	copy->name = my_strdup(item->name);
	copy->type = my_strdup(item->type);
	copy->rarity = my_strdup(item->type);
	if (strcmp(copy->type, "weapon") == 0)
	{
		t_weapon_data *wdata = &copy->data.weapon_data;
		wdata->properties = copy_char_array(wdata->properties);
		wdata->proficiency = my_strdup(wdata->proficiency);
		wdata->damage_type = my_strdup(wdata->damage_type);
		wdata->damage = my_strdup(wdata->damage);
	}
	else if (strcmp(copy->type, "armor") == 0)
	{
		t_armor_data *adata = &copy->data.armor_data;
		adata->proficiency = my_strdup(adata->proficiency);
	}
	else if (strcmp(copy->type, "potion") == 0)
	{
		t_potion_data *pdata = &copy->data.potion_data;
		pdata->effect = my_strdup(pdata->effect);
		pdata->duration = my_strdup(pdata->duration);
	}
	else if (strcmp(copy->type, "reagent") == 0)
	{
		t_reagent_data *rdata = &copy->data.reagent_data;
		rdata->properties = copy_char_array(rdata->properties);
	}
	copy->id = generate_id();
	return copy;
}

t_item *new_item(char *name)
{
	for (size_t i = 0; i < g_item_group_count; ++i)
	{
		t_item_group *group = &g_item_groups[i];
		t_item *array = group->array;
		size_t count = group->count;

		for (size_t j = 0; j < count; ++j)
		{
			if (strcmp(array[j].name, name) == 0)
			{
				t_item *item = copy_item(&array[j]);
				return item;
			}
		}
	}
	logger("Item not found: %s", name);
	end_ncurses(1);
}

int is_equipment(t_item *item)
{
	return is_weapon(item) || is_armor(item);
}

int is_armor(t_item *item)
{
	return strcmp(item->type, "armor") == 0;
}

int is_potion(t_item *item)
{
	return strcmp(item->type, "potion") == 0;
}

int is_weapon(t_item *item)
{
	return strcmp(item->type, "weapon") == 0;
}

