// Copyright: https://git.bounceme.net/hex0x0000/GameOfLife-C/src/branch/main/LICENSE
#include "settings.h"

Settings *settings_default()
{
    Settings *settings = malloc(sizeof(Settings));
    settings->zoom_sensibility = 5;
    settings->show_coords = true;
    settings->show_grid = true;
    settings->min_rect_size = 50;
    settings->max_rect_size = 200;
    return settings;
}
