#ifndef MEM_ALLOCATIONS_H_
#define MEM_ALLOCATIONS_H_

#include <stddef.h>

typedef struct
{
    void** buffer;
    unsigned int row;
    unsigned int freeIndex;
    size_t elemSize;
    unsigned int curRow;
    unsigned int curFreeIndex;
} DynArr;

typedef struct
{
    size_t elemSize;
    void* data;
    unsigned int size;
    unsigned int capacity;
} Stack;

int   dynArrCtor        (DynArr* arr, size_t elemSize);
int   dynArrDtor        (DynArr* arr);
void* dynArrCalloc      (DynArr* arr);
void* dynArrGetElem     (DynArr* arr);
void  dynArrGetElemReset(DynArr* arr);

int   stackCtor(Stack* stk, size_t elemSize); 
int   stackDtor(Stack* stk);
void* stackPop (Stack* stk);
int   stackPush(Stack* stk, const void* elem);

#endif // MEM_ALLOCATIONS_H_