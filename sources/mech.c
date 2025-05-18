#include "mech.h"
#include "utils.h"
#include "die.h"
#include "globals.h"
#include <stdlib.h>

int is_trapped(t_mech *mech)
{
	return mech != NULL && mech->trap > 0;
}

int is_locked(t_mech *mech)
{
	return mech != NULL && mech->lock > 0;
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
	return mech;
}
