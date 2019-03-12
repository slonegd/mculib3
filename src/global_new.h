#pragma once

#include "heap.h"

// собственный глобальный оператор new, который работает с нашей кучей
void* operator new (size_t size)
{
   return heap.allocate(size);
}

inline void* operator new[] (size_t size)
{
    return heap.operator_multinew (size);
}

inline void operator delete[] (void* p)
{
    return heap.operator_multidelete (p);
}