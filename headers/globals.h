#ifndef GLOBALS_H
#define GLOBALS_H

#include <ncurses.h>
#include "area.h"

#define BLOCKING_TERRAIN "0O#"
#define INTERACTABLE_TERRAIN "O0"
#define ENEMY_CREATURES "qwertyuiopasdfghjklzxcvbnm"

/* For scanning */
#define VISIBLE 1
#define NEIGHBOR 2
#define ENEMY 4
#define LOCKED 8
#define UNLOCKED 16

#define PLAYER_EXAMINE (VISIBLE)
#define PLAYER_OPEN (NEIGHBOR | UNLOCKED)
#define PLAYER_UNLOCK (NEIGHBOR | LOCKED)
#define PLAYER_MELEE_ATTACK (NEIGHBOR | ENEMY)
#define PLAYER_RANGED_ATTACK (VISIBLE | ENEMY)

/* Enemy "AI" */
#define WANDER 1 // enemy should wander around
#define PURSUE 2 // if sees player should pursue
#define FLEE 4 // should flee if low health

#define CRAZY_GOBLIN (WANDER | FLEE | PURSUE)

/* Keys */
#define ESCAPE 27
#define ENTER '\n'

/* Calculations */
#define AREA(area) ((area)->width * (area)->height)

/* Maps */
#define DUNGEON "maps/dungeon.txt"

/* Highlights */
#define REVERSE 1 // highlight stuff in scnning mode
#define SELECTED 2 // selected highlighted thing


#endif
