#ifndef DRAW_H
#define DRAW_H

#include "cell.h"
#include "area.h"

void visual_effect(t_creature *creature, chtype effect);
void draw_cell(int y, int x, t_cell *cell);
void draw_area(void);

#endif
