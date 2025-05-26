#include "mech.h"
#include "utils.h"
#include "globals.h"
#include <stdlib.h>

t_mech *new_mech(char ch)
{
	if (strchr(MECH_CHARS, ch) == NULL)
		return NULL;
	t_mech *mech = my_calloc(1, sizeof(*mech));
	mech->ch = ch;
	if (strchr("LS", ch) != NULL)
		mech->lock = 1;
	if (strchr("TS", ch) != NULL)
		mech->trap = 1;
	mech->name = "Foreign contraption";
	mech->color = COLOR_WHITE;
	return mech;
}
