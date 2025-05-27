#ifndef ABILITIES_H
#define ABILITIES_H

typedef struct s_abilities
{
	int strength;
	int dexterity;
	int constitution;
	int intelligence;
	int wisdom;
	int charisma;
}	t_abilities;

int get_AC(t_creature *creature);
int strmod(t_creature *creature);
int dexmod(t_creature *creature);
int conmod(t_creature *creature);
int intmod(t_creature *creature);
int wismod(t_creature *creature);
int chamod(t_creature *creature);
int get_saving_throw(t_creature *creature, char *saving_throw);

#endif
