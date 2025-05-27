#ifndef MECH_H
#define MECH_H

typedef struct s_mech
{
	char	*name;
	char	ch;
	int		lock;
	int		trap;
	short	color;
}	t_mech;

char *mech_string(t_mech *mech);
t_mech *spawn_mech(char ch);

#endif
