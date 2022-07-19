#ifndef __GRID_RENDER_H__
#define __GRID_RENDER_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdint.h>
#include "dynamic_matrix.h"
#include "linkedlist.h"

#define INITIAL_RECT_SIZE 100

typedef struct Settings
{
    int zoom_sensibility;
    bool show_coords;
    bool can_show_coords;
    uint16_t min_rect_size;
    uint16_t max_rect_size;
} Settings;

typedef struct Tile
{
    bool is_initialized;
    SDL_Texture *text_coords;
    SDL_Rect rect;
    int64_t x;
    int64_t y;
} Tile;

typedef struct GridRender
{
    DynMatrix *matrix;
    TTF_Font *font;
    SDL_Renderer *renderer;
    uint16_t rect_size;
    Settings settings;
} GridRender;

GridRender *GR_New(SDL_Renderer *renderer, TTF_Font *font);
int GR_UpdateScreenSize(GridRender *gr, int length, int width);
int GR_ChangeVisual(GridRender *gr, int8_t delta_x, int8_t delta_y);
int GR_ChangeZoom(GridRender *gr, int new_zoom, int length, int width);
int GR_Render(GridRender *gr, LinkedList *alive_cells);
void GR_Free(GridRender *gr);

#endif