#pragma once

#include "f0_f1_bits_dma_stream.h"
#include "periph_dma.h"

namespace mcu {

class DMA_stream {
   __IO DMA_bits::CCR CCR;   // DMA channel x configuration register      offset: 0x08 + 0d20 × (channel number – 1)
   __IO uint32_t      CNDTR; // DMA channel x number of data register     offset: 0x0C + 0d20 × (channel number – 1)
   __IO uint32_t      CPAR;  // DMA channel x peripheral address register offset: 0x10 + 0d20 × (channel number – 1)
   __IO uint32_t      CMAR;  // DMA channel x memory address register     offset: 0x14 + 0d20 × (channel number – 1)

public:
   using CMSIS_type    = DMA_Channel_TypeDef;
   using DataSize      = DMA_bits::CCR::DataSize;
   using Priority      = DMA_bits::CCR::Priority;
   using DataDirection = DMA_bits::CCR::DataDirection;
   using Channel       = DMA::Channel;

   auto& like_CMSIS() { return *reinterpret_cast<CMSIS_type*>(this); }

   DMA_stream& enable       (){CCR.EN = true; while(not CCR.EN){} return *this;}
   DMA_stream& disable      (){CCR.EN = false;  return *this;}
   bool        is_disable   (){return (not CCR.EN);}
   DMA_stream& inc_memory   (){CCR.MINC = true; return *this;}
   DMA_stream& inc_periph   (){CCR.PINC = true; return *this;}
   DMA_stream& circular_mode(){CCR.CIRC = true; return *this;}

   DMA_stream& set_memory_adr      (uint32_t v){CMAR  = v; return *this;}
   DMA_stream& set_periph_adr      (uint32_t v){CPAR  = v; return *this;}
   DMA_stream& set_qty_transactions(uint16_t v){CNDTR = v; return *this;}

   DMA_stream& direction  (DataDirection d){CCR.DIR = d;   return *this;}
   DMA_stream& size_memory(DataSize d)     {CCR.MSIZE = d; return *this;}
   DMA_stream& size_periph(DataSize d)     {CCR.PSIZE = d; return *this;}

   static constexpr IRQn_Type IRQn (Periph);

   DMA_stream& enable_transfer_complete_interrupt(){CCR.TCIE = true; return *this;}

   uint16_t qty_transactions_left(){return CNDTR;}

   

};

#if not defined(USE_MOCK_DMA_STREAM)
SFINAE(DMA1_stream1,DMA_stream) make_reference() {return *reinterpret_cast<DMA_stream*>(DMA1_Channel1_BASE);}
SFINAE(DMA1_stream2,DMA_stream) make_reference() {return *reinterpret_cast<DMA_stream*>(DMA1_Channel2_BASE);}
SFINAE(DMA1_stream3,DMA_stream) make_reference() {return *reinterpret_cast<DMA_stream*>(DMA1_Channel3_BASE);}
SFINAE(DMA1_stream4,DMA_stream) make_reference() {return *reinterpret_cast<DMA_stream*>(DMA1_Channel4_BASE);}
SFINAE(DMA1_stream5,DMA_stream) make_reference() {return *reinterpret_cast<DMA_stream*>(DMA1_Channel5_BASE);}
#if defined(STM32F1)
SFINAE(DMA1_stream6,DMA_stream) make_reference() {return *reinterpret_cast<DMA_stream*>(DMA1_Channel6_BASE);}
SFINAE(DMA1_stream7,DMA_stream) make_reference() {return *reinterpret_cast<DMA_stream*>(DMA1_Channel7_BASE);}
#endif
#endif


constexpr IRQn_Type DMA_stream::IRQn(Periph v)
{
   return
      v == Periph::DMA1_stream1 ? DMA1_Channel1_IRQn :
   #if defined(STM32F1)
      v == Periph::DMA1_stream2 ? DMA1_Channel2_IRQn :
      v == Periph::DMA1_stream3 ? DMA1_Channel3_IRQn :
      v == Periph::DMA1_stream4 ? DMA1_Channel4_IRQn :
      v == Periph::DMA1_stream5 ? DMA1_Channel5_IRQn :
      v == Periph::DMA1_stream6 ? DMA1_Channel6_IRQn :
      v == Periph::DMA1_stream7 ? DMA1_Channel7_IRQn :
   #elif defined(STM32F0)
         v == Periph::DMA1_stream2 
      or v == Periph::DMA1_stream3 ? DMA1_Channel2_3_IRQn :
         v == Periph::DMA1_stream4
      or v == Periph::DMA1_stream5 ? DMA1_Channel4_5_6_7_IRQn :
   #endif
      NonMaskableInt_IRQn;
}



} //namespace mcu
