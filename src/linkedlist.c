// Copyright: https://git.bounceme.net/hex0x0000/GameOfLife-C/src/branch/main/LICENSE
#include "linkedlist.h"
#include "utils.h"
#include <string.h>

LinkedList *LL_New(size_t type_size, void (*free)(void *unit))
{
    LinkedList *new_ll = malloc(sizeof(LinkedList));
    if (!new_ll)
        return NULL;
    new_ll->type_size = type_size;
    new_ll->list_size = 0;
    new_ll->starting_unit = NULL;
    new_ll->free = free;
    return new_ll;
}

int LL_Add(LinkedList *ll, void *value)
{
    if (!ll || !value)
        return EXIT_FAILURE;
    if (ll->starting_unit == NULL)
    {
        void *value_copy = malloc(ll->type_size);
        if (!value_copy)
            return EXIT_FAILURE;
        memcpy(value_copy, value, ll->type_size);
        Unit *new_unit = malloc(sizeof(Unit));
        if (!new_unit)
            return EXIT_FAILURE;
        new_unit->value = value_copy;
        new_unit->next = NULL;
        ll->starting_unit = new_unit;
    }
    else
    {
        Unit *last_unit = ll->starting_unit;
        while (last_unit->next != NULL)
            last_unit = last_unit->next;
        void *value_copy = malloc(ll->type_size);
        if (!value_copy)
            return EXIT_FAILURE;
        memcpy(value_copy, value, ll->type_size);
        Unit *new_unit = malloc(sizeof(Unit));
        if (!new_unit)
            return EXIT_FAILURE;
        new_unit->value = value_copy;
        new_unit->next = NULL;
        last_unit->next = new_unit;
    }
    ll->list_size += 1;
    return EXIT_SUCCESS;
}

int LL_AddPointer(LinkedList *ll, void *value)
{
    if (!ll || !value)
        return EXIT_FAILURE;
    if (ll->starting_unit == NULL)
    {
        Unit *new_unit = malloc(sizeof(Unit));
        if (!new_unit)
            return EXIT_FAILURE;
        new_unit->value = value;
        new_unit->next = NULL;
        ll->starting_unit = new_unit;
    }
    else
    {
        Unit *last_unit = ll->starting_unit;
        while (last_unit->next != NULL)
            last_unit = last_unit->next;
        Unit *new_unit = malloc(sizeof(Unit));
        if (!new_unit)
            return EXIT_FAILURE;
        new_unit->value = value;
        new_unit->next = NULL;
        last_unit->next = new_unit;
    }
    ll->list_size += 1;
    return EXIT_SUCCESS;
}

void LL_Remove(LinkedList *ll, void *value)
{
    if (!ll || !value)
        return;
    if (ll->starting_unit == NULL)
        return;
    if (memcmp(ll->starting_unit->value, value, ll->type_size) == 0)
    {
        Unit *next = ll->starting_unit->next;
        (*ll->free)(ll->starting_unit->value);
        free(ll->starting_unit);
        ll->starting_unit = next;
        return;
    }
    Unit *prev_unit = ll->starting_unit;
    Unit *last_unit = prev_unit->next;
    while (last_unit != NULL)
    {
        if (memcmp(last_unit->value, value, ll->type_size) == 0)
        {
            Unit *next = last_unit->next;
            (*ll->free)(last_unit->value);
            free(last_unit);
            prev_unit->next = next;
            return;
        }
        prev_unit = last_unit;
        last_unit = last_unit->next;
    }
    ll->list_size -= 1;
}

void *LL_Get(LinkedList *ll, size_t index)
{
    if (index >= ll->list_size)
        return NULL;
    if (index == 0)
        return ll->starting_unit->value;
    Unit *last_unit = ll->starting_unit;
    for (size_t i = 1; i <= index; i++)
        last_unit = last_unit->next;
    return last_unit->value;
}

bool LL_Exists(LinkedList *ll, void *value)
{
    if (!ll || !value)
        return false;
    Unit *last_unit = ll->starting_unit;
    while (last_unit != NULL)
    {
        int m = memcmp(last_unit->value, value, ll->type_size);
        if (m == 0)
            return true;
        last_unit = last_unit->next;
    }
    return false;
}

void LL_Iterate(LinkedList *ll, void *(*f)(void *unit, size_t i, void *opt_arg), void *opt_arg)
{
    if (!ll)
        return;
    size_t i = 0;
    Unit *last_unit = ll->starting_unit;
    while (last_unit != NULL)
    {
        void *new_val = (*f)(last_unit->value, i, opt_arg);
        if (new_val)
            last_unit->value = new_val;
        last_unit = last_unit->next;
        i++;
    }
}

LinkedList *LL_Copy(LinkedList *ll)
{
    if (!ll)
        return NULL;
    LinkedList *copy = malloc(sizeof(LinkedList));
    if (!copy)
        return NULL;
    copy->type_size = ll->type_size;
    copy->free = ll->free;
    copy->starting_unit = NULL;
    if (ll->starting_unit)
    {
        void *value_copy = malloc(copy->type_size);
        if (!value_copy)
            return NULL;
        memcpy(value_copy, ll->starting_unit->value, copy->type_size);
        Unit *unit_copy = malloc(sizeof(Unit));
        if (!unit_copy)
            return NULL;
        unit_copy->next = NULL;
        unit_copy->value = value_copy;
        copy->starting_unit = unit_copy;
    }
    else
        return copy;
    Unit *last_unit = ll->starting_unit;
    Unit *copy_last_unit = copy->starting_unit;
    while (last_unit != NULL)
    {
        void *value_copy = malloc(copy->type_size);
        if (!value_copy)
            return NULL;
        memcpy(value_copy, last_unit->value, copy->type_size);
        Unit *unit_copy = malloc(sizeof(Unit));
        if (!unit_copy)
            return NULL;
        unit_copy->next = NULL;
        unit_copy->value = value_copy;
        copy_last_unit->next = unit_copy;
        last_unit = last_unit->next;
        copy_last_unit = copy_last_unit->next;
    }
    copy->list_size = ll->list_size;
    return copy;
}

void *LL_PopHead(LinkedList *ll)
{
    if (!ll)
        return NULL;
    Unit *first_unit = ll->starting_unit;
    ll->starting_unit = first_unit->next;
    void *value = first_unit->value;
    free(first_unit);
    ll->list_size -= 1;
    return value;
}

void LL_Free(LinkedList *ll)
{
    if (!ll)
        return;
    Unit *last_unit = ll->starting_unit;
    while (last_unit != NULL)
    {
        Unit *next = last_unit->next;
        (*ll->free)(last_unit->value);
        free(last_unit);
        last_unit = next;
    }
    free(ll);
}
