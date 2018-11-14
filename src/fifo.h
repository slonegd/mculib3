#include <iostream>
#include <type_traits>

class FIFO
{
   // struct Node {
   //    uint8_t v;
   //    Node* next {nullptr};
   // };
   
   // std::array <uint8_t, size> buffer{0};
   uint8_t buffer[255];
   // Node* first {nullptr};
   // Node* last  {nullptr};

public:
   int size;
   void    operator<< (const uint8_t v);
   uint8_t operator>> (uint8_t v);
};

void FIFO::operator<< (const uint8_t v)
{
   static int count {0};

   buffer [count] = v;

   count++;
   size = count;
}

uint8_t FIFO::operator>> (uint8_t v)
{
   static int read {0};

   v = buffer [read];

   read ++;
   size --;

   return v;
}

