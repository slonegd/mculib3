#pragma once

#include <array>
#include <utility>
#include "test_periph.h"
#include "mock_pin.h"
#include "mock_usart.h"
#include "mock_interrupt.h"

uint8_t buffer[255]{0};
   int begin_ {0};
   int end_   {0};

namespace mcu {

uint16_t CNDTR {255};

template<size_t buffer_size = 255>
class UART_
{
   // Interrupt& interrupt_usart;

public:

   // UART_ (Interrupt& interrupt_usart) : interrupt_usart {interrupt_usart} {}
   // UART_ (UART_&& v) : interrupt_usart {v.interrupt_usart} {}
   // ~UART_() { interrupt_usart.unsubscribe(&uart_); }
   
   template <Periph usart, class TXpin, class RXpin, class RTSpin, class LEDpin> static auto make()
   {
      // auto interrupt_usart = usart == Periph::USART1 ? &interrupt_usart1 :
      //                        usart == Periph::USART2 ? &interrupt_usart2 :
      //                        usart == Periph::USART3 ? &interrupt_usart3 :
      //                        nullptr;
      
      UART_ uart; 
      // UART_ uart {*interrupt_usart};
      result << "Создаем объект UART" << '\n'; 

      return uart;
   }
   using Baudrate  = USART::Baudrate;
   struct Settings {
      Baudrate baudrate;
   };

   void init (const Settings& set) {result << "Инициализация uart" << '\n';}
   void start_transmit();
   void start_receive ();
   uint16_t pop_back();

   bool is_IDLE(){return true;}
   bool is_tx_complete(){return true;}
   size_t message_size(){return buffer_size - CNDTR;}
   void tx_complete_handler(){}

   int modbus_time (Baudrate) {result << "Определение время задержки для модбаса" << '\n'; return 4;}
   auto begin(){result << "Возвращаем указатель на буфер для расчета CRC" << '\n'; return &buffer[0];}
   auto end    (){ result << "Берем значение end" << '\n'; return &buffer[end_];}
   void buffer_clean  (){result << "Очищаем буфер" << '\n'; begin_ = end_ = 0;}
   UART_&   operator<< (const std::array<uint8_t, 8> &arr){std::copy(arr.begin(), arr.end(), buffer); return *this;}
   UART_&   operator<< (const uint8_t&);
   UART_&   operator<< (const uint16_t&);
   UART_&   operator>> (uint8_t&);
   UART_&   operator>> (uint16_t&);
   uint8_t  operator[] (const int index) {return buffer[index];}
   void interrupt () {end_ = message_size();}
   uint16_t qty_byte();

   // using Parent = UART_;
   // // структура создана потому что не срабатывало RVO у parent при подписке в make()
   // struct uart_interrupt : Interrupting
   // {
   //    Parent& parent;
   //    uart_interrupt (Parent& parent) : parent(parent) {
   //       parent.interrupt_usart.subscribe (this);
   //    }
   //    void interrupt() override {parent.interrupt();} 
   // } uart_ {*this};
};

using UART = UART_<>;

// template <Periph usart, class TXpin, class RXpin, class RTSpin, class LEDpin> auto make_uart()
// {
//    auto interrupt_usart = usart == Periph::USART1 ? &interrupt_usart1 :
//                            usart == Periph::USART2 ? &interrupt_usart2 :
//                            usart == Periph::USART3 ? &interrupt_usart3 :
//                            nullptr;
   
//    // UART_ uart; 
//    UART uart {*interrupt_usart};
//    result << "Создаем объект UART" << '\n'; 

//    return uart;
// }

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
uint16_t UART_<buffer_size>::pop_back()
{
   result << "Получаем значение CRC полученного буфера" << '\n';
   uint8_t v  = buffer[--end_];
   return v;
}

template<size_t buffer_size>
UART_<buffer_size>& UART_<buffer_size>::operator<< (const uint8_t& v)
{
   result << "Добавляем в буфер новый 8-битный элемент" << '\n';
   buffer [end_++] = v;
   return *this;
}

template<size_t buffer_size>
UART_<buffer_size>& UART_<buffer_size>::operator>> (uint8_t& v)
{
   result << "Забираем из буфера 8-битный элемент" << '\n';
   v = buffer[begin_++];
   if (begin_ == end_) {
      begin_ = end_ = 0;
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

