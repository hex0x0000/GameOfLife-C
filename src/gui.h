// Copyright: https://git.bounceme.net/hex0x0000/GameOfLife-C/src/branch/main/LICENSE
#ifndef __GUI_H__
#define __GUI_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdint.h>
#include <stdbool.h>
#include "settings.h"
#include "version.h"

#ifndef FONT
#error You must specify the font's path with -DFONT="\"/path/to/font.ttf\""'
#endif
#define GUI_TEXT_SIZE 15

typedef struct ScrollBar
{
    SDL_Texture *label;
    SDL_Rect label_rect;
    SDL_Rect bar;
    SDL_Rect indicator;
    float value; // 0.0 - 100.0
} ScrollBar;

typedef struct CheckBox
{
    SDL_Texture *label;
    SDL_Rect label_rect;
    SDL_Rect box;
    SDL_Rect check;
    bool value;
} CheckBox;

typedef struct Button
{
    SDL_Texture *label;
    SDL_Rect label_rect;
    SDL_Rect button;
    void (*btn_action)(void *);
} Button;

typedef struct SettingsUI
{
    CheckBox *show_coordinates;
    CheckBox *show_grid;
    ScrollBar *zoom_sensibility;
    ScrollBar *speed;
    Button *export_gen;
    Button *import_gen;
    SDL_Rect settings_rect;
    SDL_Rect settings_rect_outer;
} SettingsUI;

typedef struct GUI
{
    SDL_Texture *game_info;
    SDL_Rect game_info_rect;
    SDL_Texture *press_tab;
    SDL_Rect press_tab_rect;
    SDL_Texture *help_text;
    SDL_Rect help_text_rect;
    Settings *settings;
    SettingsUI *settingsui;
    SDL_Renderer *renderer;
    TTF_Font *font;
} GUI;

GUI *GUI_New(SDL_Renderer *renderer, Settings *settings);
int GUI_RenderInfo(GUI *gui, bool show_help, float game_speed, uint32_t gen_num);
int GUI_RenderSettings(GUI *gui);
void GUI_Free(GUI *gui);

#endif