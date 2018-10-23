#pragma once

#include <type_traits>
#include "f4_bits_usart.h"
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
};

template<class R = USART_default>
class USART_ : private R {
public:

   using CMSIS_type     = USART_TypeDef;
   using Parity         = USART_bits::CR1::Parity;
   using WakeMethod     = USART_bits::CR1::WakeMethod;
   using DataBits       = USART_bits::CR1::DataBits;
   using OverSample     = USART_bits::CR1::OverSample;
   using BreakDetection = USART_bits::CR2::BreakDetection;
   using StopBits       = USART_bits::CR2::StopBits;

   USART_& set (Parity         v)         {this->CR1.PS    = v; return *this;}
   USART_& set (WakeMethod     v)         {this->CR1.WAKE  = v; return *this;}
   USART_& set (DataBits       v)         {this->CR1.M     = v; return *this;}
   USART_& set (OverSample     v)         {this->CR1.OVER8 = v; return *this;}
   USART_& set (BreakDetection v)         {this->CR2.LBDL  = v; return *this;}
   USART_& set (StopBits       v)         {this->CR2.STOP  = v; return *this;}
   USART_& send_byte (uint8_t  v)         {this->DR = v;        return *this;}
   USART_& set (Baudrate v, size_t clock) {this->BRR = clock/v; return *this;}

   
   template <Periph p, class RCC = RCC>
   USART_& clock_enable  (){RCC::make_reference().template clock_enable<p>(); return *this;}
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

   USART_& enable_IDLE_interrupt         (){this->CR1.IDLEIE = true;  return *this;}
   bool   is_IDLE_interrupt              (){return this->SR.IDLE;}
   USART_& enable_tx_complete_interrupt  (){this->CR1.TCIE   = true;  return *this;}
   USART_& disable_tx_complete_interrupt (){this->CR1.TCIE   = false; return *this;}
   bool   is_tx_complete                 (){return this->SR.TC;}
   bool   is_tx_complete_interrupt_enable(){return this->CR1.TCIE;}
   USART_& clear_interrupt_flags         (){this->SR.read(); uint32_t t = this->DR; return *this;}

   size_t receive_data_adr () {return reinterpret_cast<size_t>(&this->DR);}
   size_t transmit_data_adr() {return reinterpret_cast<size_t>(&this->DR);}

   static constexpr Channel DMA_channel(Periph);
   static constexpr IRQn_Type IRQn(Periph);
   static constexpr PinMode pin_mode(Periph);
   
   template <Periph p, class RCC = RCC> static size_t clock ();
   template<class RXpin,  Periph> static constexpr bool is_rx_support();
   template<class TXpin,  Periph> static constexpr bool is_tx_support();
   template<class RTSpin, Periph> static constexpr bool is_rts_support();
};




using USART = USART_<>;



template <Periph p>
auto& make_reference()
{
   if      constexpr (p == Periph::USART1) return *reinterpret_cast<USART*>(USART1_BASE); 
   else if constexpr (p == Periph::USART2) return *reinterpret_cast<USART*>(USART2_BASE);
   else if constexpr (p == Periph::USART3) return *reinterpret_cast<USART*>(USART3_BASE);
   else if constexpr (p == Periph::USART4) return *reinterpret_cast<USART*>(UART4_BASE );
   else if constexpr (p == Periph::USART5) return *reinterpret_cast<USART*>(UART5_BASE );
   else if constexpr (p == Periph::USART6) return *reinterpret_cast<USART*>(USART6_BASE);
}




























template <class R>
template <Periph p, class RCC>
size_t USART_<R>::clock()
{
   if      constexpr (p == Periph::USART1) return RCC::make_reference().get_APB2_clock();
   else if constexpr (p == Periph::USART2) return RCC::make_reference().get_APB1_clock();
   else if constexpr (p == Periph::USART3) return RCC::make_reference().get_APB1_clock();
   else if constexpr (p == Periph::USART4) return RCC::make_reference().get_APB1_clock();
   else if constexpr (p == Periph::USART5) return RCC::make_reference().get_APB1_clock();
   else if constexpr (p == Periph::USART6) return RCC::make_reference().get_APB2_clock(); 
}

