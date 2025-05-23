#ifndef GLOBALS_H
#define GLOBALS_H

#include "area.h"
#include <ncurses.h>

typedef struct s_item t_item;

/* Dice rolls */
#define ROLL_DISADVANTAGE -1
#define ROLL_NORMAL 0
#define ROLL_ADVANTAGE 1

/* Area and cell sizes etc */
#define CELL_SIZE 5
#define AREA(area) ((area)->width * (area)->height)

/* Exit codes */
#define QUIT_GAME 1
#define END_TURN 2
#define ERROR_JSON_PARSE 2

/* char */
#define TERRAIN_BLOCKED "DC#"
#define TERRAIN_CLOSED "DCR"
#define TERRAIN_DOOR "D"
#define TERRAIN_INTERACTABLE (TERRAIN_CLOSED)
#define TERRAIN_CONTAINER "CR"
#define TERRAIN_WALLFLOOR "#."

#define MECH_INTERACTABLE "LTS"
#define MECH_CHARS "LTS"

#define ENEMY_CHARS "qwertyuiopasdfghjklzxcvbnm"
#define PLAYER_CHAR "@"
#define CREATURE_CHARS (ENEMY_CHARS PLAYER_CHAR)

#define ITEM_CHARS "WP"
#define TERRAIN_CHARS "#.DCR"

#define CELL_GHOST_CHARS "#DCR"

/* Vision related */
#define VISION_NONE 0 // no line of sight and was never seen before
#define VISION_GHOST 1 // no line of sight but was seen before
#define VISION_DIM 2 // outside darkvision range
#define VISION_BRIGHT 3 // inside darkvision range

/* For scanning and targeting */
#define SCAN_VISIBLE (1 << 0)
#define SCAN_NEIGHBOR (1 << 1)
#define SCAN_ENEMY (1 << 2)
#define SCAN_LOCKED (1 << 3)
#define SCAN_CLOSED (1 << 4)
#define SCAN_TRAPPED (1 << 5)
#define SCAN_ITEM (1 << 6)

#define FACTION_PLAYER (1 << 0)
#define FACTION_GOBLIN (2 << 1)

#define FACTION_ALL (FACTION_PLAYER | FACTION_GOBLIN)

#define ENEMY_FACTION(faction) (FACTION_ALL & ~faction)

#define PLAYER_EXAMINE (SCAN_VISIBLE)
#define PLAYER_OPEN (SCAN_NEIGHBOR | SCAN_CLOSED)
#define PLAYER_UNLOCK (SCAN_NEIGHBOR | SCAN_LOCKED)

#define MELEE_ATTACK (SCAN_NEIGHBOR | SCAN_ENEMY)
#define RANGED_ATTACK (SCAN_VISIBLE | SCAN_ENEMY)

/* Enemy "AI" */
#define AI_IDLE (1 << 0)
#define AI_WANDER (1 << 1)
#define AI_PURSUE (1 << 2)
#define AI_FLEE (1 << 3)

#define AI_CRAZY_GOBLIN (AI_WANDER | AI_FLEE | AI_PURSUE)

/* Keys */
#define ESCAPE 27
#define ENTER '\n'

/* Files */
#define MAP_DUNGEON "json/maps/dungeon.json"
#define MAP_CAVES "json/maps/caves.json"

#define WEAPON_FILE "json/items/weapons.json"
#define POTION_FILE "json/items/potions.json"
#define ARMOR_FILE "json/items/armor.json"

/* Color pairs */
#define COLOR_PAIR_RED 1
#define NEXT_FREE 2

/* Colors */
#define COLOR_NORMAL 0
#define COLOR_DARKER (1 << 0)
#define COLOR_GREYSCALE (1 << 1)

#define DARK_FACTOR 0.5

#define COLOR_ID(r,g,b) (16 + 36 * r + 6 * g + b)
#define COLOR_ITEM_COMMON (COLOR_ID(2,2,2))
#define COLOR_ITEM_UNCOMMON (COLOR_ID(0,2,0))
#define COLOR_ITEM_RARE (COLOR_ID(0,0,2))
#define COLOR_ITEM_VERY_RARE (COLOR_ID(2,0,0))
#define COLOR_ITEM_LEGENDARY (COLOR_ID(1,2,3))

#define COLOR_CAVE_WALL (COLOR_ID(1,2,1))
#define COLOR_CAVE_FLOOR (COLOR_ID(1,2,2))
#define COLOR_METAL_DOOR (COLOR_ID(3,1,1))
#define COLOR_METAL_CHEST (COLOR_ID(3,1,1))

#define COLOR_CREATURE_GOBLIN (COLOR_ID(0,1,0))

#define COLOR_CELL_GHOST (COLOR_ID(1,1,1))

/* Damage related */
#define DAMAGE_FATAL 1

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
