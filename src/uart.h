#pragma once


#include <array>
#if not defined (TEST)
	#include "usart.h"
   #include "interrupt.h"
#endif
namespace mcu {

template<size_t buffer_size = 255>
class UART_
{
   USART& usart;
   DMA_stream& TXstream;
   DMA_stream& RXstream;
   // Interrupt& interrupt_usart;
   const size_t clock;
   Pin tx;
   Pin rx;
   Pin rts;
   Pin led;
   uint8_t buffer[buffer_size];
   uint8_t begin_ {0};
   uint8_t end_   {0};
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
      // , Interrupt& interrupt_usart
      , size_t clock
      , Pin tx
      , Pin rx
      , Pin rts
      , Pin led
   )  : usart    {usart}
      , TXstream {TXstream}
      , RXstream {RXstream}
      // , interrupt_usart{interrupt_usart}
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

   template <Periph usart, class TXpin, class RXpin, class RTSpin, class LEDpin> static auto make();
   void init (const Settings&);
   void transmit(uint16_t qty);
   void start_transmit();
   void start_receive();
   bool is_tx_complete();
   uint16_t CNDTR();
   bool is_IDLE();
   
   int modbus_time (Baudrate);
   auto begin         (){return &buffer[0];}
   auto end           (){return &buffer[end_];}
   auto message_size  (){return buffer_size - RXstream.qty_transactions_left();}
   void interrupt     (){end_ = message_size();} 
   void buffer_clean  (){begin_ = end_ = 0;}
   UART_&   operator<< (const std::array<uint8_t, 8> &arr){std::copy(arr.begin(), arr.end(), buffer); return *this;}
   UART_&   operator<< (const uint8_t&);
   UART_&   operator<< (const uint16_t&);
   UART_&   operator>> (uint8_t&);
   UART_&   operator>> (uint16_t&);
   uint8_t  operator[] (const int index) {return buffer[index];}
   uint16_t qty_byte(){return buffer_size - RXstream.qty_transactions_left();}
   uint16_t pop_back();
   void push_back(const uint16_t&); // метод записывает переставляя старший и младший биты, сейчас не используется

   // using Parent = UART_;
   // // структура создана потому что не срабатывало NRVO у parent при подписке в make()
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






































template<size_t buffer_size>
template <Periph usart, class TXpin, class RXpin, class RTSpin, class LEDpin> 
auto UART_<buffer_size>::make()
{
   USART::pin_static_assert<usart, TXpin, RXpin>();
   
   constexpr auto TX_stream  = USART::stream  <usart, TXpin>();
   constexpr auto RX_stream  = USART::stream  <usart, RXpin>();
   constexpr auto TXpin_mode = USART::pin_mode<usart, TXpin>();
   constexpr auto RXpin_mode = USART::pin_mode<usart, RXpin>();

   // auto interrupt_usart = usart == Periph::USART1 ? &interrupt_usart1 :
   //                        usart == Periph::USART2 ? &interrupt_usart2 :
   //                        usart == Periph::USART3 ? &interrupt_usart3 :
   //                        nullptr;

   UART_ uart {
        make_reference<usart>()
      , make_reference<TX_stream>()
      , make_reference<RX_stream>()
      // , *interrupt_usart
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

   
   
   // interrupt_usart->subscribe(&uart);

   return uart;
}

template<size_t buffer_size>
void UART_<buffer_size>::init (const UART_<buffer_size>::Settings& set) 
{
   usart.set(set.baudrate, clock)
        .set(set.parity)
        .set(set.data_bits)
        .set(set.stop_bits);
}

template<size_t buffer_size>
void UART_<buffer_size>::transmit(uint16_t qty)
{
   rts = led = true;
   RXstream.disable();
   TXstream.disable()
           .set_qty_transactions(qty)
           .enable();
}

template<size_t buffer_size>
void UART_<buffer_size>::start_transmit()
{
   rts = led = true;
   RXstream.disable();
   TXstream.disable()
           .set_qty_transactions(end)
           .enable();
}

template<size_t buffer_size>
void UART_<buffer_size>::start_receive()
{
   buffer_clean();
   rts = led = false;
   TXstream.disable();
   RXstream.disable()
           .enable();
}

template<size_t buffer_size>
int UART_<buffer_size>::modbus_time (Baudrate baudrate)
{
   return baudrate == Baudrate::BR9600  ? 4 :
          baudrate == Baudrate::BR14400 ? 3 :
          baudrate == Baudrate::BR19200 ? 2 :
          baudrate == Baudrate::BR28800 ? 2 : 1;
}

template<size_t buffer_size>
bool UART_<buffer_size>::is_IDLE()
{
   return usart.is_IDLE_interrupt();
}

template<size_t buffer_size>
bool UART_<buffer_size>::is_tx_complete()
{
   return usart.is_tx_complete();
}

template<size_t buffer_size>
uint16_t UART_<buffer_size>::CNDTR()
{
   return RXstream.qty_transactions_left();
}

template<size_t buffer_size>
UART_<buffer_size>& UART_<buffer_size>::operator<< (const uint8_t& v)
{
   buffer [end_++] = v;
   return *this;
}

template<size_t buffer_size>
UART_<buffer_size>& UART_<buffer_size>::operator>> (uint8_t& v)
{
   v = buffer[begin_++];
   if (begin_ == end_) {
      begin_ = end_ = 0;
   }
   return *this;
}

template<size_t buffer_size>
UART_<buffer_size>& UART_<buffer_size>::operator<< (const uint16_t& v)
{
   uint8_t high, low;
   high = v >> 8; 
   low  = static_cast<uint8_t>(v);
   *this << high << low;
   return *this;
}

template<size_t buffer_size>
UART_<buffer_size>& UART_<buffer_size>::operator>> (uint16_t& v)
{
   uint8_t high, low;
   *this >> high >> low;
   v = high << 8 | low;
   return *this;
}

template<size_t buffer_size>
uint16_t UART_<buffer_size>::pop_back() 
{
   uint8_t v  = buffer[--end_];
   return v;
}

template<size_t buffer_size>
void UART_<buffer_size>::push_back(const uint16_t& v)
{
   buffer[end_++] = static_cast<uint8_t>(v);
   buffer[end_++] = v >> 8;
}

} // namespace mcu

