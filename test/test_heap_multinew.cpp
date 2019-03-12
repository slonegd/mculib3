/**
 * бустом пользоваться не могу, потому что он
 * активно использует оператор new,
 * который тут и тестирую
 */
#define HEAP_SIZE 22

#include <cstddef>
#include "global_new.h"

int main()
{
   #define CHECK(...) if (not (__VA_ARGS__)) return 1;

   auto memory = *reinterpret_cast<uint8_t(*)[20]>(new uint8_t);
   size_t index {1};

   auto pbytes1 = new uint8_t[2];
   index += sizeof(size_t);
   CHECK (pbytes1 != nullptr);
   pbytes1[0] = 1;
   CHECK (memory[index++] == 1);

   pbytes1[1] = 2;
   CHECK (memory[index++] == 2);

   pbytes1[2] = 3;
   CHECK (memory[index++] == 3);

   auto p = new uint8_t;
   *p = 4;
   CHECK (memory[index-1] == 4);

   auto pbytes2 = new uint8_t[2];
   index += sizeof(size_t);
   CHECK (pbytes2 != nullptr);
   pbytes2[0] = 5;
   CHECK (memory[index++] == 5);

   pbytes2[1] = 6;
   CHECK (memory[index++] == 6);

   // new uint8_t[1]; // SEGFAULT
}