#ifndef DIE_H
#define DIE_H

typedef enum
{
	SLASHING,
	PIERCING,
	BLUDGEONING,
	FIRE,
	COLD,
	LIGHTNING,
	THUNDER,
	ACID,
	POISON,
	RADIANT,
	NECROTIC,
	PSYCHIC,
	FORCE,
	HEALING
}	e_damage_type;

typedef struct s_die
{
	e_damage_type	type;
	int				sides;
}	t_die;

int throw_die(t_die die);
t_die *new_dice(e_damage_type type, char *die);

#endif
