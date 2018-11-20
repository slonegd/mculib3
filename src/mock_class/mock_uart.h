#pragma once

#include "test_periph.h"
#include "mock_pin.h"

namespace mcu {
    
template<size_t buffer_size = 255>
class UART_ {

   uint8_t buffer[buffer_size];
   int begin {0};
   int end   {0};

public:

   template <Periph usart, class TXpin, class RXpin, class RTSpin, class LEDpin> static auto make()
   {
      UART_ uart;
      result << "Создаем объект UART" << '\n'; return uart;
   }
   void start_transmit();
   void start_receive ();
   uint16_t buffer_CRC();

   auto buffer_pointer(){result << "Возвращаем указатель на буфер" << '\n'; return &buffer[0];}
   auto buffer_end    (){return end;}
   void buffer_clean  (){result << "Очищаем буфер" << '\n'; begin = end = 0;}
   UART_&   operator<< (const uint8_t&);
   UART_&   operator<< (const uint16_t&);
   UART_&   operator>> (uint8_t&);
   UART_&   operator>> (uint16_t&);
   uint8_t  operator[] (const int index) {return buffer[index];}
   // uint16_t qty_byte();
};

using UART = UART_<>;

template<size_t buffer_size>
void UART_<buffer_size>::start_transmit()
{
   result << "Старт передачи" << '\n';
}

template<size_t buffer_size>
void UART_<buffer_size>::start_receive()
{
   buffer_clean();
   result << "Старт приема" << '\n';
}

template<size_t buffer_size>
uint16_t UART_<buffer_size>::buffer_CRC()
{
   result << "Получаем значение CRC полученного буфера" << '\n';
   uint8_t low  = buffer[end -1];
   uint8_t high = buffer[end];
   end -=2;
   return high << 8 | low;
}

template<size_t buffer_size>
UART_<buffer_size>& UART_<buffer_size>::operator<< (const uint8_t& v)
{
   result << "Добавляем в буфер новый 8-битный элемент" << '\n';
   buffer [end] = v;
   end ++;
   return *this;
}

template<size_t buffer_size>
UART_<buffer_size>& UART_<buffer_size>::operator>> (uint8_t& v)
{
   result << "Забираем из буфера 8-битный элемент" << '\n';
   v = buffer[begin];
   begin ++;
   if (begin == end) {
      begin = end = 0;
   }
   return *this;
}

template<size_t buffer_size>
UART_<buffer_size>& UART_<buffer_size>::operator<< (const uint16_t& v)
{
   result << "Добавляем в буфер новый 16-битный элемент" << '\n';
   uint8_t high, low;
   high = v >> 8; 
   low  = static_cast<uint8_t>(v);
   *this << high << low;
   return *this;
}

template<size_t buffer_size>
UART_<buffer_size>& UART_<buffer_size>::operator>> (uint16_t& v)
{
   result << "Забираем из буфера 16-битный элемент" << '\n';
   uint8_t high, low;
   *this >> high >> low;
   v = high << 8 | low;
   return *this;
}


} //namespace mcu

