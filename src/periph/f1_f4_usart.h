#pragma once

#include <type_traits>
#if defined(STM32F1)
#include "f1_bits_usart.h"
#elif defined(STM32F4)
#include "f4_bits_usart.h"
#endif
#include "periph_rcc.h"
#include "pin.h"
#include "meta.h"

namespace mcu {

class USART {
   volatile USART_bits::SR  SR;   // USART Status register,                   offset: 0x00
   volatile uint32_t        DR;   // USART Data register,                     offset: 0x04
   volatile uint32_t        BRR;  // USART Baud rate register,                offset: 0x08
   volatile USART_bits::CR1 CR1;  // USART Control register 1,                offset: 0x0C
   volatile USART_bits::CR2 CR2;  // USART Control register 2,                offset: 0x10
   volatile USART_bits::CR3 CR3;  // USART Control register 3,                offset: 0x14
   volatile uint32_t        GTPR; // USART Guard time and prescaler register, offset: 0x18

public:
   using CMSIS_type     = USART_TypeDef;
   using Parity         = USART_bits::CR1::Parity;
   using WakeMethod     = USART_bits::CR1::WakeMethod;
   using DataBits       = USART_bits::CR1::DataBits;
   using BreakDetection = USART_bits::CR2::BreakDetection;
   using StopBits       = USART_bits::CR2::StopBits;

   enum Baudrate {
      BR9600   = 0b000, // modbus time 4 ms
      BR14400  = 0b001, // modbus time 3 ms
      BR19200  = 0b010, // modbus time 2 ms
      BR28800  = 0b011, // modbus time 2 ms
      BR38400  = 0b100, // modbus time 2 ms
      BR57600  = 0b101, // modbus time 2 ms
      BR76800  = 0b110, // modbus time 2 ms
      BR115200 = 0b111  // modbus time 2 ms
   };

   auto& like_CMSIS() { return *reinterpret_cast<CMSIS_type*>(this); }

   USART& set (Parity         v)         {CR1.PS    = v; return *this;}
   USART& set (DataBits       v)         {CR1.M     = v; return *this;}
   USART& set (StopBits       v)         {CR2.STOP  = v; return *this;}
   USART& set (WakeMethod     v)         {CR1.WAKE  = v; return *this;}
   USART& set (BreakDetection v)         {CR2.LBDL  = v; return *this;}
   // USART& send_byte (uint8_t  v)         {DR = v;        return *this;} // no test
   USART& set (Baudrate, Periph);

   USART& enable        (){CR1.UE   = true;  return *this;}
   USART& disable       (){CR1.UE   = false; return *this;}
   USART& rx_enable     (){CR1.RE   = true;  return *this;}
   USART& rx_disable    (){CR1.RE   = false; return *this;}
   USART& tx_enable     (){CR1.TE   = true;  return *this;}
   USART& tx_disable    (){CR1.TE   = false; return *this;}
   USART& rts_enable    (){CR3.RTSE = true;  return *this;}
   USART& rts_disable   (){CR3.RTSE = false; return *this;}
   USART& DMA_tx_enable (){CR3.DMAT = true;  return *this;}
   USART& DMA_rx_enable (){CR3.DMAR = true;  return *this;}
   USART& parity_enable (){CR1.PCE  = true;  return *this;}
   USART& parity_disable(){CR1.PCE  = false; return *this;}
   // bool   is_ready_read (){return SR.RXNE;} // no test

   USART& enable_IDLE_interrupt          (){CR1.IDLEIE = true;  return *this;}
   bool   is_IDLE_interrupt              (){return SR.IDLE;}
   USART& enable_tx_complete_interrupt   (){CR1.TCIE   = true;  return *this;}
   USART& disable_tx_complete_interrupt  (){CR1.TCIE   = false; return *this;}
   bool   is_tx_complete                 (){return SR.TC;}
   bool   is_tx_complete_interrupt_enable(){return CR1.TCIE;}

   
   VIRTUAL_TEST uint32_t read_SR() { return *reinterpret_cast<volatile uint32_t*>(&SR); }
   VIRTUAL_TEST uint32_t read_DR() { return DR; }
   USART& clear_interrupt_flags()
   {
      [[maybe_unused]] auto 
      _ = read_SR();
      IF_TEST_WAIT_MS(100);
      _ = read_DR();
      return *this;
   }

