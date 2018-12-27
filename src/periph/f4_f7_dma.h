#pragma once

#include "f4_f7_bits_dma.h"
#include "f4_f7_bits_dma_stream.h"

namespace mcu {

class DMA 
{
   volatile DMA_bits::ISR  ISR;  // DMA low/high interrupt status register,     offset: 0x00
   volatile DMA_bits::IFCR IFCR; // DMA low/high interrupt flag clear register, offset: 0x08

public:
   using CMSIS_type = DMA_TypeDef;
   using Mask       = DMA_bits::IFCR::Mask;
   using Channel    = DMA_bits::CR::Channel;

   auto& like_CMSIS() { return *reinterpret_cast<CMSIS_type*>(this); }

   void clear_interrupt_flags         (Channel);
   bool is_transfer_complete_interrupt(Channel);
};

#if not defined(USE_MOCK_DMA)
template <Periph p> std::enable_if_t<p == Periph::DMA1, DMA&> make_reference() {return *reinterpret_cast<DMA*>(DMA1_BASE);}
template <Periph p> std::enable_if_t<p == Periph::DMA2, DMA&> make_reference() {return *reinterpret_cast<DMA*>(DMA2_BASE);}
#endif
















void DMA::clear_interrupt_flags(Channel v)
{
   if      (v == Channel::_0) IFCR._0 = Mask::clear;
   else if (v == Channel::_1) IFCR._1 = Mask::clear;
   else if (v == Channel::_2) IFCR._2 = Mask::clear;
   else if (v == Channel::_3) IFCR._3 = Mask::clear;
   else if (v == Channel::_4) IFCR._4 = Mask::clear;
   else if (v == Channel::_5) IFCR._5 = Mask::clear;
   else if (v == Channel::_6) IFCR._6 = Mask::clear;
   else if (v == Channel::_7) IFCR._7 = Mask::clear;
}

bool DMA::is_transfer_complete_interrupt(Channel v)
{
   if      (v == Channel::_0) return ISR.TCIF0;
   else if (v == Channel::_1) return ISR.TCIF1;
   else if (v == Channel::_2) return ISR.TCIF2;
   else if (v == Channel::_3) return ISR.TCIF3;
   else if (v == Channel::_4) return ISR.TCIF4;
   else if (v == Channel::_5) return ISR.TCIF5;
   else if (v == Channel::_6) return ISR.TCIF6;
   else if (v == Channel::_7) return ISR.TCIF7;
   else return false;
}

} //namespace mcu