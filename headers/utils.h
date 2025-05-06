#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include "file_reader.h"

void *my_malloc(size_t size);
void init_ncurses();
void end_ncurses(int exit_value);
int count_char(char *str, char c);

#endif
