#ifndef FILE_READER_H
#define FILE_READER_H

#include "utils.h"
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>

void init_logger(void);
void logger(const char *format, ...);
char *read_file(char *file);

#endif
