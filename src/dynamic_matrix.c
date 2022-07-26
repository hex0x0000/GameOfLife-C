// Copyright: https://git.bounceme.net/hex0x0000/GameOfLife-C/src/branch/main/LICENSE
#include "dynamic_matrix.h"
#include <string.h>
#include "utils.h"

DynMatrix *DM_New(void *init_value, size_t type_size, void (*free)(void *value))
{
    DynMatrix *new_dm = malloc(sizeof(DynMatrix));
    if (!new_dm)
        return NULL;
    void *init_value_copy = malloc(type_size);
    if (!init_value_copy)
    {
        free(new_dm);
        return NULL;
    }
    memcpy(init_value_copy, init_value, type_size);
    new_dm->init_value = init_value_copy;
    new_dm->size_x = 0;
    new_dm->size_y = 0;
    new_dm->type_size = type_size;
    new_dm->free = free;
    return new_dm;
}

int DM_Realloc(DynMatrix *dm, uint64_t new_size_x, uint64_t new_size_y)
{
    if (new_size_x == dm->size_x && new_size_y == dm->size_y)
        return EXIT_SUCCESS;

    uint64_t old_size_x = dm->size_x;
    uint64_t old_size_y = dm->size_y;

    if (old_size_x == 0 && old_size_y == 0)
    {
        dm->matrix = calloc(new_size_y, sizeof(void *));
        for (uint64_t y = 0; y < new_size_y; y++)
        {
            dm->matrix[y] = calloc(new_size_x, sizeof(void *));
            if (!dm->matrix[y])
                return EXIT_FAILURE;
            for (uint64_t x = 0; x < new_size_x; x++)
            {
                void *new_value = malloc(dm->type_size);
                if (!new_value)
                {
                    free(dm->matrix[y]);
                    return EXIT_FAILURE;
                }
                memcpy(new_value, dm->init_value, dm->type_size);
                dm->matrix[y][x] = new_value;
            }
        }
        dm->size_x = new_size_x;
        dm->size_y = new_size_y;
        return EXIT_SUCCESS;
    }
    else if (old_size_x == 0 || old_size_y == 0)
        return EXIT_FAILURE;

    if (old_size_x < new_size_x)
    {
        for (uint64_t y = 0; y < old_size_y; y++)
        {
            dm->matrix[y] = realloc(dm->matrix[y], sizeof(void *) * new_size_x);
            if (!dm->matrix[y])
                return EXIT_FAILURE;
            for (uint64_t x = old_size_x; x < new_size_x; x++)
            {
                void *new_value = malloc(dm->type_size);
                if (!new_value)
                {
                    free(dm->matrix[y]);
                    return EXIT_FAILURE;
                }
                memcpy(new_value, dm->init_value, dm->type_size);
                dm->matrix[y][x] = new_value;
            }
        }
    }
    else
    {
        for (uint64_t y = 0; y < old_size_y; y++)
        {
            for (uint64_t x = new_size_x; x < old_size_x; x++)
                (*dm->free)(dm->matrix[y][x]);
            dm->matrix[y] = realloc(dm->matrix[y], sizeof(void *) * new_size_x);
            if (!dm->matrix[y])
                return EXIT_FAILURE;
        }
    }

    if (old_size_y < new_size_y)
    {
        dm->matrix = realloc(dm->matrix, sizeof(void *) * new_size_y);
        if (!dm->matrix)
            return EXIT_FAILURE;
        for (uint64_t y = old_size_y; y < new_size_y; y++)
        {
            dm->matrix[y] = calloc(new_size_x, sizeof(void *));
            if (!dm->matrix[y])
                return EXIT_FAILURE;
            for (uint64_t x = 0; x < new_size_x; x++)
            {
                void *new_value = malloc(dm->type_size);
                if (!new_value)
                {
                    free(dm->matrix[y]);
                    return EXIT_FAILURE;
                }
                memcpy(new_value, dm->init_value, dm->type_size);
                dm->matrix[y][x] = new_value;
            }
        }
    }
    else
    {
        for (uint64_t y = new_size_y; y < old_size_y; y++)
        {
            for (uint64_t x = 0; x < new_size_x; x++)
                (*dm->free)(dm->matrix[y][x]);
            free(dm->matrix[y]);
        }
        dm->matrix = realloc(dm->matrix, sizeof(void *) * new_size_y);
        if (!dm->matrix)
            return EXIT_FAILURE;
    }

    dm->size_x = new_size_x;
    dm->size_y = new_size_y;

    return EXIT_SUCCESS;
}

void *DM_Get(DynMatrix *dm, uint64_t x, uint64_t y)
{
    if (x >= dm->size_x || y >= dm->size_y)
        return NULL;
    return dm->matrix[y][x];
}

void DM_Free(DynMatrix *dm)
{
    (*dm->free)(dm->init_value);
    for (uint64_t y = 0; y < dm->size_y; y++)
    {
        for (uint64_t x = 0; x < dm->size_x; x++)
            (*dm->free)(dm->matrix[y][x]);
        free(dm->matrix[y]);
    }
    free(dm->matrix);
    free(dm);
}
