#ifndef ITEM_DATA_H
#define ITEM_DATA_H

typedef struct s_reagent_data
{
	char	**properties;
}	t_reagent_data;

typedef struct s_potion_data
{
	char	*effect;
	char	*duration;
}	t_potion_data;

typedef struct s_weapon_data
{
	char	**properties;
	char	*proficiency;
	char	*damage_type;
	char	*damage; // "1d6;1d8" <- has versatile info
}	t_weapon_data;

#endif