   size_t receive_data_adr () {return reinterpret_cast<size_t>(&DR);}
   size_t transmit_data_adr() {return reinterpret_cast<size_t>(&DR);}

   
   static constexpr IRQn_Type IRQn(Periph);
   template<class Pin> static constexpr Periph default_stream();
   template<class Pin> static constexpr PinMode pin_mode();
   template<Periph usart, class TXpin, class RXpin> static void pin_static_assert();
};



#if not defined(USE_MOCK_USART)
SFINAE(USART1,USART) make_reference() {return *reinterpret_cast<USART*>(USART1_BASE);}
SFINAE(USART2,USART) make_reference() {return *reinterpret_cast<USART*>(USART2_BASE);}
SFINAE(USART3,USART) make_reference() {return *reinterpret_cast<USART*>(USART3_BASE);}
#if defined(STM32F4)
SFINAE(USART4,USART) make_reference() {return *reinterpret_cast<USART*>(UART4_BASE );}
SFINAE(USART5,USART) make_reference() {return *reinterpret_cast<USART*>(UART5_BASE );}
SFINAE(USART6,USART) make_reference() {return *reinterpret_cast<USART*>(USART6_BASE);}
#endif
#endif

















USART& USART::set (Baudrate baudrate, Periph p)
{
   auto clock = REF(RCC).clock(p);
   switch (baudrate) {
      case BR9600:   BRR = clock/9600  ; break;
      case BR14400:  BRR = clock/14400 ; break;
      case BR19200:  BRR = clock/19200 ; break;
      case BR28800:  BRR = clock/28800 ; break;
      case BR38400:  BRR = clock/38400 ; break;
      case BR57600:  BRR = clock/57600 ; break;
      case BR76800:  BRR = clock/76800 ; break;
      case BR115200: BRR = clock/115200; break;
   }
   return *this;
}


template <class Pin> constexpr PinMode USART::pin_mode()
{
   static_assert (
      WRAP(meta::position_v<Pin
         ,PA9,PA10,PB6,PB7,PA2,PA3,PD5,PD6,PB10,PB11,PC10,PC11,PD8,PD9
   #if defined(STM32F4)
         ,PA0,PA1,PC10,PC11,PC12,PD2,PC6,PC7,PG14,PG9
   #endif
      >) != -1,
      "неверный аргумент шаблона class Pin"
   );
   if      constexpr (std::is_same_v<Pin, PA9>  or std::is_same_v<Pin, PB6>) return PinMode::USART1_TX; 
   else if constexpr (std::is_same_v<Pin, PA10> or std::is_same_v<Pin, PB7>) return PinMode::USART1_RX;
   else if constexpr (std::is_same_v<Pin, PA2>  or std::is_same_v<Pin, PD5>) return PinMode::USART2_TX; 
   else if constexpr (std::is_same_v<Pin, PA3>  or std::is_same_v<Pin, PD6>) return PinMode::USART2_RX;
   else if constexpr (std::is_same_v<Pin, PB10> or std::is_same_v<Pin, PC10> or std::is_same_v<Pin, PD8>) return PinMode::USART3_TX; 
   else if constexpr (std::is_same_v<Pin, PB11> or std::is_same_v<Pin, PC11> or std::is_same_v<Pin, PD9>) return PinMode::USART3_RX;
#if defined(STM32F4)
   else if constexpr (std::is_same_v<Pin, PA0>  or std::is_same_v<Pin, PC10>) return PinMode::USART4_TX; // PC10 will not work
   else if constexpr (std::is_same_v<Pin, PA1>  or std::is_same_v<Pin, PC11>) return PinMode::USART4_RX; // PC11 will not work
   else if constexpr (std::is_same_v<Pin, PC12>) return PinMode::USART5_TX;
   else if constexpr (std::is_same_v<Pin, PD2> ) return PinMode::USART5_RX;
   else if constexpr (std::is_same_v<Pin, PC6>  or std::is_same_v<Pin, PG14>) return PinMode::USART6_TX; 
   else if constexpr (std::is_same_v<Pin, PC7>  or std::is_same_v<Pin, PG9 >) return PinMode::USART6_RX;
#endif

}


template<class Pin> constexpr Periph USART::default_stream()
{
   static_assert (
      WRAP(meta::position_v<Pin
         ,PA9,PA10,PB6,PB7,PA2,PA3,PD5,PD6,PB10,PB11,PC10,PC11,PD8,PD9
   #if defined(STM32F4)
         ,PA0,PA1,PC10,PC11,PC12,PD2,PC6,PC7,PG14,PG9
   #endif
      >) != -1,
      "неверный аргумент шаблона class Pin"
   );
#if defined(STM32F1)
   if      constexpr (std::is_same_v<Pin, PA9>  or std::is_same_v<Pin, PB6>) return Periph::DMA1_stream4; 
   else if constexpr (std::is_same_v<Pin, PA10> or std::is_same_v<Pin, PB7>) return Periph::DMA1_stream5;
   else if constexpr (std::is_same_v<Pin, PA2>  or std::is_same_v<Pin, PD5>) return Periph::DMA1_stream7; 
   else if constexpr (std::is_same_v<Pin, PA3>  or std::is_same_v<Pin, PD6>) return Periph::DMA1_stream6;
   else if constexpr (std::is_same_v<Pin, PB10> or std::is_same_v<Pin, PC10> or std::is_same_v<Pin, PD8>) return Periph::DMA1_stream2; 
   else if constexpr (std::is_same_v<Pin, PB11> or std::is_same_v<Pin, PC11> or std::is_same_v<Pin, PD9>) return Periph::DMA1_stream3;
#elif defined(STM32F4)
   if      constexpr (std::is_same_v<Pin, PA9>  or std::is_same_v<Pin, PB6>) return Periph::DMA2_stream7; 
   else if constexpr (std::is_same_v<Pin, PA10> or std::is_same_v<Pin, PB7>) return Periph::DMA2_stream2;
   else if constexpr (std::is_same_v<Pin, PA2>  or std::is_same_v<Pin, PD5>) return Periph::DMA1_stream6; 
   else if constexpr (std::is_same_v<Pin, PA3>  or std::is_same_v<Pin, PD6>) return Periph::DMA1_stream5;
   else if constexpr (std::is_same_v<Pin, PB10> or std::is_same_v<Pin, PC10> or std::is_same_v<Pin, PD8>) return Periph::DMA1_stream3; 
   else if constexpr (std::is_same_v<Pin, PB11> or std::is_same_v<Pin, PC11> or std::is_same_v<Pin, PD9>) return Periph::DMA1_stream1;
   else if constexpr (std::is_same_v<Pin, PA0>  or std::is_same_v<Pin, PC10>) return Periph::DMA1_stream4;
   else if constexpr (std::is_same_v<Pin, PA1>  or std::is_same_v<Pin, PC11>) return Periph::DMA1_stream2;
   else if constexpr (std::is_same_v<Pin, PC12>) return Periph::DMA1_stream7;
   else if constexpr (std::is_same_v<Pin, PD2> ) return Periph::DMA1_stream0;
   else if constexpr (std::is_same_v<Pin, PC6>  or std::is_same_v<Pin, PG14>) return Periph::DMA2_stream6; 
   else if constexpr (std::is_same_v<Pin, PC7>  or std::is_same_v<Pin, PG9 >) return Periph::DMA2_stream1;
#endif
}


constexpr IRQn_Type USART::IRQn (Periph usart)
{
   return usart == Periph::USART1 ? USART1_IRQn :
          usart == Periph::USART2 ? USART2_IRQn :
          usart == Periph::USART3 ? USART3_IRQn :
      #if defined (STM32F4)
          usart == Periph::USART4 ? UART4_IRQn :
          usart == Periph::USART5 ? UART5_IRQn :
          usart == Periph::USART6 ? USART6_IRQn :
      #endif
          NonMaskableInt_IRQn;
}


template<Periph usart, class TXpin, class RXpin> void USART::pin_static_assert()
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
#if defined (STM32F4)
   if constexpr (usart == Periph::USART4) {
      static_assert (
         (std::is_same_v<TXpin, PA0> and std::is_same_v<RXpin, PA1>) or
         (std::is_same_v<TXpin, PC10> and std::is_same_v<RXpin, PC11>),
         "USART4 возможен только с парами пинов TX/PA0, RX/PA1 или TX/PC10, RX/PC11");
   } else if constexpr (usart == Periph::USART5) {
      static_assert (
         (std::is_same_v<TXpin, PC12> and std::is_same_v<RXpin, PD2>),
         "USART5 возможен только с парами пинов TX/PC12, RX/PD2");
   } else if constexpr (usart == Periph::USART6) {
      static_assert (
         (std::is_same_v<TXpin, PC6> and std::is_same_v<RXpin, PC7>) or
         (std::is_same_v<TXpin, PG14> and std::is_same_v<RXpin, PG9>),
         "USART6 возможен только с парами пинов TX/PC6, RX/PC7 или TX/PG14, RX/PG9");
   }
#endif
}

} //namespace mcu
