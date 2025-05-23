#include "file_reader.h"
#include "area.h"
#include "globals.h"
#include "interface.h"
#include "color.h"
#include "cell.h"
#include <stdlib.h>
#include <ncurses.h>

int win_width(WINDOW *win)
{
	int y, x;
	getmaxyx(win, y, x);
	(void) y;
	return x;
}

int win_height(WINDOW *win)
{
	int y, x;
	getmaxyx(win, y, x);
	(void) x;
	return y;
}

int min(int a, int b)
{
	if (a < b)
		return a;
	return b;
}

int max(int a, int b)
{
	if (a > b)
		return a;
	return b;
}

void *my_calloc(size_t size)
{
	void *mem = malloc(size);
	if (mem == NULL)
	{
		logger("Malloc failed, size: %zu", size);
		end_ncurses(errno);
	}
	memset(mem, 0, size);
	return mem;
}

char *my_strdup(char *str)
{
	char *dup = strdup(str);
	if (dup == NULL)
	{
		logger("strdup failed: %s", str);
		end_ncurses(errno);
	}
	return dup;
}

void init_color_pairs(void)
{
	init_pair(COLOR_PAIR_RED, COLOR_RED, COLOR_BLACK);
}

void init_ncurses(void)
{
	initscr();
	curs_set(0);
	if (!has_colors() || !can_change_color())
		end_ncurses(1);
	start_color();
	use_default_colors();
	noecho();
	cbreak();
	init_color_pairs();
	keypad(stdscr, TRUE);
	refresh();
}

void end_ncurses(int exit_value)
{
	logger("exit_value: %d", exit_value);
	if (exit_value != 0)
	{
		print_log("Error occurred. Press any key to continue. Read log/debug.log.");
		getch();
	}
	endwin();
	exit(exit_value);
}

int count_char(char *str, char c)
{
	int count = 0;
	while (*str != '\0')
	{
		if (*str == c)
			count++;
		str++;
	}
	return count;
}

void handle_segfault(int sig)
{
	logger("segfault");
	end_ncurses(sig);
}

/* LINKED LIST STUFF */
t_node *new_node(void *data)
{
	t_node *node = my_calloc(sizeof(t_node));
	node->data = data;
	return node;
}

void add_node_last(t_node **list, t_node *add)
{
	if (*list == NULL)
	{
		*list = add;
		return;
	}
	t_node *last = *list;
	while (last->next != NULL)
		last = last->next;
	last->next = add;
}

void remove_node(t_node **list, t_node *remove)
{
	if (remove == NULL)
		return;
	if (*list == remove)
	{
		*list = remove->next;
		free(remove);
		return;
	}
	t_node *prev = *list;
	while (prev != NULL && prev->next != remove)
		prev = prev->next;
	prev->next = remove->next;
	free(remove);
}

void list_clear(t_node **list)
{
	while (*list != NULL)
		remove_node(list, *list);
}

t_node *copy_list(t_node *list)
{
	t_node *copy = NULL;
	while (list != NULL)
	{
		add_node_last(&copy, new_node(list->data));
		list = list->next;
	}
	return copy;
}

int list_len(t_node *list)
{
	int len = 0;
	while (list != NULL)
	{
		len++;
		list = list->next;
	}
	return len;
}

t_node *get_node(t_node *list, int i)
{
	int n = 0;
	while (list != NULL)
	{
		if (i == n)
			return list;
		list = list->next;
		n++;
	}
	return NULL;
}

t_node *get_node_data(t_node *list, void *data)
{
	while (list != NULL)
	{
		if (list->data == data)
			return list;
		list = list->next;
	}
	return NULL;
}

static void node_swap_data(t_node *a, t_node *b)
{
	void *temp = a->data;
	a->data = b->data;
	b->data = temp;
}

int compare_distance_player(t_node *a, t_node *b)
{
	t_cell *player = get_player_cell();
	return distance((t_cell *) a->data, player) > distance((t_cell *) b->data, player);
}

// bubble sort
void list_sort(t_node *list, int (*compare)(t_node *a, t_node *b))
{
	int len = list_len(list);
	for (int i = 0; i < len - 1; ++i)
	{
		t_node *node = list;
		for (int j = 0; j < len - i - 1; ++j)
		{
			if (compare(node, node->next) == 1)
				node_swap_data(node, node->next);
			node = node->next;
		}
	}
}
