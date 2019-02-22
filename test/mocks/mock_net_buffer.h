#pragma once

#define USE_MOCK_NET_BUFFER

#include "net_buffer.h"
#include "process.h"

namespace mock {

template<size_t size_>
class Net_buffer : public ::Net_buffer<size_>
{
   Process& process {Process::make()};
   UART_sized() = default;
   auto& base() { return *static_cast<::Net_buffer<>*>(this); }

public:
   void clear()
   {
      process << "Очистка буфера" << std::endl;
      base().clear;
   }

   void begin()
   {
      process << "Индекс первого элемента буфера = " << 
      base().begin() << std::endl;
   }

   void end()
   {
      process << "Индекс последнего элемента буфера = " <<
      base().end() << std::endl;
   }

   void size()
   {
      process << "Размер буфера = " << base().size() << std::endl;
   }

   Net_buffer& operator<< (const uint8_t& v)
   {
      process << "Записываем значение " << v << " в ячейку " << end_i << std::endl;
      base() << v;
      return *this;
   }

   Net_buffer& operator>> (uint8_t& v)
   {
      base() >> v;
      process << "Получаем значение " << v << "из ячейки " << begin_i << std::endl;
      return *this;
   }

   void pop_front()
   {
      process << "Получаем значение " << base().pop_front() << "из ячейки " << begin_i << std::endl;
   }

   void pop_back()
   {
      process << "Получаем значение " << base().pop_back() << "из ячейки " << end_i << std::endl;
   }
};


} //namespace mock