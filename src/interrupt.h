#pragma once

#include "periph.h"

/// интерфейс для подписки на прерывания
struct Interrupting
{
   Interrupting* next {nullptr};
   virtual void interrupt() = 0;
};

class Interrupt 
{
   Interrupting* first{nullptr};

public:
   void subscribe(Interrupting* ps)
   {
      auto p = first;
      if (p) {
         while (p->next)
            p = p->next;
         p->next = ps;
      } else {  
         first = ps;
      } 
   }

   void clear_subscribe() { first = nullptr; }

   void interrupt()
   {
      auto p = first;
      while (p) {
         p->interrupt();
         p = p->next;
      }
   }
};

Interrupt interrupt_usart1;
Interrupt interrupt_usart2;
Interrupt interrupt_usart3;

Interrupt interrupt_DMA_channel1;
Interrupt interrupt_DMA_channel2;
Interrupt interrupt_DMA_channel3;
Interrupt interrupt_DMA_channel4;
Interrupt interrupt_DMA_channel5;
Interrupt interrupt_DMA_channel6;
Interrupt interrupt_DMA_channel7;

template<mcu::Periph v>
auto& get_interrupt()
{
    if      constexpr (v == mcu::Periph::USART1)       return interrupt_usart1;
    else if constexpr (v == mcu::Periph::USART2)       return interrupt_usart2;
    else if constexpr (v == mcu::Periph::USART3)       return interrupt_usart3;
    else if constexpr (v == mcu::Periph::DMA1_stream1) return interrupt_DMA_channel1;
    else if constexpr (v == mcu::Periph::DMA1_stream2) return interrupt_DMA_channel2;
    else if constexpr (v == mcu::Periph::DMA1_stream3) return interrupt_DMA_channel3;
    else if constexpr (v == mcu::Periph::DMA1_stream4) return interrupt_DMA_channel4;
    else if constexpr (v == mcu::Periph::DMA1_stream5) return interrupt_DMA_channel5;
    else if constexpr (v == mcu::Periph::DMA1_stream6) return interrupt_DMA_channel6;
    else if constexpr (v == mcu::Periph::DMA1_stream7) return interrupt_DMA_channel7;
} 


#include "interrupts.h"