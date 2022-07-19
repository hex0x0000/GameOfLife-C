#ifndef __LOG_H__
#define __LOG_H__

#include <stdarg.h>
#include <time.h>
#include <stdio.h>
#include <pthread.h>

#ifdef FILE_LOGS
FILE *logs_file;
#define OUT logs_file
#define ERR_OUT logs_file
#else
#warning -DFILE_LOGS not specified, using stdout and stderr.
#define OUT stdout
#define ERR_OUT stderr
#endif

int LOG_INIT();
void _log(FILE *stream, const char *type, const char *fmt, ...);
void LOG_CLOSE();

#ifdef DEBUG
#define LOG_DEBUG(...) _log(OUT, "DEBUG", __VA_ARGS__)
#else
#define LOG_DEBUG(...)
#endif
#define LOG_INFO(...) _log(OUT, "INFO", __VA_ARGS__)
#define LOG_WARN(...) _log(ERR_OUT, "WARN", __VA_ARGS__)
#define LOG_ERROR(...) _log(ERR_OUT, "ERROR", __VA_ARGS__)
#define LOG_CRITICAL(...) _log(ERR_OUT, "CRITICAL", __VA_ARGS__)


#endif