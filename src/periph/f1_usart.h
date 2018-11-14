#pragma once

#include <type_traits>
#include "f1_bits_usart.h"
#include "rcc.h"
#include "pin.h"
#include "dma.h"

namespace mcu {

class USART_default
{
public:
   __IO USART_bits::SR  SR;   // USART Status register,                   offset: 0x00
   __IO uint32_t        DR;   // USART Data register,                     offset: 0x04
   __IO uint32_t        BRR;  // USART Baud rate register,                offset: 0x08
   __IO USART_bits::CR1 CR1;  // USART Control register 1,                offset: 0x0C
   __IO USART_bits::CR2 CR2;  // USART Control register 2,                offset: 0x10
   __IO USART_bits::CR3 CR3;  // USART Control register 3,                offset: 0x14
   __IO uint32_t        GTPR; // USART Guard time and prescaler register, offset: 0x18
   // this thing for test
   auto read_SR() { return *reinterpret_cast<__IO uint32_t*>(&SR); }
};

template<class R = USART_default>
class USART_ : private R {
public:

   using CMSIS_type     = USART_TypeDef;
   using Parity         = USART_bits::CR1::Parity;
   using WakeMethod     = USART_bits::CR1::WakeMethod;
   using DataBits       = USART_bits::CR1::DataBits;
   using BreakDetection = USART_bits::CR2::BreakDetection;
   using StopBits       = USART_bits::CR2::StopBits;

   enum Baudrate {
      BR9600   = 0b000,
      BR14400  = 0b001,
      BR19200  = 0b010,
      BR28800  = 0b011,
      BR38400  = 0b100,
      BR57600  = 0b101,
      BR76800  = 0b110,
      BR115200 = 0b111
   };

   USART_& set (Parity         v)         {this->CR1.PS    = v; return *this;}
   USART_& set (WakeMethod     v)         {this->CR1.WAKE  = v; return *this;}
   USART_& set (DataBits       v)         {this->CR1.M     = v; return *this;}
   USART_& set (BreakDetection v)         {this->CR2.LBDL  = v; return *this;}
   USART_& set (StopBits       v)         {this->CR2.STOP  = v; return *this;}
   USART_& send_byte (uint8_t  v)         {this->DR = v;        return *this;}
   USART_& set (Baudrate, size_t);

   template <Periph usart, Periph rcc = Periph::RCC>
   USART_& clock_enable  (){make_reference<rcc>().template clock_enable<usart>(); return *this;}
   USART_& enable        (){this->CR1.UE   = true;  return *this;}
   USART_& disable       (){this->CR1.UE   = false; return *this;}
   USART_& rx_enable     (){this->CR1.RE   = true;  return *this;}
   USART_& rx_disable    (){this->CR1.RE   = false; return *this;}
   USART_& tx_enable     (){this->CR1.TE   = true;  return *this;}
   USART_& tx_disable    (){this->CR1.TE   = false; return *this;}
   USART_& rts_enable    (){this->CR3.RTSE = true;  return *this;}
   USART_& rts_disable   (){this->CR3.RTSE = false; return *this;}
   USART_& DMA_tx_enable (){this->CR3.DMAT = true;  return *this;}
   USART_& DMA_rx_enable (){this->CR3.DMAR = true;  return *this;}
   USART_& parity_enable (){this->CR1.PCE  = true;  return *this;}
   USART_& parity_disable(){this->CR1.PCE  = false; return *this;}

   USART_& enable_IDLE_interrupt          (){this->CR1.IDLEIE = true;  return *this;}
   bool    is_IDLE_interrupt              (){return this->SR.IDLE;}
   USART_& enable_tx_complete_interrupt   (){this->CR1.TCIE   = true;  return *this;}
   USART_& disable_tx_complete_interrupt  (){this->CR1.TCIE   = false; return *this;}
   bool    is_tx_complete                 (){return this->SR.TC;}
   bool    is_tx_complete_interrupt_enable(){return this->CR1.TCIE;}
   USART_& clear_interrupt_flags          (){
      [[maybe_unused]] uint32_t t1 = this->read_SR();
      [[maybe_unused]] uint32_t t2 = this->DR;
      return *this;
   }