template <class R>
template<class RXpin, Periph p>
constexpr bool USART_<R>::is_rx_support()
{
   return p == Periph::USART1 ? std::is_same_v<RXpin, PA10>
                             or std::is_same_v<RXpin, PB7 > :
          p == Periph::USART2 ? std::is_same_v<RXpin, PA3 >
                             or std::is_same_v<RXpin, PD6 > :
          p == Periph::USART3 ? std::is_same_v<RXpin, PB11>
                             or std::is_same_v<RXpin, PD9 >
                             or std::is_same_v<RXpin, PC11> :
          p == Periph::USART4 ? std::is_same_v<RXpin, PA1 >
                             or std::is_same_v<RXpin, PC11> :
          p == Periph::USART5 ? std::is_same_v<RXpin, PD2 > :
          p == Periph::USART6 ? std::is_same_v<RXpin, PC7 >
                             or std::is_same_v<RXpin, PG9 > :
          false;
}

template <class R>
template<class TXpin, Periph p>
constexpr bool USART_<R>::is_tx_support()
{
   return p == Periph::USART1 ? std::is_same_v<TXpin, PA9 >
                             or std::is_same_v<TXpin, PB6 > :
          p == Periph::USART2 ? std::is_same_v<TXpin, PA2 >
                             or std::is_same_v<TXpin, PD5 > :
          p == Periph::USART3 ? std::is_same_v<TXpin, PB10>
                             or std::is_same_v<TXpin, PD8 >
                             or std::is_same_v<TXpin, PC10> :
          p == Periph::USART4 ? std::is_same_v<TXpin, PA0 >
                             or std::is_same_v<TXpin, PC10> :
          p == Periph::USART5 ? std::is_same_v<TXpin, PC12> :
          p == Periph::USART6 ? std::is_same_v<TXpin, PC6 >
                             or std::is_same_v<TXpin, PG14> :
          false;
}

template <class R>
template<class RTSpin, Periph p>
constexpr bool USART_<R>::is_rts_support()
{
   return p == Periph::USART1 ? std::is_same_v<RTSpin, PA12> :
          p == Periph::USART2 ? std::is_same_v<RTSpin, PD4 > :
          p == Periph::USART3 ? std::is_same_v<RTSpin, PB14>
                             or std::is_same_v<RTSpin, PD12> :
          p == Periph::USART6 ? std::is_same_v<RTSpin, PG8 >
                             or std::is_same_v<RTSpin, PG12> :
          false;
}

template <class R>
constexpr Channel USART_<R>::DMA_channel(Periph p)
{
   return  p == Periph::USART6 ? Channel::_5 : Channel::_4;
}

template <class R>
constexpr IRQn_Type USART_<R>::IRQn(Periph p)
{
   return p == Periph::USART1 ? USART1_IRQn :
          p == Periph::USART2 ? USART2_IRQn :
          p == Periph::USART3 ? USART3_IRQn :
          p == Periph::USART4 ? UART4_IRQn  :
          p == Periph::USART5 ? UART5_IRQn  :
          p == Periph::USART6 ? USART6_IRQn :
          NonMaskableInt_IRQn;
}

template <class R>
constexpr PinMode USART_<R>::pin_mode(Periph p)
{
   return p == Periph::USART1 ? PinMode::Alternate_7 :
          p == Periph::USART2 ? PinMode::Alternate_7 :
          p == Periph::USART3 ? PinMode::Alternate_7 :
          p == Periph::USART4 ? PinMode::Alternate_8 :
          p == Periph::USART5 ? PinMode::Alternate_8 :
          p == Periph::USART6 ? PinMode::Alternate_8 :
          PinMode::Input;
}
} //namespace mcu