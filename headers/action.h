#ifndef ACTION_H
#define ACTION_H

#include "terrain.h"

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
	ACTION_NONE
}	e_action;

/* Actions are separated into pc_action.c and npc_action.c and action.c for universals */

/* PC ACTIONS, WITH SCAN UI */
int pc_act(t_area *area);
int pc_pick_up(t_area *area);
int pc_examine(t_area *area);
int pc_open(t_area *area);
int pc_unlock(t_area *area);
int pc_attack(t_area *area);

/* NPC ACTIONS, USES AI */
int npc_act(t_area *area);

/* UNIVERSAL ACTIONS */
int act_attack(t_cell *attacker_cell, t_cell *defender_cell);
int act_move(t_cell *dst, t_cell *src);

#endif
