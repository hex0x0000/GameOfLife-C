#include "gol.h"
#include "log.h"
#include <stdlib.h>
#include <string.h>

Cell *CL_NewCell(int64_t x, int64_t y)
{
    Cell *cell = NULL;
    {
        void *_cell = malloc(sizeof(Cell));
        memset(_cell, 0, sizeof(Cell));
        cell = (Cell *)_cell;
    }
    if (!cell)
        return NULL;
    cell->x = x;
    cell->y = y;
    return cell;
}

typedef struct Args
{
    LinkedList *prev_gen;
    LinkedList *new_gen;
} Args;

bool exists(LinkedList *list, int64_t x, int64_t y)
{
    Cell *cell = CL_NewCell(x, y);
    if (!cell)
        return false;
    bool res = LL_Exists(list, cell);
    free(cell);
    return res;
}

void _handle_cell(LinkedList *prev_gen, LinkedList *new_gen, bool is_alive, int64_t x, int64_t y)
{
    if (exists(new_gen, x, y))
        return;
    uint8_t alive_neighbors = 0;
    // Checks how many neighbors this cell has and also
    // handles dead cells (only if the cell handled here wasn't dead already)
    if (exists(prev_gen, x - 1, y - 1))
        alive_neighbors += 1;
    else if (is_alive)
        _handle_cell(prev_gen, new_gen, false, x - 1, y - 1);

    if (exists(prev_gen, x, y - 1))
        alive_neighbors += 1;
    else if (is_alive)
        _handle_cell(prev_gen, new_gen, false, x, y - 1);

    if (exists(prev_gen, x + 1, y - 1))
        alive_neighbors += 1;
    else if (is_alive)
        _handle_cell(prev_gen, new_gen, false, x + 1, y - 1);

    if (exists(prev_gen, x - 1, y))
        alive_neighbors += 1;
    else if (is_alive)
        _handle_cell(prev_gen, new_gen, false, x - 1, y);

    if (exists(prev_gen, x + 1, y))
        alive_neighbors += 1;
    else if (is_alive)
        _handle_cell(prev_gen, new_gen, false, x + 1, y);

    if (exists(prev_gen, x - 1, y + 1))
        alive_neighbors += 1;
    else if (is_alive)
        _handle_cell(prev_gen, new_gen, false, x - 1, y + 1);

    if (exists(prev_gen, x, y + 1))
        alive_neighbors += 1;
    else if (is_alive)
        _handle_cell(prev_gen, new_gen, false, x, y + 1);

    if (exists(prev_gen, x + 1, y + 1))
        alive_neighbors += 1;
    else if (is_alive)
        _handle_cell(prev_gen, new_gen, false, x + 1, y + 1);

    if (is_alive && !(alive_neighbors < 2 || alive_neighbors > 3))
    {
        Cell *cell = CL_NewCell(x, y);
        LL_AddPointer(new_gen, cell);
    }
    else if (alive_neighbors == 3)
    {
        Cell *cell = CL_NewCell(x, y);
        LL_AddPointer(new_gen, cell);
    }
}

void *handle_cell(void *unit, uint64_t i __attribute__((unused)), void *opt_arg)
{
    Cell *cell = (Cell *)unit;
    LinkedList *prev_gen, *new_gen;
    {
        Args *args = (Args *)opt_arg;
        prev_gen = args->prev_gen;
        new_gen = args->new_gen;
    }
    _handle_cell(prev_gen, new_gen, true, cell->x, cell->y);
    return NULL;
}

void GOL_NewGen(LinkedList *gen_pool)
{
    if (gen_pool->list_size == 0)
        return;
    if (gen_pool->list_size >= MAX_POOL)
        return;
    LinkedList *new_gen = LL_New(sizeof(Cell), free);
    LinkedList *prev_gen = *(LinkedList **)LL_Get(gen_pool, gen_pool->list_size - 1);
    Args args = {
        .prev_gen = prev_gen,
        .new_gen = new_gen};
    LL_Iterate(prev_gen, handle_cell, &args);
    LL_Add(gen_pool, &new_gen);
}

void GOL_Add(LinkedList *cells, int64_t x, int64_t y)
{
    Cell *cell = CL_NewCell(x, y);
    LL_AddPointer(cells, cell);
}

void GOL_Remove(LinkedList *cells, int64_t x, int64_t y)
{
    Cell *cell = CL_NewCell(x, y);
    LL_Remove(cells, cell);
    free(cell);
}

LinkedList *GOL_Import(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
        return NULL;
    LinkedList *new = LL_New(sizeof(Cell), free);
    while (1) {
        int64_t x, y;
        if (fscanf(file, "%ld, %ld\n", &x, &y) != 2)
            break;
        GOL_Add(new, x, y);
    }
    fclose(file);
    return new;
}

void *_export_cell(void *unit, size_t _ __attribute__((unused)), void *opt_arg)
{
    Cell *cell = unit;
    FILE *file = opt_arg;
    if(!fprintf(file, "%ld, %ld\n", cell->x, cell->y))
        LOG_ERROR("Failed to export generation");
    return NULL;
}

int GOL_Export(LinkedList *cells, const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (!file)
        return EXIT_FAILURE;
    LL_Iterate(cells, _export_cell, file);
    fclose(file);
    return EXIT_SUCCESS;
}
