// Copyright: https://git.bounceme.net/hex0x0000/GameOfLife-C/src/branch/main/LICENSE
#include "utils.h"

#include <errno.h>
#include <stdio.h>
#include "log.h"
#ifdef WIN32
#include <windows.h>
#elif defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
#include <unistd.h>
#include <time.h>
#else
#include <SDL2/SDL.h>
#endif

#ifdef WIN32
#include <windows.h>
// Took from https://gist.github.com/Youka/4153f12cf2e17a77314c
BOOLEAN win_nanosleep(LONGLONG ns)
{
    HANDLE timer;
    LARGE_INTEGER li;
    if (!(timer = CreateWaitableTimer(NULL, TRUE, NULL)))
        return FALSE;
    li.QuadPart = -ns;
    if (!SetWaitableTimer(timer, &li, 0, NULL, NULL, FALSE))
    {
        CloseHandle(timer);
        return FALSE;
    }
    WaitForSingleObject(timer, INFINITE);
    CloseHandle(timer);
    return TRUE;
}
#endif

bool precise_sleep(double seconds)
{
    if (seconds >= 1.0)
        return false;
#ifdef WIN32
    if (!win_nanosleep((LONGLONG)(seconds * 1000000000)))
        return false;
#elif _POSIX_C_SOURCE >= 199309L
    struct timespec req = {0, seconds * 1000000000};
    if (nanosleep(&req, NULL) == -1)
        return false;
#elif _POSIX_C_SOURCE < 199309L || \
    (!(defined _POSIX_C_SOURCE) && defined(__unix__) || (defined(__APPLE__) && defined(__MACH__)))
    if (usleep((unsigned int)(seconds * 1000000)) != 0)
        return false;
#else
    SDL_Delay((Uint32)(seconds * 1000));
#endif
    return true;
}

void handle_file_error(const char *filename, int err, bool use_logging)
{
    switch (err)
    {
    case ENONET:
        if (use_logging)
            LOG_CRITICAL("%s not found.", filename);
        else
            fprintf(stderr, "%s not found.\n", filename);
        break;

    case EEXIST:
        if (use_logging)
            LOG_CRITICAL("%s already exists.", filename);
        else
            fprintf(stderr, "%s already exists.\n", filename);
        break;
    
    case EFBIG:
        if (use_logging)
            LOG_CRITICAL("%s too big.", filename);
        else
            fprintf(stderr, "%s too big.\n", filename);
        break;

    case EMFILE:
    case ENFILE:
        if (use_logging)
            LOG_CRITICAL("Couldn't open %s, too many files open.", filename);
        else
            fprintf(stderr, "Couldn't open %s, too many files open.\n", filename);
        break;
    
    case ETXTBSY:
        if (use_logging)
            LOG_CRITICAL("%s busy.", filename);
        else
            fprintf(stderr, "%s busy.\n", filename);
        break;
    
    case EROFS:
        if (use_logging)
            LOG_CRITICAL("%s is read-only.", filename);
        else
            fprintf(stderr, "%s is read-only.\n", filename);
        break;

    case EACCES:
        if (use_logging)
            LOG_CRITICAL("%s: permission denied.", filename);
        else
            fprintf(stderr, "%s: permission denied.\n", filename);
        break;

    default:
        if (use_logging)
            LOG_CRITICAL("Couldn't open %s: errno %d.", filename, err);
        else
            fprintf(stderr, "Couldn't open %s: errno %d.\n", filename, err);
        break;
    }
}
