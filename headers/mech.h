#ifndef MECH_H
#define MECH_H

typedef struct s_mech
{
	char	*name;
	char	ch;
	int		lock;
	int		trap;
}	t_mech;

t_mech *new_mech(char ch, int area_level);
int is_locked(t_mech *mech);
int is_trapped(t_mech *mech);

#endif
