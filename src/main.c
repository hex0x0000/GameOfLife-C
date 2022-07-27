// Copyright: https://git.bounceme.net/hex0x0000/GameOfLife-C/src/branch/main/LICENSE
#include "version.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <pthread.h>
#include <sys/time.h>
#include <errno.h>
#include "utils.h"
#include "grid_render.h"
#include "gol.h"
#include "settings.h"
#include "gui.h"

#define FONT_SIZE 20
#define FPS 60

pthread_mutex_t lock;
#define MUTEX lock
#include "log.h"

_Atomic(uint32_t) await_time = 100; // Milliseconds
_Atomic(uint32_t) gen_num = 0;
volatile atomic_bool quit = false;
LinkedList *current_gen = NULL;
pthread_t thread_id;

void free_gen_pool(void *unit) { LL_Free((LinkedList *)unit); }

void *gen_maker_thread(void *_ __attribute__((unused)))
{
    LinkedList *gen_pool = LL_New(sizeof(LinkedList *), free_gen_pool);
    pthread_mutex_lock(&lock);
    if (current_gen)
    {
        LinkedList *curr_gen_copy = LL_Copy(current_gen);
        LL_Free(current_gen);
        current_gen = NULL;
        pthread_mutex_unlock(&lock);
        LL_Add(gen_pool, &curr_gen_copy);
    }
    else
    {
        pthread_mutex_unlock(&lock);
        LinkedList *first_gen = LL_New(sizeof(Cell), free);
        // Glider
        GOL_Add(first_gen, 1, 0);
        GOL_Add(first_gen, 2, 1);
        GOL_Add(first_gen, 0, 2);
        GOL_Add(first_gen, 1, 2);
        GOL_Add(first_gen, 2, 2);
        LL_Add(gen_pool, &first_gen);
    }

    struct timeval t_before, t_now;
    uint32_t before, now;
    gettimeofday(&t_before, NULL);
    before = t_before.tv_sec * 1000 + t_now.tv_usec / 1000;
    while (!quit)
    {
        GOL_NewGen(gen_pool);

        gettimeofday(&t_now, NULL);
        now = t_now.tv_sec * 1000 + t_now.tv_usec / 1000;

        if (now - before >= await_time && gen_pool->list_size > 1)
        {
            LOG_DEBUG("difference: %d >= AWAIT_TIME: %d", now - before, await_time);
            pthread_mutex_lock(&lock);
            if (current_gen)
                LL_Free(current_gen);
            current_gen = *(LinkedList **)LL_PopHead(gen_pool);
            pthread_mutex_unlock(&lock);
            gen_num += 1;

            gettimeofday(&t_before, NULL);
            before = t_before.tv_sec * 1000 + t_now.tv_usec / 1000;
        }
    }
    LL_Free(gen_pool);
    return NULL;
}

int main(int argc, char **argv)
{
    puts("GameOfLife " VERSION "  Copyright (C) 2022  Valentino Peggi\n"
         "This program comes with ABSOLUTELY NO WARRANTY\n"
         "This is free software, and you are welcome to redistribute it under certain conditions");
    if (LOG_INIT() == EXIT_FAILURE)
    {
#ifdef FILE_LOGS
        int err = errno;
        handle_file_error(FILE_LOGS, err, false);
        return 1;
#endif
    }

    if (argc > 1)
    {
        current_gen = GOL_Import(argv[1]);
        if (!current_gen)
        {
            int err = errno;
            handle_file_error(argv[1], err, true);
            return 1;
        }
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        LOG_CRITICAL("Couldn't initialize SDL: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    if (TTF_Init() != 0)
    {
        LOG_CRITICAL("Couldn't initialize font library: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Window *window = SDL_CreateWindow("Conway's Game of Life", 100, 100, 500, 500, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!window)
    {
        LOG_CRITICAL("Couldn't initialize window: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
    {
        LOG_CRITICAL("Couldn't initialize renderer: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    TTF_Font *font = TTF_OpenFont(FONT, FONT_SIZE);
    if (!font)
    {
        LOG_CRITICAL("Couldn't load font: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    Settings *settings = settings_default();

    GridRender *gr = GR_New(renderer, font, settings);
    if (!gr)
    {
        LOG_CRITICAL("Couldn't initialize GridRender.");
        return EXIT_FAILURE;
    }

    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        LOG_CRITICAL("Couldn't initialize mutex.");
        return EXIT_FAILURE;
    }
    if (pthread_create(&thread_id, NULL, gen_maker_thread, NULL) != 0)
    {
        LOG_CRITICAL("Couldn't start second thread.");
        return EXIT_FAILURE;
    }

    // Main loop
    SDL_Keycode key_pressed = 0;
    bool gui_open = false;
    while (!quit)
    {
        while (gui_open && !quit)
        {
            
        }
        
        int w = -1;
        int h = -1;
        SDL_GetRendererOutputSize(renderer, &w, &h);
        if (w == -1 || h == -1)
        {
            LOG_CRITICAL("Couldn't get window size: %s", SDL_GetError());
            break;
        }
        if (GR_UpdateScreenSize(gr, h, w) == EXIT_FAILURE)
        {
            LOG_CRITICAL("Failed to update grid size.");
            break;
        }

        SDL_Event event;
        if (SDL_PollEvent(&event) == 1)
        {
            switch (event.type)
            {
            case SDL_QUIT:
                quit = true;
                break;

            case SDL_MOUSEWHEEL:
                if (GR_ChangeZoom(gr, event.wheel.y, h, w) == EXIT_FAILURE)
                {
                    LOG_CRITICAL("Failed to change zoom");
                    quit = true;
                    continue;
                }
                break;

            case SDL_KEYDOWN:
                key_pressed = event.key.keysym.sym;
                break;

            case SDL_KEYUP:
                key_pressed = 0;
                break;

            default:
                break;
            }
        }
        switch (key_pressed)
        {
        case SDLK_UP:
            GR_ChangeVisual(gr, 0, -1);
            break;

        case SDLK_DOWN:
            GR_ChangeVisual(gr, 0, 1);
            break;

        case SDLK_LEFT:
            GR_ChangeVisual(gr, -1, 0);
            break;

        case SDLK_RIGHT:
            GR_ChangeVisual(gr, 1, 0);
            break;

        case SDLK_TAB:
            break;

        default:
            break;
        }

        int err = 0;
        err += SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        err += SDL_RenderClear(renderer);
        if (err != 0)
            LOG_ERROR("Rendering return an error: %s", SDL_GetError());
        pthread_mutex_lock(&lock);
        if (current_gen)
            if (GR_Render(gr, current_gen) == EXIT_FAILURE)
            {
                LOG_CRITICAL("Failed to render grid: %s", SDL_GetError());
                break;
            }
        pthread_mutex_unlock(&lock);
        SDL_RenderPresent(renderer);

        if (!precise_sleep(1 / FPS))
        {
            LOG_CRITICAL("Failed to sleep.");
            break;
        }
    }
    quit = true;
    pthread_join(thread_id, NULL);
    pthread_mutex_destroy(&lock);
    if (GOL_Export(current_gen, "final_gen.txt") == EXIT_FAILURE)
    {
        int err = errno;
        handle_file_error("final_gen.txt", err, true);
    }
    if (current_gen)
        LL_Free(current_gen);
    GR_Free(gr);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    free(settings);
    SDL_Quit();
    LOG_CLOSE();
    return 0;
}
