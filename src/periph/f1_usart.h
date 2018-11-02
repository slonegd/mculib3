#pragma once

#include <type_traits>
#include "f1_bits_usart.h"
#include "rcc.h"
#include "pin.h"
#include "dma.h"

namespace mcu {

enum Baudrate {
      BR9600   = 9600,
      BR14400  = 14400,
      BR19200  = 19200,
      BR28800  = 28800,
      BR38400  = 38400,
      BR57600  = 57600,
      BR76800  = 76800,
      BR115200 = 115200
   };

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
   using Channel        = DMA_bits::Channel;

   USART_& set (Parity         v)         {this->CR1.PS    = v; return *this;}
   USART_& set (WakeMethod     v)         {this->CR1.WAKE  = v; return *this;}
   USART_& set (DataBits       v)         {this->CR1.M     = v; return *this;}
   USART_& set (BreakDetection v)         {this->CR2.LBDL  = v; return *this;}
   USART_& set (StopBits       v)         {this->CR2.STOP  = v; return *this;}
   USART_& send_byte (uint8_t  v)         {this->DR = v;        return *this;}
   USART_& set (Baudrate v, size_t clock) {this->BRR = clock/v; return *this;}

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

   template<Periph, class Pin_> static constexpr Periph stream();
   template<Periph> static constexpr IRQn_Type IRQn();
   
   template <Periph usart, class Pin_> static constexpr PinMode pin_mode();
   template <Periph usart, Periph rcc = Periph::RCC> static size_t clock();
};

using USART = USART_<>;



template <Periph p> std::enable_if_t<p == Periph::USART1, USART&> make_reference() {return *reinterpret_cast<USART*>(USART1_BASE);}
template <Periph p> std::enable_if_t<p == Periph::USART2, USART&> make_reference() {return *reinterpret_cast<USART*>(USART2_BASE);}
template <Periph p> std::enable_if_t<p == Periph::USART3, USART&> make_reference() {return *reinterpret_cast<USART*>(USART3_BASE);}






















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
template<Periph usart, class Pin_> constexpr Periph USART_<R>::stream()
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
template<Periph usart> constexpr IRQn_Type USART_<R>::IRQn()
{
   return usart == Periph::USART1 ? USART1_IRQn :
          usart == Periph::USART2 ? USART2_IRQn :
          usart == Periph::USART3 ? USART3_IRQn :
          NonMaskableInt_IRQn;
}

} //namespace mcu
