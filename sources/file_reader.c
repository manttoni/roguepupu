#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "../headers/utils.h"


void logger(const char *format, ...)
{
    FILE *logfile = fopen("logs/debug.log", "a");
    if (logfile == NULL)
        end_ncurses(errno);

    va_list args;
    va_start(args, format);
    vfprintf(logfile, format, args);
    fprintf(logfile, "\n");
    va_end(args);

    fclose(logfile);
}


char *read_file(char *file)
{
	int fd = open(file, O_RDONLY);
	if (fd < 0)
		end_ncurses(errno);
	size_t len = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);
	char *buf = my_malloc((len + 1) * sizeof(char));
	memset(buf, 0, len + 1);
	if (read(fd, buf, len) < 0)
		end_ncurses(errno);
	close(fd);
	logger("File %s read", file);
	return buf;
}