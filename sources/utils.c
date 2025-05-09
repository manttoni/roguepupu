#include <stdlib.h>
#include <ncurses.h>
#include "../headers/file_reader.h"
#include "../headers/area.h"

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

void init_ncurses()
{
	initscr();
	curs_set(0);
	start_color();
	noecho();
	cbreak();
	keypad(stdscr, TRUE);
    logger("ncurses initialized");
}

void end_ncurses(int exit_value)
{
	clear();
    if (exit_value != 0)
    {
        char *log = read_file("logs/debug.log");
        mvprintw(0,0,"%s", log);
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
t_node *init_node(void *data)
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
	if (*list == remove)
	{
		*list = remove->next;
		return;
	}
	t_node *prev = *list;
	while (prev->next != remove)
		prev = prev->next;
	prev->next = remove->next;
	free(remove);
}

void list_clear(t_node **list)
{
	while (*list)
		remove_node(list, *list);
}
