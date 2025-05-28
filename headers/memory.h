#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>
void *my_calloc(size_t count, size_t size);
char *my_strdup(char *str);
void my_free(void *ptr);

#endif
