#ifndef STATUS_H
#define STATUS_H

#include "../headers/cell.h"
#include "../headers/creature.h"

int apply_status_effects(t_cell *cell);
int bleed(t_creature *creature);

#endif
