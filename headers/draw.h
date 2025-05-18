#ifndef DRAW_H
#define DRAW_H

#include "cell.h"
#include "area.h"

void cosmetic_effects(t_area *area);
void draw_cell(int y, int x, t_cell *cell);
void draw_area(t_area *area);

#endif
