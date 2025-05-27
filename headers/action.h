#ifndef ACTION_H
#define ACTION_H

#include "terrain.h"
#include "cell.h"

typedef struct s_area t_area;
typedef struct s_cell t_cell;
typedef struct s_creature t_creature;

/* Actions are separated into pc_action.c and npc_action.c and action.c for universals */

/* PC ACTIONS, WITH SELECT_TARGET UI */
int pc_act(void);

/* NPC ACTIONS, USES AI */
int npc_act(void);

/* UNIVERSAL ACTIONS */
void act_attack(t_creature *attacker, t_creature *defender);
int act_move(e_direction dir, t_cell *src);

#endif
