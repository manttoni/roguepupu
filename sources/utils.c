#include <stdlib.h>
#include <ncurses.h>
#include "../headers/file_reader.h"

void *my_malloc(size_t size)
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


