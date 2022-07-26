// Copyright: https://git.bounceme.net/hex0x0000/GameOfLife-C/src/branch/main/LICENSE
#ifndef __GUI_H__
#define __GUI_H__

#include <SDL2/SDL.h>
#include "settings.h"

typedef struct GUI
{
    SDL_Texture *upper_text;
    SDL_Texture *bottom_text;
    SDL_Rect upper_text_rect;
    SDL_Rect bottom_text_rect;
    Settings *settings;
} GUI;

#endif