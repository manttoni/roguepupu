#ifndef UTILS_H
#define UTILS_H

#include "file_reader.h"
#include "area.h"
#include "windows.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdnoreturn.h>

noreturn void end_ncurses(int exit_value);

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
void add_node_first(t_node **list, t_node *add);
void add_node_last(t_node **list, t_node *add);
void remove_node(t_node **list, t_node *remove);
void list_clear(t_node **list);
t_node *copy_list(t_node *list);
size_t list_len(t_node *list);
t_node *get_node(t_node *list, int i);
t_node *get_node_data(t_node *list, void *data);
void list_sort(t_node *list, int (*compare)(t_node *a, t_node *b));
int compare_distance_player(t_node *a, t_node *b);
int compare_item_name(t_node *a, t_node *b);
void free_list_data(t_node *list);

/* OTHER STUFF */
void free_allocations(void);
size_t generate_id(void);
char **copy_char_array(char **array);
int resolve_macro(char *str);
char *find_next_of(char *str, char *set);
int min(int a, int b);
int max(int a, int b);
void init_ncurses(void);
void end_ncurses(int exit_value);
int count_char(char *str, char c);
void handle_segfault(int sig);
int win_width(WINDOW *win);
int win_height(WINDOW *win);

#endif
