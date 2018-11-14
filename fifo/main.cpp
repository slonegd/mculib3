#include <iostream>
#include <type_traits>

template <class Type, size_t buffer_size = 255>
class FIFO
{
   Type buffer[buffer_size] {0};
   int begin {0};
   int end   {0};
   int buzy_size{0};
public:
   int get_begin(){return begin;}
   int get_size (){return buzy_size;}
   FIFO& operator<< (const Type& v);
   FIFO& operator>> (Type& v);
   Type operator[] (const int index);
};

template <class Type, size_t buffer_size>
FIFO<Type, buffer_size>& FIFO <Type, buffer_size>::operator<< (const Type& v)
{
   buffer [end] = v;
   buzy_size ++;
   end ++;;

   return *this;
}

template <class Type, size_t buffer_size>
FIFO<Type, buffer_size>& FIFO <Type, buffer_size>::operator>> (Type& v)
{
   v = buffer[begin];
   buffer[begin] = 0;
   begin ++;
   buzy_size --;
   if (begin == end) {
      begin = end = 0;
      buzy_size = 0;
   }

   return *this;
}

template <class Type, size_t buffer_size>
Type FIFO <Type, buffer_size>::operator[] (const int index)
{
 
   // assert
   return buffer[index];
   
}

int main ()
{
   FIFO <uint8_t> buffer;
   buffer << 2 << 3 << 4 << 5;
 

   uint8_t x = 0;
   uint8_t y = 0;
   uint8_t z = 0;
   uint8_t k = 0;

   std::cout << buffer.get_begin() << std::endl;
   uint8_t t = 0;
   t = buffer[2];
   

   std::cout << static_cast<int>(t) << std::endl;

   buffer >> x;
   std::cout << buffer.get_begin() << std::endl;
   buffer >> y >> z >> k;

   std::cout << buffer.get_begin() << std::endl;

   std::cout << 0+x << 0+y << 0+z << 0+k << std::endl;
   

   


   return 0;
}