   size_t receive_data_adr () {return reinterpret_cast<size_t>(&this->DR);}
   size_t transmit_data_adr() {return reinterpret_cast<size_t>(&this->DR);}

   
   static constexpr IRQn_Type IRQn(Periph);
   template<Periph usart, class Pin> static constexpr Periph stream();
   template<Periph usart, class Pin> static constexpr PinMode pin_mode();
   template<Periph usart, class TXpin, class RXpin> static void pin_static_assert();
   template<Periph usart, Periph rcc = Periph::RCC> static size_t clock();
};

using USART = USART_<>;



template <Periph p> std::enable_if_t<p == Periph::USART1, USART&> make_reference() {return *reinterpret_cast<USART*>(USART1_BASE);}
template <Periph p> std::enable_if_t<p == Periph::USART2, USART&> make_reference() {return *reinterpret_cast<USART*>(USART2_BASE);}
template <Periph p> std::enable_if_t<p == Periph::USART3, USART&> make_reference() {return *reinterpret_cast<USART*>(USART3_BASE);}




















template <class R>
USART_<R>& USART_<R>::set(Baudrate baudrate, size_t clock)
{
   switch (baudrate) {
      case BR9600:
         this->BRR = clock/9600;  return *this;
      case BR14400:
         this->BRR = clock/14400; return *this;
      case BR19200:
         this->BRR = clock/19200; return *this;
      case BR28800:
         this->BRR = clock/28800; return *this;
      case BR38400:
         this->BRR = clock/38400; return *this;
      case BR57600:
         this->BRR = clock/57600; return *this;
      case BR76800:
         this->BRR = clock/76800; return *this;
      case BR115200:
         this->BRR = clock/115200;return *this;
   }
}

template <class R>
template <Periph usart, class Pin> constexpr PinMode USART_<R>::pin_mode()
{
   if constexpr (usart == Periph::USART1) {
      if      constexpr (std::is_same_v<Pin, PA9>  or std::is_same_v<Pin, PB6>) return PinMode::USART1_TX; 
      else if constexpr (std::is_same_v<Pin, PA10> or std::is_same_v<Pin, PB7>) return PinMode::USART1_RX;
      else return PinMode::Input;
   }
   else if constexpr (usart == Periph::USART2) {
      if      constexpr (std::is_same_v<Pin, PA2> or std::is_same_v<Pin, PD5>) return PinMode::USART2_TX; 
      else if constexpr (std::is_same_v<Pin, PA3> or std::is_same_v<Pin, PD6>) return PinMode::USART2_RX;
      else return PinMode::Input;
   }
   else if constexpr (usart == Periph::USART3) {
      if      constexpr (std::is_same_v<Pin, PB10> or std::is_same_v<Pin, PC10> or std::is_same_v<Pin, PD8>) return PinMode::USART3_TX; 
      else if constexpr (std::is_same_v<Pin, PB11> or std::is_same_v<Pin, PC11> or std::is_same_v<Pin, PD9>) return PinMode::USART3_RX;
      else return PinMode::Input;
   }
}

template <class R>
template <Periph usart, Periph rcc>
size_t USART_<R>::clock()
{
   if      constexpr (usart == Periph::USART1) return make_reference<rcc>().get_APB2_clock();
   else if constexpr (usart == Periph::USART2) return make_reference<rcc>().get_APB1_clock();
   else if constexpr (usart == Periph::USART3) return make_reference<rcc>().get_APB1_clock();
}

template <class R>
template<Periph usart, class Pin> constexpr Periph USART_<R>::stream()
{
   if constexpr (usart == Periph::USART1) {
      if      constexpr (std::is_same_v<Pin, PA9>  or std::is_same_v<Pin, PB6>) return Periph::DMA1_stream4; 
      else if constexpr (std::is_same_v<Pin, PA10> or std::is_same_v<Pin, PB7>) return Periph::DMA1_stream5;
   }
   else if constexpr (usart == Periph::USART2) {
      if      constexpr (std::is_same_v<Pin, PA2> or std::is_same_v<Pin, PD5>) return Periph::DMA1_stream7; 
      else if constexpr (std::is_same_v<Pin, PA3> or std::is_same_v<Pin, PD6>) return Periph::DMA1_stream6;
   }
   else if constexpr (usart == Periph::USART3) {
      if      constexpr (std::is_same_v<Pin, PB10> or std::is_same_v<Pin, PC10> or std::is_same_v<Pin, PD8>) return Periph::DMA1_stream2; 
      else if constexpr (std::is_same_v<Pin, PB11> or std::is_same_v<Pin, PC11> or std::is_same_v<Pin, PD9>) return Periph::DMA1_stream3;
   }
   return Periph::DMA1_stream1;
}

template <class R>
constexpr IRQn_Type USART_<R>::IRQn(Periph usart)
{
   return usart == Periph::USART1 ? USART1_IRQn :
          usart == Periph::USART2 ? USART2_IRQn :
          usart == Periph::USART3 ? USART3_IRQn :
          NonMaskableInt_IRQn;
}

template <class R>
template<Periph usart, class TXpin, class RXpin> void USART_<R>::pin_static_assert()
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
}

} //namespace mcu
