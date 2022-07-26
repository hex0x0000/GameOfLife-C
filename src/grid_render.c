// Copyright: https://git.bounceme.net/hex0x0000/GameOfLife-C/src/branch/main/LICENSE
#include "grid_render.h"
#include "utils.h"
#include "gol.h"
#include "log.h"
#include <string.h>

SDL_Texture *make_text_coords(SDL_Renderer *renderer,
                              TTF_Font *font,
                              SDL_Rect *rect,
                              int64_t x,
                              int64_t y,
                              int x_pos,
                              int y_pos,
                              int tile_size)
{
    char *coords = calloc(16, sizeof(char));
    if (!coords)
        return NULL;
    if (snprintf(coords, 15, "(%ld, %ld)", x, y) < 0)
        return NULL;
    SDL_Surface *text_surface = TTF_RenderText_Solid(font, coords, (SDL_Color){0, 255, 0, 0});
    if (!text_surface)
        return NULL;
    free(coords);
    SDL_Texture *text = SDL_CreateTextureFromSurface(renderer, text_surface);
    if (!text)
        return NULL;
    rect->x = x_pos * tile_size;
    rect->y = y_pos * tile_size;
    rect->w = text_surface->w;
    rect->h = text_surface->h;
    SDL_FreeSurface(text_surface);
    return text;
}

void tile_free(void *unit)
{
    Tile *tile = unit;
    if (tile->text_coords)
        SDL_DestroyTexture(unit);
    free(tile);
}

GridRender *GR_New(SDL_Renderer *renderer, TTF_Font *font)
{
    GridRender *gr = malloc(sizeof(GridRender));
    if (!gr)
        return NULL;
    Tile init_value = {
        .is_initialized = false,
        .x = 0,
        .y = 0,
        .text_coords = NULL};
    gr->matrix = DM_New(&init_value, sizeof(Tile), tile_free);
    gr->rect_size = INITIAL_RECT_SIZE;
    gr->settings = (Settings){
        .zoom_sensibility = 5,
        .show_coords = true,
        .can_show_coords = true,
        .min_rect_size = 50,
        .max_rect_size = 200};
    gr->font = font;
    gr->renderer = renderer;
    return gr;
}

int GR_UpdateScreenSize(GridRender *gr, int length, int width)
{
    uint64_t matrix_size_x = width / gr->rect_size + 10;
    uint64_t matrix_size_y = length / gr->rect_size + 10;
    if (matrix_size_x == gr->matrix->size_x && matrix_size_y == gr->matrix->size_y)
        return EXIT_SUCCESS;
    if (DM_Realloc(gr->matrix, matrix_size_x, matrix_size_y) == EXIT_FAILURE)
        return EXIT_FAILURE;
    if (!((Tile *)DM_Get(gr->matrix, 0, 0))->is_initialized)
    {
        Tile *tile = DM_Get(gr->matrix, 0, 0);
        tile->x = 0;
        tile->y = 0;
        tile->text_coords = make_text_coords(gr->renderer, gr->font, &tile->rect, 0, 0, 0, 0, gr->rect_size);
        if (!tile->text_coords)
            return EXIT_FAILURE;
        tile->is_initialized = true;
    }
    for (uint64_t y = 0; y < gr->matrix->size_y; y++)
    {
        for (uint64_t x = 0; x < gr->matrix->size_x; x++)
        {
            Tile *tile = DM_Get(gr->matrix, x, y);
            if (!tile->is_initialized)
            {
                if (x == 0)
                {
                    Tile *prev_tile = DM_Get(gr->matrix, x, y - 1);
                    tile->x = prev_tile->x;
                    tile->y = prev_tile->y + 1;
                }
                else
                {
                    Tile *prev_tile = DM_Get(gr->matrix, x - 1, y);
                    tile->x = prev_tile->x + 1;
                    tile->y = prev_tile->y;
                }
                tile->text_coords = make_text_coords(gr->renderer, gr->font, &tile->rect, tile->x, tile->y, x, y, gr->rect_size);
                if (!tile->text_coords)
                    return EXIT_FAILURE;
                tile->is_initialized = true;
            }
        }
    }
    return EXIT_SUCCESS;
}

int GR_ChangeVisual(GridRender *gr, int8_t delta_x, int8_t delta_y)
{
    for (uint64_t y = 0; y < gr->matrix->size_y; y++)
        for (uint16_t x = 0; x < gr->matrix->size_x; x++)
        {
            Tile *tile = DM_Get(gr->matrix, x, y);
            tile->x += delta_x;
            tile->y += delta_y;
            SDL_DestroyTexture(tile->text_coords);
            tile->text_coords = make_text_coords(gr->renderer, gr->font, &tile->rect, tile->x, tile->y, x, y, gr->rect_size);
            if (!tile->text_coords)
                return EXIT_FAILURE;
        }
    return EXIT_SUCCESS;
}

int GR_ChangeZoom(GridRender *gr, int new_zoom, int length, int width)
{
    uint16_t new_rect_size = gr->rect_size + (new_zoom * gr->settings.zoom_sensibility);
    if (new_rect_size < gr->settings.min_rect_size || new_rect_size > gr->settings.max_rect_size)
        return EXIT_SUCCESS;
    gr->rect_size = new_rect_size;
    if (GR_UpdateScreenSize(gr, length, width) == EXIT_FAILURE)
        return EXIT_FAILURE;
    return GR_ChangeVisual(gr, 0, 0);
}

int GR_Render(GridRender *gr, LinkedList *alive_cells)
{
    bool can_show_coords = true;
    for (uint64_t y = 0; y < gr->matrix->size_y; y++)
        for (uint64_t x = 0; x < gr->matrix->size_x; x++)
        {
            SDL_Rect rect_tile = {
                .h = gr->rect_size,
                .w = gr->rect_size,
                .x = x * gr->rect_size,
                .y = y * gr->rect_size};
            Tile *tile = DM_Get(gr->matrix, x, y);
            if (!tile)
                return EXIT_FAILURE;
            Cell *cell = CL_NewCell(tile->x, tile->y);
            if (!cell)
                return EXIT_FAILURE;
            if (LL_Exists(alive_cells, cell))
            {
                if (SDL_SetRenderDrawColor(gr->renderer, 255, 255, 255, 0) != 0)
                    return EXIT_FAILURE;
            }
            else
            {
                if (SDL_SetRenderDrawColor(gr->renderer, 0, 0, 0, 0) != 0)
                    return EXIT_FAILURE;
            }
            free(cell);
            if (SDL_RenderFillRect(gr->renderer, &rect_tile) != 0)
                return EXIT_FAILURE;
            if (gr->settings.can_show_coords && gr->settings.show_coords)
            {
                if (SDL_RenderCopy(gr->renderer, tile->text_coords, NULL, &tile->rect) != 0)
                    return EXIT_FAILURE;
            }
            if (tile->rect.w >= gr->rect_size)
                can_show_coords = false;
            else
                can_show_coords = true;
        }
    gr->settings.can_show_coords = can_show_coords;
    return EXIT_SUCCESS;
}

void GR_Free(GridRender *gr)
{
    DM_Free(gr->matrix);
    free(gr);
}
