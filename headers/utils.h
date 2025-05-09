#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include "file_reader.h"
#include "area.h"

typedef struct s_coord
{
	int x;
	int y;
}	t_coord;

/* LINKED LIST STUFF */
typedef struct s_node
{
	void			*data;
	struct s_node	*next;
}	t_node;

t_node *init_node(void *data);
void add_node_last(t_node **list, t_node *add);
void remove_node(t_node **list, t_node *remove);
void list_clear(t_node **list);

void *my_calloc(size_t size);
void init_ncurses();
void end_ncurses(int exit_value);
int count_char(char *str, char c);
void handle_segfault(int sig);

#endif
