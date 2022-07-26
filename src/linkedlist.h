// Copyright: https://git.bounceme.net/hex0x0000/GameOfLife-C/src/branch/main/LICENSE
#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>

typedef struct Unit
{
    void *value;
    struct Unit *next;
} Unit;

typedef struct LinkedList
{
    size_t type_size;
    size_t list_size;
    Unit *starting_unit;
    void (*free)(void *unit);
} LinkedList;

/**
 * @brief Returns a new linked list
 *
 * @param type_size the size of the values of the linked list
 * @param _free function that will be called to free the unit's values
 * @return LinkedList* new linked list
 */
LinkedList *LL_New(size_t type_size, void (*free)(void *unit));

/**
 * @brief Adds a value to the linked list
 *
 * @param ll linked list
 * @param value new value
 * @return int 0 if everything went ok, 1 if something went wrong
 */
int LL_Add(LinkedList *ll, void *value);

/**
 * @brief Adds a value to the linked list without making a clone of it
 *
 * @param ll linked list
 * @param value new value
 * @return int 0 if everything went ok, 1 if something went wrong
 */
int LL_AddPointer(LinkedList *ll, void *value);

/**
 * @brief Removes a value from the linked list, only the first value found will be removed
 *
 * @param ll linked list to remove the value from
 * @param value the value to be removed
 */
void LL_Remove(LinkedList *ll, void *value);

/**
 * @brief Gets the value at a specific index
 *
 * @param ll linked list
 * @param index
 * @return void* pointer to the value
 */
void *LL_Get(LinkedList *ll, size_t index);

/**
 * @brief Checks if a value exists
 *
 * @param ll linked list to check
 * @param value value to find
 * @return true the value exists
 * @return false the value doesn't exist
 */
bool LL_Exists(LinkedList *ll, void *value);

/**
 * @brief Iterates over the list
 *
 * @param ll linked list that'll be iterated
 * @param f function that handles every unit, inputs the unit and the iteration index, and outputs the new unit, if it wasn't changed just return NULL
 */
void LL_Iterate(LinkedList *ll, void *(*f)(void *unit, size_t i, void *opt_arg), void *opt_arg);

/**
 * @brief Copies a list
 *
 * @param ll linked list to copy
 * @return LinkedList* copy of the linked list
 */
LinkedList *LL_Copy(LinkedList *ll);

/**
 * @brief removes the first value from the list and returns it
 *
 * @param ll linked list
 * @return void* value removed, it must be freed manually
 */
void *LL_PopHead(LinkedList *ll);

/**
 * @brief Frees the list
 *
 * @param ll linked list to be freed
 */
void LL_Free(LinkedList *ll);

#endif