#pragma once

#define USE_MOCK_UART
#include "mock_dma.h"
#include "mock_periph_usart.h"
#include "mock_afio.h"
#include "mock_gpio.h"
#include "mock_interrupt.h"
#include "uart.h"
#include "process.h"

namespace mock {

uint16_t CNDTR {255};

template<size_t buffer_size = 255>
struct UART_sized : ::UART_sized<buffer_size> {

   Process& process {Process::make()};
   auto& base() { return *static_cast<::UART_sized<buffer_size>*>(this); }

   template <
      mcu::Periph usart
      , class TXpin
      , class RXpin
      , class RTSpin
      , class LEDpin
   > static UART_sized& make()
   {
      return static_cast<UART_sized&>(
      ::UART_sized<buffer_size>::template make<usart,TXpin,RXpin,RTSpin,LEDpin>());
   }

   // void transmit() 
   // {
   //    process << "Включение передачи буфера: " << std::endl; // написать что передаётся
   //    if (go_deeper) base().transmit();
   // }

   bool is_tx_complete() {return true;}
   bool is_IDLE() {return true;}



};

using UART = UART_sized<>;

} // namespace mock {