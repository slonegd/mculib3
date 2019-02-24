#pragma once

#include <string>

#define USE_MOCK_NET_BUFFER

#include "net_buffer.h"
#include "process.h"

namespace mock {

template<size_t size_>
class Net_buffer : public ::Net_buffer<size_>
{
   Process& process {Process::make()};
   auto& base() { return *static_cast<::Net_buffer<size_>*>(this); }

public:
   void clear()
   {
      process << "Очистка буфера" << std::endl;
      base().clear();
   }

   auto begin()
   {
      process << "Индекс первого элемента буфера = " << 
      ::Net_buffer<size_>::begin_i << std::endl;
      return base().begin();
   }

   auto end()
   {
      process << "Индекс последнего элемента буфера = " <<
      ::Net_buffer<size_>::end_i - 1 << std::endl;
      return base().end();
   }

   auto size()
   {
      process << "Размер буфера = " << base().size() << std::endl;
      return base().size();
   }

   Net_buffer& operator<< (const uint8_t& v)
   {
      process << "Записываем значение " << std::to_string(v) << " в ячейку №" << ::Net_buffer<size_>::end_i << std::endl;
      base() << v;
      return *this;
   }

   Net_buffer& operator>> (uint8_t& v)
   {
      process << "Получаем " << "из ячейки №" << ::Net_buffer<size_>::begin_i << " значение ";
      base() >> v;
      process << std::to_string(v) << std::endl;
      
      return *this;
   }

   Net_buffer& operator>> (uint16_t& v)
   {
      process << "Получаем " << "из ячеек №" << ::Net_buffer<size_>::begin_i << " и " << ::Net_buffer<size_>::begin_i + 1 << " значение ";
      base() >> v;
      process << std::to_string(v) << std::endl;
      return *this;
   }

   Net_buffer& operator<< (uint16_t v)
   {
      process << "Записываем значение " << std::to_string(v) << " в ячейки №" << ::Net_buffer<size_>::end_i << " и " << ::Net_buffer<size_>::end_i + 1 << std::endl;
      base() << v;
      return *this;
   }

   uint8_t pop_front()
   {
      process << "Получаем " << "из ячейки №" << ::Net_buffer<size_>::begin_i << " значение ";
      uint8_t v = base().pop_front();
      process << std::to_string(v) << std::endl;
      return v;
   }

   uint8_t pop_back()
   {
      process << "Получаем " << "из ячейки №" << ::Net_buffer<size_>::end_i << " значение ";
      uint8_t v = base().pop_back();
      process << std::to_string(v) << std::endl;
      return v;
   }

   uint8_t& front()
   {
      uint8_t v = base().front();
      process << "Получаем " << "из ячейки №" << ::Net_buffer<size_>::begin_i << " значение ";
      process << std::to_string(v) << std::endl;
      return base().front();
   }

   uint8_t operator[] (int i)
   {
      return base()[i];
   }
};


} //namespace mock