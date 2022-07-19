#ifndef __GOL_H__
#define __GOL_H__

#include "linkedlist.h"
#include <stdint.h>
#include <stdio.h>

#define MAX_POOL 20

typedef struct Cell
{
    int64_t x;
    int64_t y;
} Cell;

/**
 * @brief Initializes a new memset'd cell
 * 
 * @param x x coord
 * @param y y coord
 * @return Cell* New cell (checked = false), must be freed with free()
 */
Cell *CL_NewCell(int64_t x, int64_t y);

LinkedList *GOL_Import(const char *filename);
int GOL_Export(LinkedList *cells, const char *filename);

void GOL_NewGen(LinkedList *gen_pool);
void GOL_Add(LinkedList *cells, int64_t x, int64_t y);
void GOL_Remove(LinkedList *cells, int64_t x, int64_t y);

#endif