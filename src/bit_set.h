#pragma once

#include <algorithm>
#include <array>

// не помню зачем этот класс создавался
template<uint16_t qty_bit>
struct Bit_set
{
   std::array<bool, qty_bit> bits {0};
   int num{0};
   int i  {0};

   bool operator[] (size_t n)
   {
      return n < qty_bit ? bits[n] : true;
   }

   Bit_set<qty_bit> operator= (uint16_t n)
   {
      num = n;
      while (n > 0) {
         bits[i++] = n % 2;
         n /= 2;
      }
      i = 0;
      std::reverse(bits.begin(), bits.end());
      return *this;
   }
   
   operator uint16_t ()
   {
      return num;
   }

   auto& get_bin() {return bits;}

};

// этот класс создавался для заполнения метатаблицы (понятная работа с битами)
template <class T>
struct bit_set {
    T value;
    constexpr bit_set (T value) : value{value} {}
    constexpr bool operator[] (T bit)
    {
        return value & (1 << bit); 
    }
    constexpr void set (T bit)
    {
        value |= (1 << bit);
    }
};