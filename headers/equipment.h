#ifndef EQUIPMENT_H
#define EQUIPMENT_H

typedef struct s_creature t_creature;
typedef struct s_item t_item;

typedef struct s_weapon_set
{
	t_item *weapon;
	t_item *offhand;
}	t_weapon_set;

typedef struct s_equipped
{
	t_weapon_set	weapon_set;
	t_weapon_set	other_set;
	t_item			*armor;
}	t_equipped;

void swap_weapon_set(t_creature *creature);
int unequip(t_creature *creature, t_item *item);
int equip(t_creature *creature, t_item *item);
t_item *get_armor(t_creature *creature);
void set_armor(t_creature *creature, t_item *armor);
t_item *get_weapon(t_creature *creature);
void set_weapon(t_creature *creature, t_item *weapon);
t_item *get_offhand(t_creature *creature);
void set_offhand(t_creature *creature, t_item *weapon);
int is_dual_wielding(t_creature *creature);
int is_equipped(t_creature *creature, t_item *item);

#endif
