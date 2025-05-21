#include "cJSON.h"
#include "cell.h"
#include "globals.h"
#include "utils.h"
#include <string.h>
#include <errno.h>

t_item *parse_weapons(char *raw)
{
	cJSON *json = cJSON_Parse(raw);
	if (json == NULL)
	{
		logger("cJSON_Parse fail in weapons.json");
		end_ncurses(errno);
	}

	cJSON *weapons = cJSON_GetObjectItemCaseSensitive(json, "weapons");
	if (!cJSON_IsArray(weapons))
	{
		logger("cJSON: not an array (weapons)");
		end_ncurses(ERROR_JSON_PARSE);
	}
	int n = cJSON_GetArraySize(weapons);
	g_weapon_count = n;
	t_item *array = my_calloc(n * sizeof(*array));

	cJSON *weapon = NULL;
	int i = 0;
	cJSON_ArrayForEach(weapon, weapons)
	{
		cJSON *name = cJSON_GetObjectItemCaseSensitive(weapon, "name");
		cJSON *damage = cJSON_GetObjectItemCaseSensitive(weapon, "damage");
		cJSON *damage_type = cJSON_GetObjectItemCaseSensitive(weapon, "damage_type");
		cJSON *proficiency = cJSON_GetObjectItemCaseSensitive(weapon, "proficiency");
		cJSON *rarity = cJSON_GetObjectItemCaseSensitive(weapon, "rarity");
		cJSON *properties = cJSON_GetObjectItemCaseSensitive(weapon, "properties");
		if (!cJSON_IsString(name) || !cJSON_IsString(damage) || !cJSON_IsString(damage_type)
			|| !cJSON_IsString(proficiency) || !cJSON_IsString(rarity) || !cJSON_IsArray(properties))
		{
			logger("json format error (weapon: %s)", name->valuestring);
			end_ncurses(ERROR_JSON_PARSE);
		}
		array[i].name = my_strdup(name->valuestring);
		array[i].rarity = my_strdup(rarity->valuestring);
		array[i].ch = 'W';
		array[i].type = "weapon";
		array[i].proficiency = my_strdup(proficiency->valuestring);

		if (strcmp(array[i].rarity, "common") == 0)
			array[i].color = COLOR_ITEM_COMMON;
		else if (strcmp(array[i].rarity, "uncommon") == 0)
			array[i].color = COLOR_ITEM_UNCOMMON;
		else if (strcmp(array[i].rarity, "rare") == 0)
			array[i].color = COLOR_ITEM_RARE;
		else if (strcmp(array[i].rarity, "very rare") == 0)
			array[i].color = COLOR_ITEM_VERY_RARE;
		else if (strcmp(array[i].rarity, "legendary") == 0)
			array[i].color = COLOR_ITEM_LEGENDARY;
		else
		{
			logger("invalid rarity (weapon)");
			end_ncurses(ERROR_JSON_PARSE);
		}

		array[i].data.weapon_data.damage = my_strdup(damage->valuestring);
		array[i].data.weapon_data.damage_type = my_strdup(damage_type->valuestring);

		int len = cJSON_GetArraySize(properties);
		if (len > 0)
			array[i].properties = my_calloc((len + 1) * sizeof(char*)); // NULL terminate
		else
			array[i].properties = NULL;

		cJSON *prop = NULL;
		int j = 0;
		cJSON_ArrayForEach(prop, properties)
		{
			if (!cJSON_IsString(prop))
			{
				logger("Weapon property not a string");
				end_ncurses(ERROR_JSON_PARSE);
			}
			array[i].properties[j++] = my_strdup(prop->valuestring);
		}
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
		logger("cJSON_Parse failed");
		end_ncurses(errno);
	}
	cJSON *name = cJSON_GetObjectItemCaseSensitive(json, "name");
	cJSON *level = cJSON_GetObjectItemCaseSensitive(json, "level");
	cJSON *terrain = cJSON_GetObjectItemCaseSensitive(json, "terrain");
	cJSON *mech = cJSON_GetObjectItemCaseSensitive(json, "mech");
	cJSON *items = cJSON_GetObjectItemCaseSensitive(json, "items");
	cJSON *creatures = cJSON_GetObjectItemCaseSensitive(json, "creatures");

	t_area *area = my_calloc(sizeof(*area));
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
	area->cells = my_calloc(AREA(area) * sizeof(*(area->cells)));
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
