#ifndef ACTION_H
#define ACTION_H

#include "terrain.h"
#include "cell.h"

typedef struct s_area t_area;
typedef struct s_cell t_cell;
typedef struct s_creature t_creature;

typedef enum
{
	ACTION_PICK_UP,
	ACTION_PASS,
	ACTION_MOVE_UPLEFT,
	ACTION_MOVE_UPRIGHT,
	ACTION_MOVE_DOWNLEFT,
	ACTION_MOVE_DOWNRIGHT,
	ACTION_MOVE_UP,
	ACTION_MOVE_DOWN,
	ACTION_MOVE_LEFT,
	ACTION_MOVE_RIGHT,
	ACTION_EXAMINE,
	ACTION_OPEN,
	ACTION_UNLOCK,
	ACTION_ATTACK,
	ACTION_INVENTORY,
	ACTION_NONE
}	e_action;

/* Actions are separated into pc_action.c and npc_action.c and action.c for universals */

/* PC ACTIONS, WITH SCAN UI */
int pc_act(void);
int pc_pick_up(void);
int pc_examine(void);
int pc_open(void);
int pc_unlock(void);
int pc_attack(void);

/* NPC ACTIONS, USES AI */
int npc_act(void);

/* UNIVERSAL ACTIONS */
int act_attack(t_creature *attacker, t_creature *defender);
int act_move(e_direction dir, t_cell *src);

#endif
