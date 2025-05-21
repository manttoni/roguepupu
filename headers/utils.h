#ifndef UTILS_H
#define UTILS_H

#include "file_reader.h"
#include "area.h"
#include "windows.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

typedef struct s_coord
{
	int y;
	int x;
}	t_coord;

/* LINKED LIST STUFF */
typedef struct s_node
{
	void			*data;
	struct s_node	*next;
}	t_node;

t_node *new_node(void *data);
void add_node_last(t_node **list, t_node *add);
void remove_node(t_node **list, t_node *remove);
void list_clear(t_node **list);
t_node *copy_list(t_node *list);
int list_len(t_node *list);
t_node *get_node(t_node *list, int i);
t_node *get_node_data(t_node *list, void *data);

/* OTHER STUFF */
int min(int a, int b);
int max(int a, int b);
void *my_calloc(size_t size);
char *my_strdup(char *str);
void init_ncurses(void);
void end_ncurses(int exit_value);
int count_char(char *str, char c);
void handle_segfault(int sig);
int win_width(WINDOW *win);
int win_height(WINDOW *win);

#endif
