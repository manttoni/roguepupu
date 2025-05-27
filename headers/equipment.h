#ifndef EQUIPMENT_H
#define EQUIPMENT_H

typedef struct s_creature t_creature;
typedef struct s_item t_item;

typedef struct s_equipped
{
	t_item	*weapon;
	t_item	*offhand;
}	t_equipped;

void unequip(t_creature *creature, t_item *item);
void equip(t_creature *creature, t_item *item);
t_item *get_weapon(t_creature *creature);
void set_weapon(t_creature *creature, t_item *weapon);
t_item *get_offhand(t_creature *creature);
void set_offhand(t_creature *creature, t_item *weapon);
int is_dual_wielding(t_creature *creature);
int is_equipped(t_creature *creature, t_item *item);

#endif
