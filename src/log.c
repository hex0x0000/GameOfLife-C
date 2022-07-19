#include "log.h"

int LOG_INIT()
{
#ifdef FILE_LOGS
    logs_file = fopen(FILE_LOGS, "a");
    if (!logs_file)
        return 1;
#endif
    return 0;
}

void _log(FILE *stream, const char *type, const char *fmt, ...)
{
    time_t now;
    char buffer[25];
    va_list args;

    va_start(args, fmt);
    time(&now);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", gmtime(&now));
#if defined MUTEX && defined FILE_LOGS
    pthread_mutex_lock(&MUTEX);
#endif
    fprintf(stream, "[%s] [%s] ", buffer, type);
    vfprintf(stream, fmt, args);
    fputc('\n', stream);
#if defined MUTEX && defined FILE_LOGS
    pthread_mutex_unlock(&MUTEX);
#endif
    va_end(args);
}

void LOG_CLOSE()
{
#ifdef FILE_LOGS
    fclose(logs_file);
#endif
}
