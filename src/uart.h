#pragma once

#include <array>
#include <type_traits>
#include "periph_usart.h"
#include "periph_dma.h"
#include "pin.h"
#include "net_buffer.h"

#include "mock_def.h"

template<size_t buffer_size = 255>
class UART_sized
{
public:
   using Parity   = USART_t::Parity;
   using DataBits = USART_t::DataBits;
   using StopBits = USART_t::StopBits;
   using Baudrate = USART_t::Baudrate;
   struct Settings {
      bool     parity_enable :1;
      Parity   parity        :1;
      DataBits data_bits     :1;
      StopBits stop_bits     :2;
      Baudrate baudrate      :3;
      uint16_t res           :8;
   };

   Net_buffer<buffer_size> buffer;

   template <
        mcu::Periph usart
      , class TXpin
      , class RXpin
      , class RTSpin
      , class LEDpin
   > static auto& make();

   void init (const Settings&);
   void transmit();
   void receive();
   bool is_tx_complete();
   bool is_IDLE();
   bool is_receiving() { return buffer.size() < (buffer_size - RXstream.qty_transactions_left()); } 
   int  modbus_time (Baudrate); // ??



private:
   using WakeMethod     = USART_t::WakeMethod;
   using BreakDetection = USART_t::BreakDetection;
   using DataSize       = DMA_stream_t::DataSize;
   using Priority       = DMA_stream_t::Priority;
   using DataDirection  = DMA_stream_t::DataDirection;

   Pin&          tx;
   Pin&          rx;
   Pin&          rts;
   Pin&          led;
   USART_t&      usart;
   DMA_stream_t& TXstream;
   DMA_stream_t& RXstream;
   const mcu::Periph uart_periph;

   UART_sized (
        Pin&          tx
      , Pin&          rx
      , Pin&          rts
      , Pin&          led
      , USART_t&      usart
      , DMA_stream_t& TXstream
      , DMA_stream_t& RXstream
      , mcu::Periph uart_periph
   )  : tx       {tx}
      , rx       {rx}
      , rts      {rts}
      , led      {led}
      , usart    {usart}
      , TXstream {TXstream}
      , RXstream {RXstream}
      , uart_periph {uart_periph}
   {}
};

using UART = UART_sized<>;
































template<size_t buffer_size>
template <mcu::Periph uart_periph, class TXpin, class RXpin, class RTSpin, class LEDpin> 
auto& UART_sized<buffer_size>::make()
{
   USART_t::pin_static_assert<uart_periph, TXpin, RXpin>();
   
   constexpr auto TX_stream  = USART_t::default_stream<TXpin>();
   constexpr auto RX_stream  = USART_t::default_stream<RXpin>();
   constexpr auto TXpin_mode = USART_t::pin_mode<TXpin>();
   constexpr auto RXpin_mode = USART_t::pin_mode<RXpin>();

   static UART_sized<buffer_size> uart {
        Pin::make<TXpin, TXpin_mode>()
      , Pin::make<RXpin, RXpin_mode>()
      , Pin::make<RTSpin, mcu::PinMode::Output>()
      , Pin::make<LEDpin, mcu::PinMode::Output>()
      , mcu::make_reference<uart_periph>()
      , mcu::make_reference<TX_stream>()
      , mcu::make_reference<RX_stream>()
      , uart_periph
   };

   auto& rcc = REF(RCC);
   rcc.clock_enable<uart_periph>();
   uart.usart.tx_enable()
             .rx_enable()
             .DMA_tx_enable()
             .DMA_rx_enable()
             .enable_IDLE_interrupt()
             .enable();
   NVIC_EnableIRQ_t(uart.usart.IRQn(uart_periph));

   rcc.clock_enable<TX_stream>();
   uart.TXstream.direction(DataDirection::MemToPer)
                .set_memory_adr(size_t(uart.buffer.begin()))
                .set_periph_adr(uart.usart.transmit_data_adr())
                .inc_memory()
                .size_memory(DataSize::byte8)
                .size_periph(DataSize::byte8)
                .enable_transfer_complete_interrupt();
      NVIC_EnableIRQ_t(uart.TXstream.IRQn(TX_stream));

   uart.RXstream.direction(DataDirection::PerToMem)
            	 .set_memory_adr(size_t(uart.buffer.begin()))
            	 .set_periph_adr(uart.usart.transmit_data_adr())
            	 .set_qty_transactions(buffer_size)
            	 .inc_memory()
            	 .size_memory(DataSize::byte8)
            	 .size_periph(DataSize::byte8);

   return uart;
}

template<size_t buffer_size>
void UART_sized<buffer_size>::init (const UART_sized<buffer_size>::Settings& set) 
{
   usart.set(set.baudrate, uart_periph)
        .set(set.parity)
        .set(set.data_bits)
        .set(set.stop_bits);
}

template<size_t buffer_size>
void UART_sized<buffer_size>::transmit()
{
   rts = led = true;
   RXstream.disable();
   TXstream.disable()
           .set_qty_transactions(buffer.size())
           .enable();
}

template<size_t buffer_size>
void UART_sized<buffer_size>::receive()
{
   buffer.clear();
   rts = led = false;
   TXstream.disable();
   RXstream.disable()
           .enable();
}

template<size_t buffer_size>
int UART_sized<buffer_size>::modbus_time (Baudrate baudrate)
{
   return baudrate == Baudrate::BR9600  ? 4 :
          baudrate == Baudrate::BR14400 ? 3 :
          baudrate == Baudrate::BR19200 ? 2 :
          baudrate == Baudrate::BR28800 ? 2 : 1;
}

template<size_t buffer_size>
bool UART_sized<buffer_size>::is_IDLE()
{
   auto res = usart.is_IDLE_interrupt();
   if (res)
      buffer.set_size (buffer_size - RXstream.qty_transactions_left());
   return res;
}

template<size_t buffer_size>
bool UART_sized<buffer_size>::is_tx_complete()
{
   return usart.is_tx_complete();
}

#undef NS