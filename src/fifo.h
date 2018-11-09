#include <iostream>
#include <type_traits>

class FIFO
{
   uint8_t v;
   static size_t size;
   uint8_t buffer[size];
   FIFO* first;
   FIFO* last;
   FIFO* next;

   void    operator<< (const uint8_t v);
   uint8_t operator>> (uint8_t v);
};

void FIFO::operator<< (const uint8_t v)
{
   static int count {0};

   buffer << v;

   count++;
   size = count;
}

uint8_t FIFO::operator>> (uint8_t v)
{
   buffer >> v;
   size -= 1;
}