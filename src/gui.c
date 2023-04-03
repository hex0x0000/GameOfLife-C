// Copyright: https://git.bounceme.net/hex0x0000/GameOfLife-C/src/branch/main/LICENSE
#include "gui.h"

SDL_Texture *new_text(SDL_Renderer *renderer,
                      const char *text,
                      SDL_Rect *rect,
                      TTF_Font *font)
{
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, (SDL_Color){255, 255, 255, 0});
    if (!surface)
        return NULL;
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture)
    {
        SDL_FreeSurface(surface);
        return NULL;
    }
    rect->h = surface->h;
    rect->w = surface->w;
    SDL_FreeSurface(surface);
    rect->x = 0;
    rect->y = 0;
    return texture;
}

ScrollBar *new_scrollbar(SDL_Renderer *renderer, TTF_Font *font, const char *text)
{
    ScrollBar *scrollbar = malloc(sizeof(ScrollBar));
    if (!scrollbar)
        return NULL;
    scrollbar->label = new_text(renderer, text, &scrollbar->label_rect, font);
    scrollbar->bar = (SDL_Rect){
        .h = 20,
        .w = 0,
        .x = 0,
        .y = 0};
    scrollbar->indicator = (SDL_Rect){
        .h = 25,
        .w = 15,
        .x = 0,
        .y = 0};
    scrollbar->value = malloc(sizeof(float));
    *scrollbar->value = 0.0;
    return scrollbar;
}

void set_scrollbar_pos_size(ScrollBar *scrollbar, int x, int y, int box_w)
{
    scrollbar->label_rect.x = x + 5;
    scrollbar->label_rect.y = y + 5;
    scrollbar->bar.x = scrollbar->label_rect.x + scrollbar->label_rect.w + 5;
    scrollbar->bar.y = scrollbar->label_rect.y + scrollbar->label_rect.h + 5;
    scrollbar->bar.w = box_w - scrollbar->label_rect.x - 10;
    scrollbar->indicator.x = scrollbar->bar.x - scrollbar->indicator.w / 2;
    scrollbar->indicator.y = y;
}

void set_scrollbar_value(ScrollBar *scrollbar, SDL_Rect *mouse_pos)
{
    if (SDL_HasIntersection(&scrollbar->bar, mouse_pos))
    {
        int distance = mouse_pos - scrollbar->indicator.x;
        if (distance > 0 && distance < scrollbar->bar.w)
        {
            *scrollbar->value = (float)distance / (float)scrollbar->bar.w;
            scrollbar->indicator.x = mouse_pos->x;
        }
    }
}

int render_scrollbar(SDL_Renderer *renderer, ScrollBar *scrollbar)
{
    if (SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0) != 0)
        return EXIT_FAILURE;
    if (SDL_RenderCopy(renderer, scrollbar->label, NULL, &scrollbar->label_rect) != 0)
        return EXIT_FAILURE;
    if (SDL_RenderFillRect(renderer, &scrollbar->bar) != 0)
        return EXIT_FAILURE;
    if (SDL_SetRenderDrawColor(renderer, 150, 150, 150, 0) != 0)
        return EXIT_FAILURE;
    if (SDL_RenderFillRect(renderer, &scrollbar->indicator) != 0)
        return EXIT_FAILURE;
}

void free_scrollbar(ScrollBar *scrollbar)
{
    SDL_DestroyTexture(scrollbar->label);
    free(scrollbar->value);
    free(scrollbar);
}

SettingsUI *new_settingsui(SDL_Renderer *renderer, TTF_Font *font)
{
    SettingsUI *settingsui = malloc(sizeof(SettingsUI));
    settingsui->speed = new_scrollbar(renderer, font, "Speed: ");
    settingsui->zoom_sensibility = new_scrollbar(renderer, font, "Zoom Sensibility: ");
    return settingsui;
}

SDL_Texture *new_text(SDL_Renderer *renderer,
                      const char *text,
                      SDL_Rect *rect,
                      TTF_Font *font)
{
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, (SDL_Color){255, 255, 255, 0});
    if (!surface)
        return NULL;
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture)
    {
        SDL_FreeSurface(surface);
        return NULL;
    }
    rect->h = surface->h;
    rect->w = surface->w;
    SDL_FreeSurface(surface);
    rect->x = 0;
    rect->y = 0;
    return texture;
}

GUI *GUI_New(SDL_Renderer *renderer, Settings *settings)
{
    int w = -1, h = -1;
    SDL_GetRendererOutputSize(renderer, &w, &h);
    if (w == -1 || h == -1)
        return NULL;
    GUI *new_gui = malloc(sizeof(GUI));
    if (!new_gui)
        return NULL;
    new_gui->font = TTF_OpenFont(FONT, GUI_TEXT_SIZE);
    if (!new_gui->font)
        return NULL;
    new_gui->renderer = renderer;
    new_gui->settings = settings;
    new_gui->game_info = new_text(renderer,
                                  "| GameOfLife " VERSION " | Gen 0 | Speed 0.0 |",
                                  &new_gui->game_info_rect,
                                  new_gui->font);
    new_gui->press_tab = new_text(renderer,
                                  "Press Tab to show help",
                                  &new_gui->press_tab_rect,
                                  new_gui->font);
    new_gui->help_text = new_text(renderer,
                                  "+ -> Increase speed\n"
                                  "- -> Decrease speed\n"
                                  "p -> Pause/Unpause\n"
                                  "e -> Toggle editing mode\n"
                                  "s -> Open/Close settings",
                                  &new_gui->help_text_rect,
                                  new_gui->font);
    new_gui->settingsui = malloc(sizeof(SettingsUI));
    if (!new_gui->game_info || !new_gui->press_tab || !new_gui->help_text || !new_gui->settingsui)
        return NULL;
    return new_gui;
}

int GUI_RenderInfo(GUI *gui, bool show_help, float game_speed, uint32_t gen_num)
{
}

int GUI_Render(GUI *gui)
{
}

void GUI_Free(GUI *gui)
{
}
