#include "cJSON.h"
#include "cell.h"
#include "globals.h"
#include "utils.h"
#include <string.h>
#include <errno.h>

t_potion_data parse_potion_data(cJSON *potion)
{
	t_potion_data data;
	cJSON *effect = cJSON_GetObjectItemCaseSensitive(potion, "effect");
	cJSON *duration = cJSON_GetObjectItemCaseSensitive(potion, "duration");
	if (!cJSON_IsString(effect) || !cJSON_IsString(duration))
	{
		logger("Potion data parsing error");
		end_ncurses(ERROR_JSON_PARSE);
	}

	data.effect = strdup(effect->valuestring);
	data.duration = strdup(duration->valuestring);
	return data;
}

t_weapon_data parse_weapon_data(cJSON *weapon)
{
	t_weapon_data data;
	cJSON *damage = cJSON_GetObjectItemCaseSensitive(weapon, "damage");
	cJSON *damage_type = cJSON_GetObjectItemCaseSensitive(weapon, "damage_type");
	cJSON *proficiency = cJSON_GetObjectItemCaseSensitive(weapon, "proficiency");
	if (!cJSON_IsString(damage) || !cJSON_IsString(damage_type) || !cJSON_IsString(proficiency))
	{
		logger("weapon data parsing error");
		end_ncurses(ERROR_JSON_PARSE);
	}
	data.damage = my_strdup(damage->valuestring);
	data.damage_type = my_strdup(damage_type->valuestring);
	data.proficiency = my_strdup(proficiency->valuestring);

	cJSON *properties = cJSON_GetObjectItemCaseSensitive(weapon, "properties");
	if (!cJSON_IsArray(properties))
	{
		logger("Properties in weapon parser is not an array");
		end_ncurses(ERROR_JSON_PARSE);
	}
	int len = cJSON_GetArraySize(properties);
	if (len > 0)
		data.properties = my_calloc(len + 1, sizeof(char*)); // NULL terminate
	else
	{
		data.properties = NULL;
		return data;
	}

	cJSON *prop = NULL;
	int i = 0;
	cJSON_ArrayForEach(prop, properties)
	{
		if (!cJSON_IsString(prop))
		{
			logger("Weapon property not a string");
			end_ncurses(ERROR_JSON_PARSE);
		}
		data.properties[i] = my_strdup(prop->valuestring);
		i++;
	}
	return data;
}

short get_color(char *rarity)
{
	if (strcmp(rarity, "common") == 0)
		return COLOR_ITEM_COMMON;
	else if (strcmp(rarity, "uncommon") == 0)
		return COLOR_ITEM_UNCOMMON;
	else if (strcmp(rarity, "rare") == 0)
		return COLOR_ITEM_RARE;
	else if (strcmp(rarity, "very rare") == 0)
		return COLOR_ITEM_VERY_RARE;
	else if (strcmp(rarity, "legendary") == 0)
		return COLOR_ITEM_LEGENDARY;
	logger("invalid rarity (weapon)");
	end_ncurses(ERROR_JSON_PARSE);
	return 0;
}

t_creature *parse_creatures(char *raw, char *type, int *count)
{
	cJSON *json = cJSON_Parse(raw);
	if (json == NULL)
	{
		logger("cJSON_Parse fail (%s)", type);
		end_ncurses(errno);
	}

	cJSON *creatures = cJSON_GetObjectItemCaseSensitive(json, type);
	if (!cJSON_IsArray(creatures))
	{
		logger("cJSON: not an array (%s)", type);
		end_ncurses(ERROR_JSON_PARSE);
	}

	int n = cJSON_GetArraySize(creatures);
	t_creature *array = my_calloc(n, sizeof(*array));
	*count = n;

	cJSON *creature = NULL;
	int i = 0;
	cJSON_ArrayForEach(creature, creatures)
	{
		cJSON *name = cJSON_GetObjectItemCaseSensitive(creature, "name");
		cJSON *level = cJSON_GetObjectItemCaseSensitive(creature, "level");
		cJSON *size = cJSON_GetObjectItemCaseSensitive(creature, "size");
		cJSON *color = cJSON_GetObjectItemCaseSensitive(creature, "color");
		cJSON *ai = cJSON_GetObjectItemCaseSensitive(creature, "ai");
		cJSON *faction = cJSON_GetObjectItemCaseSensitive(creature, "faction");
		cJSON *abilities = cJSON_GetObjectItemCaseSensitive(creature, "abilities");

		// Validate required fields
		if (!cJSON_IsString(name) || !cJSON_IsNumber(level) || !cJSON_IsString(size) ||
			!cJSON_IsString(color) || !cJSON_IsString(ai) || !cJSON_IsString(faction) ||
			!cJSON_IsObject(abilities))
		{
			logger("Creature parsing error (%s)", type);
			end_ncurses(ERROR_JSON_PARSE);
		}

		array[i].name = my_strdup(name->valuestring);
		array[i].level = level->valueint;
		array[i].size = my_strdup(size->valuestring);
		array[i].color = resolve_macro(color->valuestring);
		array[i].ai = resolve_macro(ai->valuestring);
		array[i].faction = resolve_macro(faction->valuestring);

		// Parse abilities individually
		//cJSON *ability = NULL;
		cJSON *strength = cJSON_GetObjectItemCaseSensitive(abilities, "strength");
		cJSON *dexterity = cJSON_GetObjectItemCaseSensitive(abilities, "dexterity");
		cJSON *constitution = cJSON_GetObjectItemCaseSensitive(abilities, "constitution");
		cJSON *intelligence = cJSON_GetObjectItemCaseSensitive(abilities, "intelligence");
		cJSON *wisdom = cJSON_GetObjectItemCaseSensitive(abilities, "wisdom");
		cJSON *charisma = cJSON_GetObjectItemCaseSensitive(abilities, "charisma");

		if (!cJSON_IsNumber(strength) || !cJSON_IsNumber(dexterity) || !cJSON_IsNumber(constitution) ||
			!cJSON_IsNumber(intelligence) || !cJSON_IsNumber(wisdom) || !cJSON_IsNumber(charisma))
		{
			logger("Creature abilities parsing error (%s)", type);
			end_ncurses(ERROR_JSON_PARSE);
		}

		array[i].abilities.strength = strength->valueint;
		array[i].abilities.dexterity = dexterity->valueint;
		array[i].abilities.constitution = constitution->valueint;
		array[i].abilities.intelligence = intelligence->valueint;
		array[i].abilities.wisdom = wisdom->valueint;
		array[i].abilities.charisma = charisma->valueint;

		i++;
	}

	cJSON_Delete(json);
	free(raw);
	return array;
}

