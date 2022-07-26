// Copyright: https://git.bounceme.net/hex0x0000/GameOfLife-C/src/branch/main/LICENSE
#ifndef __DYNAMIC_MATRIX_H__
#define __DYNAMIC_MATRIX_H__

#include <stdint.h>
#include <stdlib.h>

typedef struct DynMatrix
{
    uint64_t size_x;
    uint64_t size_y;
    size_t type_size;
    void ***matrix;
    void *init_value;
    void (*free)(void *value);
} DynMatrix;

/**
 * @brief Creates a new DynMatrix
 *
 * @param init_value the value that will initialize the matrix when DM_Reallocking
 * @param type_size the size of the type
 * @param _free function that will be called to free the values
 * @return DynMatrix* new DynMatrix
 */
DynMatrix *DM_New(void *init_value, size_t type_size, void (*free)(void *value));

/**
 * @brief Resizes the matrix
 *
 * @param dm DynMatrix to be resized
 * @param new_size_x new width size
 * @param new_size_y new length size
 * @return int 0 if everything went ok, 1 if something went wrong
 */
int DM_Realloc(DynMatrix *dm, uint64_t new_size_x, uint64_t new_size_y);

/**
 * @brief Gets the pointer of a value
 *
 * @param dm DynMatrix from which the value will be taken
 * @param x column of the value
 * @param y row of the value
 * @return void* value's pointer
 */
void *DM_Get(DynMatrix *dm, uint64_t x, uint64_t y);

/**
 * @brief Frees the DynMatrix
 *
 * @param dm matrix to be freed
 */
void DM_Free(DynMatrix *dm);

#endif