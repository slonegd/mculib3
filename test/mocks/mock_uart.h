#pragma once

#define USE_MOCK_UART
#include "mock_dma.h"
#include "mock_periph_usart.h"
#if defined (STM32F1)
#include "mock_afio.h"
#endif
#include "mock_gpio.h"
#include "mock_interrupt.h"
#include "uart.h"
#include "process.h"

namespace mock {

// std::ostream& operator<< (std::ostream& s, mcu::Pin& pin)
// {
//    return pin.n == 9  ? s << "PA9"  :
//           pin.n == 10 ? s << "PA10" :
//           pin.n == 11 ? s << "PA11" : s;
// }

uint16_t CNDTR {255};

// template<size_t buffer_size = 255>
class UART_sized : public ::UART_sized<> {

   Process& process {Process::make()};
   UART_sized() = default;
   UART_sized(const ::UART_sized<>& v) : ::UART_sized<>(v)
   {
      process.clear();
      process << "Инициализация пинов, и " << uart_periph << std::endl;
   }
   
public:
   template <
      mcu::Periph usart
      , class TXpin
      , class RXpin
      , class RTSpin
      , class LEDpin
   > static auto& make()
   {
      static auto uart = UART_sized(::UART_sized<>::template make<usart,TXpin,RXpin,RTSpin,LEDpin>());
      return uart;
   }

   auto& base() { return *static_cast<::UART_sized<>*>(this); }

   void init(const ::UART_sized<>::Settings& set)
   {
      process << "настройка параметров uart" << std::endl;
      // base().init(set);
   }

   // void transmit() 
   // {
   //    process << "Включение передачи буфера: " << std::endl; // написать что передаётся
   //    if (go_deeper) base().transmit();
   // }

   bool is_tx_complete() {return true;}
   bool is_IDLE() {return true;}



};

using UART = UART_sized;

} // namespace mock {