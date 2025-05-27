#include "creature.h"

int get_AC(t_creature *creature)
{
	int AC = 10;
	AC += dexmod(creature);
	return AC;
}

static int mod(int val)
{
	return (val - 10) / 2;
}

int strmod(t_creature *creature)
{
	return mod(creature->abilities.strength);
}

int dexmod(t_creature *creature)
{
	return mod(creature->abilities.dexterity);
}

int conmod(t_creature *creature)
{
	return mod(creature->abilities.constitution);
}

int intmod(t_creature *creature)
{
	return mod(creature->abilities.intelligence);
}

int wismod(t_creature *creature)
{
	return mod(creature->abilities.wisdom);
}

int chamod(t_creature *creature)
{
	return mod(creature->abilities.charisma);
}


