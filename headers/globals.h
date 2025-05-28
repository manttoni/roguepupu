#ifndef GLOBALS_H
#define GLOBALS_H

#include "area.h"
#include "entities.h"
#include "fungus.h"
#include "cell.h"
#include <ncurses.h>

typedef struct s_item t_item;

/* Fungi */
#define FUNGAL_DENSITY 10 // spawn chance
#define FUNGUS_SPAWN_SPACE 2 // how much space fo space mushrooms need

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
#define CHAR_CORPSE 'C'
#define CHAR_DOOR 'D'
#define CHAR_LOOT 'L'
#define CHAR_WALL '#'
#define CHAR_FLOOR '.'
#define CHAR_WET '~'
#define CHAR_PLAYER '@'
#define CHAR_UNKNOWN '?'

#define CHAR_BLOCK_VISION "$D#"
#define FUNGUS_CHARS "$*%"
#define FUNGUS_BLOCKING "$"

#define TERRAIN_BLOCKING "D#"
#define TERRAIN_CLOSED "DL"
#define TERRAIN_INTERACTABLE (TERRAIN_CLOSED)
#define TERRAIN_CONTAINER "L"

#define CREATURE_CHARS "@qwertyuiopasdfghjklzxcvbnm"

#define TERRAIN_CHARS "#.DL"

#define CELL_GHOST_CHARS "#DC"

/* Vision related */
#define VISION_NONE 0
#define VISION_GHOST 1 // was seen before
#define VISION_FULL 3

/* For scanning and targeting */
#define TARGET_INTERACTABLE (1 << 0)
#define TARGET_LIGHT_SOURCE (1 << 1)
#define TARGET_VISIBLE (1 << 2)
#define TARGET_CREATURE (1 << 3)

#define RANGE_MELEE (1 << 10)
#define RANGE_SHORT (1 << 11)
#define RANGE_LONG (1 << 12)

#define FACTION_PLAYER (1 << 20)
#define FACTION_GOBLIN (1 << 21)
#define FACTION_TRADER (1 << 22)

#define ATTACK_FLAGS(creature) (TARGET_VISIBLE | enemy_factions(creature) | get_attack_range(creature))

#define NEIGHBOR_DIRECTIONS 8

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

#define WEAPON_FILE "json/items/weapon.json"
#define POTION_FILE "json/items/potion.json"
#define REAGENT_FILE "json/items/reagent.json"
#define ARMOR_FILE "json/items/armor.json"

#define GOBLIN_FILE "json/creatures/goblin.json"

#define FUNGUS_FILE "json/fungus/fungus.json"

/* Color pairs */
#define COLOR_PAIR_RED 1
#define NEXT_FREE 2

/* Colors */
#define COLOR_NORMAL 0
#define COLOR_GREYSCALE (1 << 0)

#define DARK_FACTOR 0.5

#define COLOR_ID(r,g,b) (16 + 36 * (r) + 6 * (g) + (b))
#define COLOR_ITEM_COMMON COLOR_ID(2,2,2)
#define COLOR_ITEM_UNCOMMON COLOR_ID(0,2,0)
#define COLOR_ITEM_RARE COLOR_ID(0,0,2)
#define COLOR_ITEM_VERY_RARE COLOR_ID(2,0,0)
#define COLOR_ITEM_LEGENDARY COLOR_ID(1,2,3)

#define COLOR_CAVE_WALL COLOR_ID(1,2,1)
#define COLOR_CAVE_FLOOR COLOR_ID(1,2,2)
#define COLOR_METAL_DOOR COLOR_ID(3,1,1)
#define COLOR_METAL_CHEST COLOR_ID(3,1,1)

#define COLOR_GOBLIN COLOR_ID(0,1,0)

#define COLOR_CELL_GHOST COLOR_ID(1,1,1)

#define MY_COLOR_BLACK COLOR_ID(0, 0, 0)
#define MY_COLOR_BROWN COLOR_ID(3, 2, 1)
#define MY_COLOR_PALE_BLUE COLOR_ID(3, 4, 5)
#define MY_COLOR_SOFT_BLUE COLOR_ID(2, 3, 5)

/* Damage related */
#define DAMAGE_FATAL 1

/* Inventory stuff */
#define INVENTORY_PLAYER 1
#define INVENTORY_LOOT 2
#define INVENTORY_TRADE 3

/* Actual globals */
extern t_area *g_area;

extern t_item_group	*g_item_groups;
extern size_t g_item_group_count;

extern t_creature_group	*g_creature_groups;
extern size_t g_creature_group_count;

extern t_fungus *g_fungi;
extern size_t g_fungus_count;

extern e_direction g_dirs[];

extern t_node *g_allocations;
#endif
