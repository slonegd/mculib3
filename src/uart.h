
// Не закончен, писался под F4


#pragma once

#include "usart.h"

namespace mcu {

class UART {
   USART& usart;
   DMA_stream& stream;
   const size_t clock;
public:
   UART (USART& usart, size_t clock)
      : usart {usart}
      , clock {clock}
   {}

   template <Periph p, class RXpin, class TXpin, class RTSpin> static auto make()
   {
      UART uart {make_reference<p>(), USART::clock<p>()};
      uart.usart.clock_enable<p>();
      uart.usart.DMA_channel(p);
      uart.usart.IRQn(p);
      uart.usart.pin_mode(p);
      static_assert (
         uart.usart.is_rx_support<RXpin, p>(),
         "выбранный вывод не поддерживает функцию RX");
      static_assert (
         uart.usart.is_tx_support<TXpin, p>(),
         "выбранный вывод не поддерживает функцию TX");
      static_assert (
         uart.usart.is_rts_support<RTSpin, p>(),
         "выбранный вывод не поддерживает функцию RTS");
      return uart;
   }


};
} // namespace mcu

