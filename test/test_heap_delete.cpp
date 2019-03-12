/**
 * бустом пользоваться не могу, потому что он
 * активно использует оператор new,
 * который тут и тестирую
 */
#define HEAP_SIZE 17

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

   auto tmp = pbytes1;
   delete[] pbytes1;

   auto pbytes2 = new uint8_t[3];
   CHECK (pbytes2 == tmp);
   CHECK (memory[index-1] == 2);

   pbytes2[2] = 3;
   CHECK (memory[index++] == 3);

   delete[] pbytes2;
   auto pbytes3 = new uint8_t[8];
   CHECK (pbytes3 == tmp);

   delete[] pbytes3;
   auto pbytes4 = new uint8_t[1];

   delete[] pbytes4;
   // new uint8_t[9]; // SEGFAULT

}