# pragma once
#include "mock_usart.h"
#include "mock_DMA_stream.h"
#include "string_result.h"

struct Interrupting
{
   Interrupting* next {nullptr};
   virtual void interrupt() = 0;
};

class Interrupt 
{
   Interrupting* first{nullptr};

public:
   
   void clear_subscribe(){first = nullptr;}
   
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

   void unsubscribe(Interrupting* ps)
   {
      auto p = first;
      while (p->next) {
         if (p == ps)
            first = ps->next;
         else if (p->next = ps) {
            p->next = p->next->next;
         }
         p++;
      }
   }

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












void USART1_IRQHandler          () { result << "Прерывание uart" << '\n'; interrupt_usart1.interrupt(); mcu::make_reference<mcu::Periph::USART1>().clear_interrupt_flags();}
void USART2_IRQHandler          () { result << "Прерывание uart" << '\n'; interrupt_usart2.interrupt(); mcu::make_reference<mcu::Periph::USART2>().clear_interrupt_flags();}
void USART3_IRQHandler          () { result << "Прерывание uart" << '\n'; interrupt_usart3.interrupt(); mcu::make_reference<mcu::Periph::USART3>().clear_interrupt_flags();}

void DMA1_Channel1_IRQHandler   () { result << "Прерывание dma"  << '\n'; interrupt_DMA_channel1.interrupt(); mcu::make_reference<mcu::Periph::TEST_DMA_stream1>().clear_interrupt_flags();}
void DMA1_Channel2_IRQHandler   () { result << "Прерывание dma"  << '\n'; interrupt_DMA_channel2.interrupt(); mcu::make_reference<mcu::Periph::TEST_DMA_stream1>().clear_interrupt_flags();}
void DMA1_Channel3_IRQHandler   () { result << "Прерывание dma"  << '\n'; interrupt_DMA_channel3.interrupt(); mcu::make_reference<mcu::Periph::TEST_DMA_stream1>().clear_interrupt_flags();}
void DMA1_Channel4_IRQHandler   () { result << "Прерывание dma"  << '\n'; interrupt_DMA_channel4.interrupt(); mcu::make_reference<mcu::Periph::TEST_DMA_stream1>().clear_interrupt_flags();}
void DMA1_Channel5_IRQHandler   () { result << "Прерывание dma"  << '\n'; interrupt_DMA_channel5.interrupt(); mcu::make_reference<mcu::Periph::TEST_DMA_stream1>().clear_interrupt_flags();}
void DMA1_Channel6_IRQHandler   () { result << "Прерывание dma"  << '\n'; interrupt_DMA_channel6.interrupt(); mcu::make_reference<mcu::Periph::TEST_DMA_stream1>().clear_interrupt_flags();}
void DMA1_Channel7_IRQHandler   () { result << "Прерывание dma"  << '\n'; interrupt_DMA_channel7.interrupt(); mcu::make_reference<mcu::Periph::TEST_DMA_stream1>().clear_interrupt_flags();}