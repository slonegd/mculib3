/**
 * бустом пользоваться не могу, потому что он
 * активно использует оператор new,
 * который тут и тестирую
 */
#define HEAP_SIZE 3

#include <cstddef>
#include "global_new.h"

int main()
{
   #define CHECK(...) if (not (__VA_ARGS__)) return 1;

   auto pbyte1 = new uint8_t;
   CHECK (pbyte1 != nullptr);
   auto memory = *reinterpret_cast<uint8_t(*)[3]>(pbyte1);
   auto pbyte2 = new uint8_t;
   CHECK (pbyte2 != nullptr);
   auto pbyte3 = new uint8_t;
   CHECK (pbyte3 != nullptr);
   // new uint8_t; // segfault

   *pbyte1 = 1;
   CHECK (memory[0] == 1);

   *pbyte2 = 2;
   CHECK (memory[0] == 1);
   CHECK (memory[1] == 2);

   *pbyte3 = 3;
   CHECK (memory[0] == 1);
   CHECK (memory[1] == 2);
   CHECK (memory[2] == 3);
}