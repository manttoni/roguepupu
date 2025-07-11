#include "creature.h"

int get_AC(t_creature *creature)
{
	int AC = max(10, get_ac(get_armor(creature)));;
	AC += dexmod(creature);
	AC += get_ac(get_weapon(creature));
	AC += get_ac(get_offhand(creature));
	return AC;
}

static int mod(int val)
{
	return val / 2 - 5;
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


