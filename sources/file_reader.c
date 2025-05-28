#include "utils.h"
#include "time.h"
#include "memory.h"
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

void init_logger(void)
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	logger("\n%d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
}

void logger(const char *format, ...)
{
	FILE *logfile = fopen("logs/debug.log", "a");
	if (logfile == NULL)
	{
		end_ncurses(errno);
	}

	va_list args;
	va_start(args, format);
	vfprintf(logfile, format, args);
	fprintf(logfile, "\n");
	va_end(args);

	fclose(logfile);
}

char *read_file(char *file)
{
	logger("reading file");
	int fd = open(file, O_RDONLY);
	if (fd < 0)
		end_ncurses(errno);
	size_t len = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);
	logger("before mymalloc");
	char *buf = my_calloc(len + 1, sizeof(*buf));
	logger("my_malloc in read_file success");
	if (read(fd, buf, len) < 0)
		end_ncurses(errno);
	close(fd);
	logger("file read");
	return buf;
}
