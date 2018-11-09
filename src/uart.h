#pragma once

#include "usart.h"

namespace mcu {

template<size_t buffer_size = 255>
class UART {
   USART& usart;
   DMA_stream& TXstream;
   DMA_stream& RXstream;
   const size_t clock;
   Pin tx;
   Pin rx;
   uint8_t buffer[buffer_size];
   
public:

   using Parity         = USART::Parity;
   using WakeMethod     = USART::WakeMethod;
   using DataBits       = USART::DataBits;
   using BreakDetection = USART::BreakDetection;
   using StopBits       = USART::StopBits;
   using Channel        = USART::Channel;
   using DataSize       = DMA_stream::DataSize;
   using Priority       = DMA_stream::Priority;
   using DataDirection  = DMA_stream::DataDirection;

   UART (USART& usart, DMA_stream& TXstream, DMA_stream& RXstream, size_t clock, Pin tx, Pin rx)
      : usart    {usart}
      , TXstream {TXstream}
      , RXstream {RXstream}
      , clock    {clock}
      , tx       {tx}
      , rx       {rx}
   {}

   struct Settings {
      Parity         parity    = Parity::even;
      DataBits       data_bits = DataBits::_8;
      StopBits       stop_bits = StopBits::_1;
      Baudrate       baudrate  = Baudrate::BR9600;
   };

   template <Periph usart, class TXpin, class RXpin> static auto make()
   {
      if constexpr (usart == Periph::USART1) {
         static_assert (
            (std::is_same_v<TXpin, PA9> and std::is_same_v<RXpin, PA10>) or
            (std::is_same_v<TXpin, PB6> and std::is_same_v<RXpin, PB7>),
            "USART1 возможен только с парами пинов TX/PA9, RX/PA10 или TX/PB6, RX/PB7");
      } else if constexpr (usart == Periph::USART2) {
         static_assert (
            (std::is_same_v<TXpin, PA2> and std::is_same_v<RXpin, PA3>) or
            (std::is_same_v<TXpin, PD5> and std::is_same_v<RXpin, PD6>),
            "USART2 возможен только с парами пинов TX/PA2, RX/PA3 или TX/PD5, RX/PD6");
      } else if constexpr (usart == Periph::USART3) {
         static_assert (
            (std::is_same_v<TXpin, PB10> and std::is_same_v<RXpin, PB11>) or
            (std::is_same_v<TXpin, PC10> and std::is_same_v<RXpin, PC11>) or
            (std::is_same_v<TXpin, PD8>  and std::is_same_v<RXpin, PD9>),
            "USART3 возможен только с парами пинов TX/PB10, RX/PB11 или TX/PC10, RX/PC11 или TX/PD8, RX/PD9");
      }
      
      constexpr auto TX_stream  = USART::stream  <usart, TXpin>();
      constexpr auto RX_stream  = USART::stream  <usart, RXpin>();
      constexpr auto TXpin_mode = USART::pin_mode<usart, TXpin>();
      constexpr auto RXpin_mode = USART::pin_mode<usart, RXpin>();

      UART uart {
           make_reference<usart>()
         , make_reference<TX_stream>()
         , make_reference<RX_stream>()
         , USART::clock<usart>()
         , Pin::make<TXpin, TXpin_mode>()
         , Pin::make<RXpin, RXpin_mode>()
      };

      uart.usart.template clock_enable<usart>()
                .tx_enable()
                .rx_enable()
                .DMA_tx_enable()
                .DMA_rx_enable()
                .enable();

      uart.TXstream.template clock_enable();
      uart.RXstream.template clock_enable();

      uart.TXstream.disable()
              .direction(DataDirection::MemToPer)
              .set_memory_adr(uint32_t(uart.buffer))
              .set_periph_adr(uart.usart.transmit_data_adr())
              .inc_memory()
              .size_memory(DataSize::byte8)
              .size_periph(DataSize::byte8)
              .enable_transfer_complete_interrupt();
        NVIC_EnableIRQ(uart.TXstream.IRQn(TX_stream));

      uart.RXstream.direction(DataDirection::PerToMem)
              .set_memory_adr(uint32_t(uart.buffer))
              .set_periph_adr(uart.usart.transmit_data_adr())
              .set_qty_transactions(buffer_size)
              .inc_memory()
              .size_memory(DataSize::byte8)
              .size_periph(DataSize::byte8);
      //         .enable_transfer_complete_interrupt();
      //   NVIC_EnableIRQ(RXstream.IRQn(RX_stream));

      return uart;
   }
      
   void init (const Settings& set) 
   {
      usart.set(set.baudrate, clock)
           .set(set.parity)
           .set(set.data_bits)
           .set(set.stop_bits);
   }

   void transmit(uint16_t qty)
   {
      TXstream.set_qty_transactions(qty)
              .enable();
   }
   void start_receive(uint8_t buffer[])
   {
      RXstream.disable()
              .enable();
   }

};
} // namespace mcu

