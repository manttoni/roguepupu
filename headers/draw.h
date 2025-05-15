#ifndef DRAW_H
#define DRAW_H

#include "cell.h"
#include "area.h"

void draw_cell(int y, int x, t_cell *cell);
void draw_area(t_area *area);
short pair_id(t_cell *cell);

#endif
