#ifndef MEMORY_H
#define MEMORY_H

#include "game.h"
#include "area.h"
#include "cell.h"
#include "creature.h"
#include "utils.h"
#include "item.h"
#include "terrain.h"
#include "mech.h"

void free_globals(void);
void free_game(t_game *game);
void free_area(t_area *area);
void free_cell(t_cell *cell);
void free_creature(t_creature *creature);
void free_inventory(t_node *inventory);
void free_item(t_item *item);
void free_terrain(t_terrain *terrain);
void free_mech(t_mech *mech);

#endif
