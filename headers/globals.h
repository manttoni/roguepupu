#ifndef GLOBALS_H
#define GLOBALS_H

#define INTERACTABLE "O0"

/* For scanning */
#define VISIBLE 1
#define NEIGHBOR 2

/* Keys */
#define ESCAPE 27

/* Calculations */
#define AREA(area) ((area)->width * (area)->height)

/* Maps */
#define DUNGEON "maps/dungeon.txt"

/* Highlights */
#define REVERSE 1 // highlight stuff in scnning mode
#define SELECTED 2 // selected highlighted thing

#endif
