#pragma once

#include "bits_dma_stream_f0_f1.h"
#include "periph_dma.h"

namespace mcu {

class DMA_stream {
protected:
   volatile DMA_bits::CCR CCR;   // DMA channel x configuration register      offset: 0x08 + 0d20 × (channel number – 1)
   volatile uint32_t      CNDTR; // DMA channel x number of data register     offset: 0x0C + 0d20 × (channel number – 1)
   volatile uint32_t      CPAR;  // DMA channel x peripheral address register offset: 0x10 + 0d20 × (channel number – 1)
   volatile uint32_t      CMAR;  // DMA channel x memory address register     offset: 0x14 + 0d20 × (channel number – 1)

public:
   using CMSIS_type = DMA_Channel_TypeDef;
   using DataSize   = DMA_bits::CCR::DataSize;
   using Priority   = DMA_bits::CCR::Priority;
   using Direction  = DMA_bits::CCR::Direction;
   using Channel    = DMA::Channel;

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

   DMA_stream& set       (Direction v) {CCR.DIR   = v; return *this;}
   DMA_stream& size_memory(DataSize v) {CCR.MSIZE = v; return *this;}
   DMA_stream& size_periph(DataSize v) {CCR.PSIZE = v; return *this;}

   DMA_stream& enable_transfer_complete_interrupt(){CCR.TCIE = true; return *this;}

   uint16_t qty_transactions_left(){return CNDTR;}

   template<Periph stream> static constexpr Periph dma_periph();
   template<Periph stream> static constexpr Channel channel();


};

#if not defined(USE_MOCK_DMA)
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

template<Periph stream> constexpr Periph DMA_stream::dma_periph()
{
   return Periph::DMA1;
}

template<Periph stream> constexpr DMA_stream::Channel DMA_stream::channel() 
{
   if      constexpr (stream == Periph::DMA1_stream1) return Channel::_1;
   else if constexpr (stream == Periph::DMA1_stream2) return Channel::_2;
   else if constexpr (stream == Periph::DMA1_stream3) return Channel::_3;
   else if constexpr (stream == Periph::DMA1_stream4) return Channel::_4;
   else if constexpr (stream == Periph::DMA1_stream5) return Channel::_5;
#if defined(STM32F1)
   else if constexpr (stream == Periph::DMA1_stream6) return Channel::_6;
   else if constexpr (stream == Periph::DMA1_stream7) return Channel::_7;
#endif
}



} //namespace mcu
