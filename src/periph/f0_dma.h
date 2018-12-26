#pragma once

#include "f0_bits_dma.h"

namespace mcu {

class DMA
{
   volatile DMA_bits::ISR  ISR;  // DMA interrupt status register     offset: 0x00
   volatile DMA_bits::ISR  IFCR; // DMA interrupt flag clear register offset: 0x04

public:
   using CMSIS_type = DMA_TypeDef;
   enum class Channel { _1 = 1, _2, _3, _4, _5, error };

   auto& like_CMSIS() { return *reinterpret_cast<CMSIS_type*>(this); }

   void clear_interrupt_flags         (Channel);
   bool is_transfer_complete_interrupt(Channel);
};

#if not defined(USE_MOCK_GPIO)
template <Periph dma> std::enable_if_t<dma == Periph::DMA1, DMA&> make_reference() {return *reinterpret_cast<DMA*>(DMA1_BASE);}
#endif







void DMA::clear_interrupt_flags(Channel v)
{
   if      (v == Channel::_1) IFCR.GIF1 = true;
   else if (v == Channel::_2) IFCR.GIF2 = true;
   else if (v == Channel::_3) IFCR.GIF3 = true;
   else if (v == Channel::_4) IFCR.GIF4 = true;
   else if (v == Channel::_5) IFCR.GIF5 = true;
}

bool DMA::is_transfer_complete_interrupt(Channel v)
{
   if      (v == Channel::_1) return ISR.TCIF1;
   else if (v == Channel::_2) return ISR.TCIF2;
   else if (v == Channel::_3) return ISR.TCIF3;
   else if (v == Channel::_4) return ISR.TCIF4;
   else if (v == Channel::_5) return ISR.TCIF5;
   else return false;
}

} // namespace mcu {