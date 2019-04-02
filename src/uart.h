#pragma once

#include <array>
#include <type_traits>
#include "periph_usart.h"
#include "periph_dma.h"
#include "pin.h"
#include "net_buffer.h"
#include "interrupt.h"

#if defined(USE_MOCK_DMA)
using DMA_stream = mock::DMA_stream;
using DMA = mock::DMA;
#else
using DMA_stream = mcu::DMA_stream;
using DMA = mcu::DMA;
#endif

#if defined(USE_MOCK_USART)
using USART = mock::USART;
#else
using USART = mcu::USART;
#endif

#if defined(USE_MOCK_NET_BUFFER)
#define NS mock
#else
#define NS
#endif

template <size_t buffer_size = 255>
class UART_sized
{
 public:
   using Parity = USART::Parity;
   using DataBits = USART::DataBits;
   using StopBits = USART::StopBits;
   using Baudrate = USART::Baudrate;
   struct Settings
   {
      bool parity_enable : 1;
      Parity parity : 1;
      DataBits data_bits : 1;
      StopBits stop_bits : 2;
      Baudrate baudrate : 3;
      uint16_t res : 8;
   };

   NS::Net_buffer<buffer_size> buffer;

   template <
       mcu::Periph usart, class TXpin, class RXpin, class RTSpin>
   static auto &make();

   void init(const Settings &);
   void transmit();
   void receive();
   bool is_tx_complete();
   bool is_rx_IDLE();
   bool is_receiving() { return buffer.size() < (buffer_size - RXstream.qty_transactions_left()); }

 protected:
   using DataSize = DMA_stream::DataSize;
   using Priority = DMA_stream::Priority;
   using Channel = DMA_stream::Channel;
   using Direction = DMA_stream::Direction;

   Pin &tx;
   Pin &rx;
   Pin &rts;
   DMA &dma;
   USART &usart;
   DMA_stream &TXstream;
   DMA_stream &RXstream;
   const mcu::Periph uart_periph;
   const Channel TX_channel;
   // const Channel RX_channel;

   UART_sized(
       Pin &tx, Pin &rx, Pin &rts, USART &usart, DMA &dma, DMA_stream &TXstream, DMA_stream &RXstream, mcu::Periph uart_periph, Channel TX_channel) : tx{tx}, rx{rx}, rts{rts}, dma{dma}, usart{usart}, TXstream{TXstream}, RXstream{RXstream}, uart_periph{uart_periph}, TX_channel{TX_channel}
   { 
   
   }

   //  UART_sized(const UART_sized&) = delete;
   UART_sized &operator=(const UART_sized &) = delete;
};

using UART = UART_sized<>;

template <size_t buffer_size>
template <mcu::Periph uart_periph, class TXpin, class RXpin, class RTSpin>
auto &UART_sized<buffer_size>::make()
{
   USART::pin_static_assert<uart_periph, TXpin, RXpin>();

   constexpr auto TX_stream = USART::default_stream<TXpin>();
   constexpr auto RX_stream = USART::default_stream<RXpin>();
   constexpr auto dma_periph = DMA_stream::dma_periph<TX_stream>();
   constexpr auto TXpin_mode = USART::pin_mode<TXpin>();
   constexpr auto RXpin_mode = USART::pin_mode<RXpin>();

   static UART_sized<buffer_size> uart{
       Pin::make<TXpin, TXpin_mode>(), Pin::make<RXpin, RXpin_mode>(), Pin::make<RTSpin, mcu::PinMode::Output>(), mcu::make_reference<uart_periph>(), mcu::make_reference<dma_periph>(), mcu::make_reference<TX_stream>(), mcu::make_reference<RX_stream>(), uart_periph, DMA_stream::channel<uart_periph, TX_stream>()};

   auto &rcc = REF(RCC);
   rcc.clock_enable<uart_periph>();
   uart.usart.tx_enable()
       .rx_enable()
       .DMA_tx_enable()
       .DMA_rx_enable();
   get_interrupt<uart_periph>().enable();

   rcc.clock_enable<dma_periph>();
   uart.TXstream.set(Direction::to_periph)
       .set_memory_adr(size_t(uart.buffer.begin()))
       .set_periph_adr(uart.usart.transmit_data_adr())
       .inc_memory()
       .size_memory(DataSize::byte8)
       .size_periph(DataSize::byte8)
       .enable_transfer_complete_interrupt();
   get_interrupt<TX_stream>().enable();

   uart.RXstream.set(Direction::to_memory)
       .set_memory_adr(size_t(uart.buffer.begin()))
       .set_periph_adr(uart.usart.receive_data_adr())
       .set_qty_transactions(buffer_size)
       .inc_memory()
       .size_memory(DataSize::byte8)
       .size_periph(DataSize::byte8)
       .circular_mode();

   #if defined(STM32F4) or defined(STM32F1)
      uart.RXstream.select_channel(DMA_stream::select_channel<uart_periph, RX_stream>());
      uart.TXstream.select_channel(DMA_stream::select_channel<uart_periph, TX_stream>());
   #endif
   
   return uart;
}

template <size_t buffer_size>
void UART_sized<buffer_size>::init(const UART_sized<buffer_size>::Settings &set)
{
   usart.set(set.baudrate, uart_periph)
       .set(set.parity)
       .set(set.data_bits)
       .set(set.stop_bits)
       .parity_enable(set.parity_enable)
       .enable();
}

template <size_t buffer_size>
void UART_sized<buffer_size>::transmit()
{
   rts = true;
   usart.enable_IDLE_interrupt (false);
   RXstream.disable();
   TXstream.disable();
   // +2 потому что теряется последние 2 байта (какой то глюк)
   TXstream.set_qty_transactions(buffer.size() + 2);
   TXstream.enable();
}

template <size_t buffer_size>
void UART_sized<buffer_size>::receive()
{
   buffer.clear();
   rts = false;
   TXstream.disable();
   RXstream.disable()
       .set_qty_transactions(buffer_size);
   RXstream.enable();
   usart.enable_IDLE_interrupt();
}

template <size_t buffer_size>
bool UART_sized<buffer_size>::is_rx_IDLE()
{
   auto res = usart.is_IDLE_interrupt();
   if (res)
      buffer.set_size(buffer_size - RXstream.qty_transactions_left());
   return res and buffer.size();
}

template <size_t buffer_size>
bool UART_sized<buffer_size>::is_tx_complete()
{
   return dma.is_transfer_complete_interrupt(TX_channel);
}
