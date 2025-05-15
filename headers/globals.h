#ifndef GLOBALS_H
#define GLOBALS_H

#include <ncurses.h>
#include "area.h"

#define BLOCKING_TERRAIN "DC#"
#define INTERACTABLE_TERRAIN "DCR"
#define INTERACTABLE_MECH "LTS"
#define ENEMY_CHARS "qwertyuiopasdfghjklzxcvbnm"
#define PLAYER_CHAR "@"

#define CLOSED_CHARS "DCR"

#define CREATURE_CHARS (ENEMY_CHARS PLAYER_CHAR)
#define ITEM_CHARS "W"
#define MECH_CHARS "LTS"
#define TERRAIN_CHARS "#.DCR"

/* For scanning */
#define VISIBLE 1
#define NEIGHBOR 2
#define ENEMY 4
#define LOCKED 8
#define CLOSED 16
#define TRAPPED 32
#define ITEM 64

#define PLAYER_EXAMINE (VISIBLE)
#define PLAYER_OPEN (NEIGHBOR | CLOSED)
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
#define DUNGEON "maps/dungeon.json"

/* Highlights */
#define REVERSE 1 // highlight stuff in scnning mode
#define SELECTED 2 // selected highlighted thing

/* Colors */
#define PAIR_ID(fg, bg) ((fg) * 256 + (bg))
#define SELECTED_HIGHLIGHT 1//(PAIR_ID(COLOR_RED, COLOR_BLACK))

/* Damage related */
#define FATAL 1
#define STUN 2

#endif
