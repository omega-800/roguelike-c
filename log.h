#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdarg.h>

#define LOGFILE "logfile.txt"

void log_msg(const char *filename, const char *msg, ...);
void clear_log(const char *filename);

#endif
