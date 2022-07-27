// Copyright: https://git.bounceme.net/hex0x0000/GameOfLife-C/src/branch/main/LICENSE
#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Settings
{
    int zoom_sensibility;
    bool show_coords;
    bool show_grid;
    uint16_t min_rect_size;
    uint16_t max_rect_size;
} Settings;

Settings *settings_default();

#endif