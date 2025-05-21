#ifndef AREA_H
#define AREA_H

typedef struct s_cell t_cell;
typedef struct s_node t_node;
typedef struct s_creature t_creature;

typedef struct s_area
{
	int		level;
    char    *name;
	int		height;
	int		width;
	t_cell	*cells;
}	t_area;

t_cell *get_player_cell(void);
t_creature *get_player(void);
int	get_player_index(void);
t_node *get_interactables(int flags);
int get_creature_index(t_creature *creature);
t_cell *get_creature_cell(t_creature *creature);

#endif
