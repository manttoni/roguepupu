#include "../headers/cJSON.h"
#include "../headers/cell.h"
#include "../headers/globals.h"
#include "../headers/utils.h"
#include <string.h>
#include <errno.h>

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
	logger("Name is %s", name->valuestring);

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

	return area;
}
