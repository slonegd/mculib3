#pragma once

#include <array>
#include <algorithm>
#include <type_traits>


template <class T, size_t size_>
class Vector : public std::array<T, size_>
{
   using Container = std::array<T, size_>;
   size_t begin_i {0};
   size_t end_i   {0};
   auto& base() { return *static_cast<Container*>(this); }
public:
   void clear()  {begin_i = 0; end_i = 0;}
   auto begin()  {return base().begin() + begin_i;}
   auto end  ()  {return base().begin() + end_i;}
   auto size ()  {return end_i - begin_i;}
   void set_size (size_t v) {begin_i = 0; end_i = std::min(v,size_);}
   void push_back(const T& value) 
   {
      if (end_i < size_) {
         *end() = value;
         end_i++;
      }
   }
   void erase (const size_t index) 
   {
      for (auto i = index + 1; i < end_i; i++) {
         base()[i - 1] = base()[i];
      }
      end_i--;
   }

   T operator[] (const size_t index) 
   {
      return base()[index];
   }

};
