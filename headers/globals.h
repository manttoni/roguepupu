#ifndef GLOBALS_H
#define GLOBALS_H

#include "area.h"
#include <ncurses.h>

typedef struct s_item t_item;

#define QUIT_GAME 1
#define END_TURN 2
#define ERROR_JSON_PARSE 2

#define TERRAIN_BLOCKED "DC#"
#define TERRAIN_CLOSED "DCR"
#define TERRAIN_DOOR "D"
#define TERRAIN_INTERACTABLE (TERRAIN_CLOSED)
#define TERRAIN_CONTAINER "CR"

#define MECH_INTERACTABLE "LTS"
#define MECH_CHARS "LTS"

#define ENEMY_CHARS "qwertyuiopasdfghjklzxcvbnm"
#define PLAYER_CHAR "@"
#define CREATURE_CHARS (ENEMY_CHARS PLAYER_CHAR)

#define ITEM_CHARS "WP"
#define TERRAIN_CHARS "#.DCR"

/* For scanning */
#define SCAN_VISIBLE 1
#define SCAN_NEIGHBOR 2
#define SCAN_ENEMY 4
#define SCAN_LOCKED 8
#define SCAN_CLOSED 16
#define SCAN_TRAPPED 32
#define SCAN_ITEM 64

#define FACTION_PLAYER 1
#define FACTION_GOBLIN 2

#define FACTION_ALL (FACTION_PLAYER | FACTION_GOBLIN)

#define ENEMY_FACTION(faction) (FACTION_ALL & ~faction)

#define PLAYER_EXAMINE (SCAN_VISIBLE)
#define PLAYER_OPEN (SCAN_NEIGHBOR | SCAN_CLOSED)
#define PLAYER_UNLOCK (SCAN_NEIGHBOR | SCAN_LOCKED)

#define MELEE_ATTACK (SCAN_NEIGHBOR | SCAN_ENEMY)
#define RANGED_ATTACK (SCAN_VISIBLE | SCAN_ENEMY)

/* Enemy "AI" */
#define AI_IDLE 0
#define AI_WANDER 1
#define AI_PURSUE 2
#define AI_FLEE 4

#define AI_CRAZY_GOBLIN (AI_WANDER | AI_FLEE | AI_PURSUE)

/* Keys */
#define ESCAPE 27
#define ENTER '\n'

/* Calculations */
#define AREA(area) ((area)->width * (area)->height)

/* Files */
#define MAP_DUNGEON "json/maps/dungeon.json"
#define MAP_CAVES "json/maps/caves.json"

#define WEAPON_FILE "json/items/weapons.json"
#define POTION_FILE "json/items/potions.json"
#define ARMOR_FILE "json/items/armor.json"

/* Highlights */
#define HIGHLIGHT_REVERSE 1 // highlight stuff in scnning mode
#define HIGHLIGHT_SELECTED 2 // selected highlighted thing

/* Color pairs */
#define COLOR_PAIR_RED 1
#define NEXT_FREE 2

/* Colors */
#define COLOR_ID(r,g,b) (16 + 36 * r + 6 * g + b)
#define COLOR_ITEM_COMMON (COLOR_ID(2,2,2))
#define COLOR_ITEM_UNCOMMON (COLOR_ID(0,2,0))
#define COLOR_ITEM_RARE (COLOR_ID(0,0,2))
#define COLOR_ITEM_VERY_RARE (COLOR_ID(2,0,0))
#define COLOR_ITEM_LEGENDARY (COLOR_ID(1,2,3))

#define COLOR_CREATURE_GOBLIN (COLOR_ID(0,1,0))


/* Damage related */
#define DAMAGE_FATAL 1
#define DAMAGE_STUN 2

/* Weapon properties */
#define NONE 0
#define LIGHT 1
#define FINESSE 2
#define TWO_HANDED 4
#define THROWN 8
#define VERSATILE 16
#define RANGED 32

/* Inventory stuff */
#define INVENTORY_PLAYER 1
#define INVENTORY_LOOT 2
#define INVENTORY_TRADE 3

extern t_item *g_weapons;
extern int g_weapon_count;
extern t_item *g_potions;
extern int g_potion_count;
extern t_area *g_area;

#endif
