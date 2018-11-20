#pragma once

#include "string_result.h"

enum IRQn_Type
{
   USART1_IRQn, DMA1_Channel1_IRQn, DMA1_Channel2_IRQn, Error
};


struct NVIC_Enable
{
   void operator() (IRQn_Type IRQn) {
      if (IRQn == USART1_IRQn) {
         result << "Включили прерывание переферии USART" << '\n';
      } else if (IRQn == DMA1_Channel1_IRQn) {
         result << "Включили прерывание переферии DMA TX_stream" << '\n';
      } else if (IRQn == DMA1_Channel2_IRQn) {
         result << "Включили прерывание переферии DMA RX_stream" << '\n';
      }
   }

}NVIC_EnableIRQ;

