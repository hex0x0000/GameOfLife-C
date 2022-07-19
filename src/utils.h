#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdbool.h>

bool precise_sleep(double seconds);
void handle_file_error(const char *filename, int err, bool use_logging);

#endif