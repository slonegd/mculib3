#pragma once

#if not defined (TEST)
	#include "usart.h"
#endif
namespace mcu {

template<size_t buffer_size = 255>
class UART_ {
   USART& usart;
   DMA_stream& TXstream;
   DMA_stream& RXstream;
   const size_t clock;
   Pin tx;
   Pin rx;
   Pin rts;
   Pin led;
   uint8_t buffer[buffer_size];
   
public:

   using Parity         = USART::Parity;
   using WakeMethod     = USART::WakeMethod;
   using DataBits       = USART::DataBits;
   using BreakDetection = USART::BreakDetection;
   using StopBits       = USART::StopBits;
   using Baudrate       = USART::Baudrate;
   using DataSize       = DMA_stream::DataSize;
   using Priority       = DMA_stream::Priority;
   using DataDirection  = DMA_stream::DataDirection;

   UART_ (
        USART& usart
      , DMA_stream& TXstream
      , DMA_stream& RXstream
      , size_t clock
      , Pin tx
      , Pin rx
      , Pin rts
      , Pin led
   )  : usart    {usart}
      , TXstream {TXstream}
      , RXstream {RXstream}
      , clock    {clock}
      , tx       {tx}
      , rx       {rx}
      , rts      {rts}
      , led      {led}
   {}

   struct Settings {
      bool     parity_enable :1;
      Parity   parity        :1;
      DataBits data_bits     :1;
      StopBits stop_bits     :2;
      Baudrate baudrate      :3;
      uint16_t res           :8;
   };

   template <Periph usart, class TXpin, class RXpin, class RTSpin, class LEDpin> static auto make()
   {
      USART::pin_static_assert<usart, TXpin, RXpin>();
      
      constexpr auto TX_stream  = USART::stream  <usart, TXpin>();
      constexpr auto RX_stream  = USART::stream  <usart, RXpin>();
      constexpr auto TXpin_mode = USART::pin_mode<usart, TXpin>();
      constexpr auto RXpin_mode = USART::pin_mode<usart, RXpin>();

      UART_ uart {
           make_reference<usart>()
         , make_reference<TX_stream>()
         , make_reference<RX_stream>()
         , USART::clock<usart>()
         , Pin::make<TXpin, TXpin_mode>()
         , Pin::make<RXpin, RXpin_mode>()
         , Pin::make<RTSpin, PinMode::Output>()
         , Pin::make<LEDpin, PinMode::Output>()
      };

      uart.usart.template clock_enable<usart>()
                .tx_enable()
                .rx_enable()
                .DMA_tx_enable()
                .DMA_rx_enable()
                .enable_IDLE_interrupt()
                .enable();
         NVIC_EnableIRQ(uart.usart.IRQn(usart));

      uart.TXstream.template clock_enable()
                   .disable()
                   .direction(DataDirection::MemToPer)
                   .set_memory_adr(reinterpret_cast<size_t>(uart.buffer))
                   .set_periph_adr(uart.usart.transmit_data_adr())
                   .inc_memory()
                   .size_memory(DataSize::byte8)
                   .size_periph(DataSize::byte8)
                   .enable_transfer_complete_interrupt();
         NVIC_EnableIRQ(uart.TXstream.IRQn(TX_stream));

      uart.RXstream.direction(DataDirection::PerToMem)
              		 .set_memory_adr(reinterpret_cast<size_t>(uart.buffer))
              		 .set_periph_adr(uart.usart.transmit_data_adr())
              		 .set_qty_transactions(buffer_size)
              		 .inc_memory()
              		 .size_memory(DataSize::byte8)
              		 .size_periph(DataSize::byte8);

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
      rts = led = true;
      RXstream.disable();
      TXstream.disable()
              .set_qty_transactions(qty)
              .enable();

   }
   void start_receive(uint8_t buffer[])
   {
      rts = led = false;
      TXstream.disable();
      RXstream.disable()
              .enable();
   }

};

using UART = UART_<>;

} // namespace mcu

