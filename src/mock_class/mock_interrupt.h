# pragma once
#include "string_result.h"

struct Interrupting
{
   Interrupting* next {nullptr};
   virtual void interrupt() = 0;
};

class Interrupt 
{
   Interrupting* first{nullptr};
   void clearInterruptFlags(){};

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
   void interrupt()
   {
      auto p = first;
      while (p) {
         p->interrupt();
         p = p->next;
      }
      clearInterruptFlags();
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












void USART1_IRQHandler          () { interrupt_usart1.interrupt(); }
void USART2_IRQHandler          () { interrupt_usart2.interrupt(); }
void USART3_IRQHandler          () { interrupt_usart3.interrupt(); }

void DMA1_Channel1_IRQHandler   () { interrupt_DMA_channel1.interrupt(); }
void DMA1_Channel2_IRQHandler   () { interrupt_DMA_channel2.interrupt(); }
void DMA1_Channel3_IRQHandler   () { interrupt_DMA_channel3.interrupt(); }
void DMA1_Channel4_IRQHandler   () { interrupt_DMA_channel4.interrupt(); }
void DMA1_Channel5_IRQHandler   () { interrupt_DMA_channel5.interrupt(); }
void DMA1_Channel6_IRQHandler   () { interrupt_DMA_channel6.interrupt(); }
void DMA1_Channel7_IRQHandler   () { interrupt_DMA_channel7.interrupt(); }