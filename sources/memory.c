#include "memory.h"
#include "utils.h"
#include "globals.h"
#include <stddef.h>

void *my_calloc(size_t count, size_t size)
{
	void *mem = calloc(count, size);
	if (mem == NULL)
	{
		logger("Allocation failed, count=%zu, size: %zu", count, size);
		end_ncurses(errno);
	}
	t_node *new_node = malloc(sizeof(*new_node));
	if (new_node == NULL)
		end_ncurses(errno);
	new_node->data = mem;
	new_node->next = NULL;
	add_node_first(&g_allocations, new_node);
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
	t_node *new_node = malloc(sizeof(*new_node));
	if (new_node == NULL)
		end_ncurses(errno);
	new_node->data = dup;
	new_node->next = NULL;
	add_node_first(&g_allocations, new_node);
	return dup;
}

void my_free(void *ptr)
{
	t_node *alloc_node = get_node_data(g_allocations, ptr);
	free(ptr);
	remove_node(&g_allocations, alloc_node);
}

void free_allocations(void)
{
	t_node *ptr = g_allocations;
	while (ptr != NULL)
	{
		free(ptr->data);
		ptr = ptr->next;
	}
	list_clear(&g_allocations);
}
