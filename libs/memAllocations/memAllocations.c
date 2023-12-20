#include "memAllocations.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "memAllocations_cfg.h"


// ================================ DynArr =====================================


void* dynArrCalloc(DynArr* arr)
{
    assert(arr);

    if (arr->freeIndex >  MEM_DYN_ARR_CAPACITY     ||
        arr->row       >= MEM_DYN_ARR_BUFFER_CAPACITY)
    {
        return NULL;
    }

    if (arr->freeIndex == MEM_DYN_ARR_CAPACITY)
    {
        arr->row++;
        if (arr->row == MEM_DYN_ARR_BUFFER_CAPACITY)
            return NULL;
        arr->freeIndex = 0;

        arr->buffer[arr->row] = (void*) calloc(MEM_DYN_ARR_CAPACITY, arr->elemSize);
        if (arr->buffer[arr->row] == NULL)
            return NULL;
    }

    return (void*)((size_t)arr->buffer[arr->row] + arr->elemSize * (arr->freeIndex)++);
}


int dynArrCtor(DynArr* arr, size_t elemSize)
{
    assert(arr);

    if (arr == NULL)
        return -1;

    arr->buffer = (void**) calloc(MEM_DYN_ARR_BUFFER_CAPACITY, sizeof(void*));
    if (arr->buffer == NULL)
        return -1;

    arr->buffer[0] = (void*) calloc(MEM_DYN_ARR_CAPACITY, elemSize);
    if (arr->buffer[0] == NULL)
        return -1;

    arr->elemSize  = elemSize;

    arr->   row = 0;
    arr->curRow = 0;
    arr->   freeIndex = 0;
    arr->curFreeIndex = 0;

    return 0;
}


int dynArrDtor(DynArr* arr)
{
    assert(arr);

    if (arr == NULL)
        return -1;

    if (arr->buffer == NULL)
        return -1;

    for (unsigned int i = 0; i <= arr->row; i++)
        free(arr->buffer[i]);

    free(arr->buffer);

    return 0;
}


void* dynArrGetElem(DynArr* arr)
{
    assert(arr);

    if (arr == NULL || arr->buffer == NULL)
        return NULL;

    if (arr->curFreeIndex >= MEM_DYN_ARR_CAPACITY)
        return NULL;

    if (arr->curRow > arr->row)
        return NULL;

    if (arr->curRow == arr->row && arr->curFreeIndex >= arr->freeIndex)
        return NULL;

    void* elemRow = arr->buffer[arr->curRow];
    void* elem    = (void*)((size_t)elemRow + arr->elemSize * arr->curFreeIndex);

    arr->curFreeIndex++;
    if (arr->curFreeIndex == MEM_DYN_ARR_CAPACITY)
    {
        arr->curFreeIndex = 0;
        arr->curRow++;
    }

    return elem;
}


void dynArrGetElemReset(DynArr* arr)
{
    assert(arr);

    if (arr == NULL || arr->buffer == NULL)
        return;

    arr->curFreeIndex = 0;
    arr->curRow       = 0;
}


// ================================ STACK =====================================


int stackCtor(Stack* stk, size_t elemSize)
{
    if (stk == NULL)
        return -1;

    stk->data = calloc(elemSize, MEM_STACK_DEFAULT_CAPACITY);
    if (stk->data == NULL)
        return -1;

    stk->capacity = MEM_STACK_DEFAULT_CAPACITY;
    stk->elemSize = elemSize;
    stk->size = 0;

    return 0;
}


int stackDtor(Stack* stk)
{
    if (stk == NULL)
        return -1;

    if (stk->data == NULL)
        return -1;

    free(stk->data);
    stk->data = NULL;
    stk->elemSize = (size_t)       -1;
    stk->size     = (unsigned int) -1;
    stk->capacity = (unsigned int) -1;

    return 0;
}


void* stackPop(Stack* stk)
{
    if (stk == NULL || stk->size == 0)
        return NULL;

    stk->size--;
    return (char*)stk->data + stk->size * stk->elemSize;
}


int stackPush(Stack* stk, const void* elem)
{
    if (stk == NULL)
        return -1;

    if (stk->size == stk->capacity)
    {
        unsigned int newCapacity = stk->capacity * MEM_STACK_CAPACITY_GROWTH_FACTOR;
        void* newData = realloc(stk->data, newCapacity * stk->elemSize);
        if (newData == NULL)
        {
            return -1;
        }
        stk->data = newData;
        stk->capacity = newCapacity;
    }

    memcpy((char*)stk->data + stk->size * stk->elemSize, elem, stk->elemSize);
    stk->size++;
    return 0;
}
