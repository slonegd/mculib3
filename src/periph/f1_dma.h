#pragma once

#include "f1_bits_dma.h"

namespace mcu {

class DMA 
{
   __IO DMA_bits::ISR  ISR;  // DMA interrupt status register     offset: 0x00
   __IO DMA_bits::IFCR IFCR; // DMA interrupt flag clear register offset: 0x04

public:
   using CMSIS_type = DMA_TypeDef;
   enum class Channel { _1 = 1, _2, _3, _4, _5, _6, _7, error };

   auto& like_CMSIS() { return *reinterpret_cast<CMSIS_type*>(this); }

   void clear_interrupt_flags         (Channel);
   bool is_transfer_complete_interrupt(Channel);
};

#if not defined(USE_MOCK_DMA)
template <Periph p> std::enable_if_t<p == Periph::DMA1, DMA&> make_reference() {return *reinterpret_cast<DMA*>(DMA1_BASE);}
#endif















void DMA::clear_interrupt_flags(Channel channel)
{
   if      (channel == Channel::_1) IFCR.CGIF1 = true;
   else if (channel == Channel::_2) IFCR.CGIF2 = true;
   else if (channel == Channel::_3) IFCR.CGIF3 = true;
   else if (channel == Channel::_4) IFCR.CGIF4 = true;
   else if (channel == Channel::_5) IFCR.CGIF5 = true;
   else if (channel == Channel::_6) IFCR.CGIF6 = true;
   else if (channel == Channel::_7) IFCR.CGIF7 = true;
}

bool DMA::is_transfer_complete_interrupt(Channel channel)
{
   if      (channel == Channel::_1) return ISR.TCIF1;
   else if (channel == Channel::_2) return ISR.TCIF2;
   else if (channel == Channel::_3) return ISR.TCIF3;
   else if (channel == Channel::_4) return ISR.TCIF4;
   else if (channel == Channel::_5) return ISR.TCIF5;
   else if (channel == Channel::_6) return ISR.TCIF6;
   else if (channel == Channel::_7) return ISR.TCIF7;
   else return false;
}

} //namespace mcu