t_item *parse_items(char *raw, char *type, int *count)
{
	cJSON *json = cJSON_Parse(raw);
	if (json == NULL)
	{
		logger("cJSON_Parse fail (%s)", type);
		end_ncurses(errno);
	}

	cJSON *items = cJSON_GetObjectItemCaseSensitive(json, type);
	if (!cJSON_IsArray(items))
	{
		logger("cJSON: not an array (%s)", type);
		end_ncurses(ERROR_JSON_PARSE);
	}
	int n = cJSON_GetArraySize(items);
	t_item *array = my_calloc(n, sizeof(*array));
	*count = n;

	cJSON *item = NULL;
	int i = 0;
	cJSON_ArrayForEach(item, items)
	{
		cJSON *name = cJSON_GetObjectItemCaseSensitive(item, "name");
		cJSON *rarity = cJSON_GetObjectItemCaseSensitive(item, "rarity");
		if (!cJSON_IsString(name) || !cJSON_IsString(rarity))
		{
			logger("Item parsing error");
			end_ncurses(ERROR_JSON_PARSE);
		}

		array[i].name = my_strdup(name->valuestring);
		array[i].rarity = my_strdup(rarity->valuestring);
		array[i].ch = toupper(type[0]);
		array[i].type = type;
		array[i].color = get_color(array[i].rarity);

		if (strcmp("weapon", type) == 0)
			array[i].data.weapon_data = parse_weapon_data(item);
		else if(strcmp("potion", type) == 0)
			array[i].data.potion_data = parse_potion_data(item);
		i++;
	}
	cJSON_Delete(json);
	free(raw);
	return array;
}

t_area *parse_area(char *raw)
{
	cJSON *json = cJSON_Parse(raw);
	if (json == NULL)
	{
		logger("cJSON_Parse area fail");
		end_ncurses(errno);
	}
	cJSON *name = cJSON_GetObjectItemCaseSensitive(json, "name");
	cJSON *level = cJSON_GetObjectItemCaseSensitive(json, "level");
	cJSON *terrain = cJSON_GetObjectItemCaseSensitive(json, "terrain");
	cJSON *mech = cJSON_GetObjectItemCaseSensitive(json, "mech");
	cJSON *items = cJSON_GetObjectItemCaseSensitive(json, "items");
	cJSON *creatures = cJSON_GetObjectItemCaseSensitive(json, "creatures");

	t_area *area = my_calloc(1, sizeof(*area));
	if (!cJSON_IsString(name)
		|| name->valuestring == NULL
		|| !cJSON_IsNumber(level)
		|| !cJSON_IsArray(terrain)
		|| !cJSON_IsArray(mech)
		|| !cJSON_IsArray(items)
		|| !cJSON_IsArray(creatures))
	{
		logger("Parsing error");
		end_ncurses(errno);
	}

	area->name = strdup(name->valuestring);
	area->level = level->valueint;
	area->height = cJSON_GetArraySize(terrain);
	area->width = strlen(cJSON_GetArrayItem(terrain, 0)->valuestring);
	area->cells = my_calloc(AREA(area), sizeof(*(area->cells)));
	for (int i = 0; i < area->height; ++i)
	{
		cJSON *terline = cJSON_GetArrayItem(terrain, i);
		cJSON *mecline = cJSON_GetArrayItem(mech, i);
		cJSON *iteline = cJSON_GetArrayItem(items, i);
		cJSON *creline = cJSON_GetArrayItem(creatures, i);

		if (!cJSON_IsString(terline) || !cJSON_IsString(mecline) || !cJSON_IsString(iteline) || !cJSON_IsString(creline))
		{
			logger("Parsing error");
			end_ncurses(errno);
		}

		char *ter = terline->valuestring;
		char *mec = mecline->valuestring;
		char *ite = iteline->valuestring;
		char *cre = creline->valuestring;

		if (ter == NULL || mec == NULL || ite == NULL || cre == NULL)
		{
			logger("Parsing error");
			end_ncurses(errno);
		}
		// error checking almost done. arrays might still be different sizes
		for (int j = 0; j < area->width; ++j)
			area->cells[j + i * area->width] = new_cell(ter[j], mec[j], ite[j], cre[j], area->level);
	}

	cJSON_Delete(json);

	free(raw);
	return area;
}
