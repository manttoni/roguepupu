#include "mech.h"
#include "utils.h"
#include "die.h"
#include "globals.h"
#include <stdlib.h>

char *mech_string(t_mech *mech)
{
	int len = 15 + strlen(mech->name);
	char *buf = my_calloc(len + 1);
	snprintf(buf, len + 1, "{%d}%s{reset}", mech->color, mech->name);
	return buf;
}

t_mech *new_mech(char ch, int area_level)
{
	if (strchr(MECH_CHARS, ch) == NULL)
		return NULL;
	t_mech *mech = my_calloc(sizeof(*mech));
	mech->ch = ch;
	if (strchr("LS", ch) != NULL)
		mech->lock = area_level;
	if (strchr("TS", ch) != NULL)
		mech->trap = area_level;
	mech->name = "Foreign contraption";
	mech->color = COLOR_WHITE;
	return mech;
}
