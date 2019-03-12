#pragma once

#include <type_traits>
#include "bits_usart_f0.h"
#include "periph_rcc.h"
#include "pin.h"
#include "meta.h"

namespace mcu {

class USART {
   volatile USART_bits::CR1 CR1;  // Control register 1,                offset: 0x00 
   volatile USART_bits::CR2 CR2;  // Control register 2,                offset: 0x04 
   volatile USART_bits::CR3 CR3;  // Control register 3,                offset: 0x08
   volatile uint32_t        BRR;  // Baud rate register,                offset: 0x0C
   volatile uint32_t        GTPR; // Guard time and prescaler register, offset: 0x10
   volatile uint32_t        RTOR; // Receiver Time Out register,        offset: 0x14  
   volatile uint32_t        RQR;  // Request register,                  offset: 0x18
   volatile USART_bits::SR  ISR;  // Interrupt and status register,     offset: 0x1C
   volatile USART_bits::SR  ICR;  // Interrupt flag Clear register,     offset: 0x20
   volatile uint32_t        RDR;  // Receive Data register,             offset: 0x24
   volatile uint32_t        TDR;  // Transmit Data register,            offset: 0x28

public:
   using CMSIS_type     = USART_TypeDef;
   using Parity         = USART_bits::CR1::Parity;
   using DataBits       = USART_bits::CR1::DataBits;
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
   USART& set (DataBits       v)         {CR1.M0    = v; return *this;}
   USART& set (StopBits       v)         {CR2.STOP  = v; return *this;}
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

   USART& enable_IDLE_interrupt          (){CR1.IDLEIE = true;  return *this;}
   bool   is_IDLE_interrupt              (){return ISR.IDLE;}
   USART& enable_tx_complete_interrupt   (){CR1.TCIE   = true;  return *this;}
   USART& disable_tx_complete_interrupt  (){CR1.TCIE   = false; return *this;}
   bool   is_tx_complete                 (){return ISR.TC;}
   bool   is_tx_complete_interrupt_enable(){return CR1.TCIE;}

   
   USART& clear_interrupt_flags()
   {
      *reinterpret_cast<volatile uint32_t*>(&ICR) = 0xFFFFFFFF;
      return *this;
   }

   size_t receive_data_adr () {return reinterpret_cast<size_t>(&RDR);}
   size_t transmit_data_adr() {return reinterpret_cast<size_t>(&TDR);}

   
   template<class Pin> static constexpr Periph default_stream();
   template<class Pin> static constexpr PinMode pin_mode();
   template<Periph usart, class TXpin, class RXpin> static void pin_static_assert();
};



#if not defined(USE_MOCK_USART)
SFINAE(USART1,USART) make_reference() {return *reinterpret_cast<USART*>(USART1_BASE);}
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
   if      constexpr (std::is_same_v<Pin, PA9> or std::is_same_v<Pin, PA10>) return PinMode::Alternate_1;
   else if constexpr (std::is_same_v<Pin, PB6> or std::is_same_v<Pin, PB7>)  return PinMode::Alternate_0;
   else if constexpr (std::is_same_v<Pin, PA2> or std::is_same_v<Pin, PA3>)  return PinMode::Alternate_1;
   else static_assert (always_false_v<Pin>, "неверный аргумент шаблона class Pin");
}


template<class Pin> constexpr Periph USART::default_stream()
{
   if      constexpr (std::is_same_v<Pin, PA9>  or std::is_same_v<Pin, PB6> or std::is_same_v<Pin, PA2>) return Periph::DMA1_stream2; 
   else if constexpr (std::is_same_v<Pin, PA10> or std::is_same_v<Pin, PB7> or std::is_same_v<Pin, PA3>) return Periph::DMA1_stream3;
   else static_assert (always_false_v<Pin>, "неверный аргумент шаблона class Pin");
}


template<Periph usart, class TXpin, class RXpin> void USART::pin_static_assert()
{
   if constexpr (usart == Periph::USART1) {
      static_assert (
            (std::is_same_v<TXpin, PA9> and std::is_same_v<RXpin, PA10>)
         or (std::is_same_v<TXpin, PB6> and std::is_same_v<RXpin, PB7 >)
         or (std::is_same_v<TXpin, PA2> and std::is_same_v<RXpin, PA3 >)
         , "USART1 возможен только с парами пинов TX/RX: PA2/PA3, PA9/PA10 или PB6/PB7");
   } 
}

} //namespace mcu
