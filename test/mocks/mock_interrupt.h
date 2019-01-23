#pragma once

#include "periph.h"
#include "process.h"

#define USE_MOCK_NVIC

namespace mock {

struct NVIC_Enable
{
   Process& process { Process::make() };
   void operator() (IRQn_Type IRQn) {
      switch (IRQn) {
         case USART1_IRQn       : process << "NVIC: включение прерывания USART1\n"       ; break;
         case DMA1_Channel1_IRQn: process << "NVIC: включение прерывания DMA1_Channel1\n"; break;
         case DMA1_Channel2_IRQn: process << "NVIC: включение прерывания DMA1_Channel2\n"; break;
         case DMA1_Channel3_IRQn: process << "NVIC: включение прерывания DMA1_Channel3\n"; break;
         case DMA1_Channel4_IRQn: process << "NVIC: включение прерывания DMA1_Channel4\n"; break;
         case DMA1_Channel5_IRQn: process << "NVIC: включение прерывания DMA1_Channel5\n"; break;
         case DMA1_Channel6_IRQn: process << "NVIC: включение прерывания DMA1_Channel6\n"; break;
         case DMA1_Channel7_IRQn: process << "NVIC: включение прерывания DMA1_Channel7\n"; break;
         default: process << "допиши NVIC_Enable\n";
      }
   }

} NVIC_EnableIRQ;

}

