/**
 * бустом пользоваться не могу, потому что он
 * активно использует оператор new,
 * который тут и тестирую
 */
#define HEAP_SIZE 17

#include <cstddef>
#include "heap.h"

int main()
{
   auto pbytes1 = new uint8_t[2];
   new uint8_t;
   delete[] pbytes1;
   // new uint8_t[2]; // SEGFAULT
}