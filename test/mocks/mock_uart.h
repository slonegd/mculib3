#pragma once

#define USE_MOCK_UART
#include "mock_dma.h"
#include "mock_periph_usart.h"
#include "mock_afio.h"
#include "mock_gpio.h"
#include "mock_interrupt.h"
#include "uart.h"

namespace mock {

template<size_t buffer_size = 255>
struct UART_sized : ::UART_sized<buffer_size> {

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
};

using UART = UART_sized<>;

} // namespace mock {