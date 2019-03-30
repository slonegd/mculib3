#pragma once

#include "bits_dma_f4_f7.h"
#include "bits_dma_stream_f4_f7.h"

namespace mcu {

class DMA 
{
   volatile DMA_bits::LISR  LISR;  // DMA low interrupt status register,      offset: 0x00
   volatile DMA_bits::HISR  HISR;  // DMA high interrupt status register,     offset: 0x04
   volatile DMA_bits::LIFCR LIFCR; // DMA low interrupt flag clear register,  offset: 0x08
   volatile DMA_bits::HIFCR HIFCR; // DMA high interrupt flag clear register, offset: 0x0C

public:
   using CMSIS_type = DMA_TypeDef;
   // using Mask       = DMA_bits::IFCR::Mask;
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
   if      (v == Channel::_0) registr(LIFCR) = DMA_LIFCR_CTCIF0_Msk | DMA_LIFCR_CHTIF0_Msk  | 
                                               DMA_LIFCR_CTEIF0_Msk | DMA_LIFCR_CDMEIF0_Msk | 
                                               DMA_LIFCR_CFEIF0_Msk;//IFCR._0 = Mask::clear;
   else if (v == Channel::_1) registr(LIFCR) = DMA_LIFCR_CTCIF1_Msk | DMA_LIFCR_CHTIF1_Msk  | 
                                               DMA_LIFCR_CTEIF1_Msk | DMA_LIFCR_CDMEIF1_Msk | 
                                               DMA_LIFCR_CFEIF1_Msk;//IFCR._1 = Mask::clear;
   else if (v == Channel::_2) registr(LIFCR) = DMA_LIFCR_CTCIF2_Msk | DMA_LIFCR_CHTIF2_Msk  | 
                                               DMA_LIFCR_CTEIF2_Msk | DMA_LIFCR_CDMEIF2_Msk | 
                                               DMA_LIFCR_CFEIF2_Msk;//IFCR._2 = Mask::clear;
   else if (v == Channel::_3) registr(LIFCR) = DMA_LIFCR_CTCIF3_Msk | DMA_LIFCR_CHTIF3_Msk  | 
                                               DMA_LIFCR_CTEIF3_Msk | DMA_LIFCR_CDMEIF3_Msk | 
                                               DMA_LIFCR_CFEIF3_Msk;//IFCR._3 = Mask::clear;
   else if (v == Channel::_4) registr(HIFCR) = DMA_HIFCR_CTCIF4_Msk | DMA_HIFCR_CHTIF4_Msk  | 
                                               DMA_HIFCR_CTEIF4_Msk | DMA_HIFCR_CDMEIF4_Msk | 
                                               DMA_HIFCR_CFEIF4_Msk;//IFCR._4 = Mask::clear;
   else if (v == Channel::_5) registr(HIFCR) = DMA_HIFCR_CTCIF5_Msk | DMA_HIFCR_CHTIF5_Msk  | 
                                               DMA_HIFCR_CTEIF5_Msk | DMA_HIFCR_CDMEIF5_Msk | 
                                               DMA_HIFCR_CFEIF5_Msk;//IFCR._5 = Mask::clear;
   else if (v == Channel::_6) registr(HIFCR) = DMA_HIFCR_CTCIF6_Msk | DMA_HIFCR_CHTIF6_Msk  | 
                                               DMA_HIFCR_CTEIF6_Msk | DMA_HIFCR_CDMEIF6_Msk | 
                                               DMA_HIFCR_CFEIF6_Msk;//IFCR._6 = Mask::clear;
   else if (v == Channel::_7) registr(HIFCR) = DMA_HIFCR_CTCIF7_Msk | DMA_HIFCR_CHTIF7_Msk  | 
                                               DMA_HIFCR_CTEIF7_Msk | DMA_HIFCR_CDMEIF7_Msk | 
                                               DMA_HIFCR_CFEIF7_Msk;//IFCR._7 = Mask::clear;
}

bool DMA::is_transfer_complete_interrupt(Channel v)
{
   if      (v == Channel::_0) return registr(LISR) & DMA_LISR_TCIF0_Msk;
   else if (v == Channel::_1) return registr(LISR) & DMA_LISR_TCIF1_Msk;
   else if (v == Channel::_2) return registr(LISR) & DMA_LISR_TCIF2_Msk;
   else if (v == Channel::_3) return registr(LISR) & DMA_LISR_TCIF3_Msk;
   else if (v == Channel::_4) return registr(HISR) & DMA_HISR_TCIF4_Msk;
   else if (v == Channel::_5) return registr(HISR) & DMA_HISR_TCIF5_Msk;
   else if (v == Channel::_6) return registr(HISR) & DMA_HISR_TCIF6_Msk;
   else if (v == Channel::_7) return registr(HISR) & DMA_HISR_TCIF7_Msk;
   else return false;

   // registr(ISR) & (1 << (5 + (static_cast<uint16_t>(v))*6));
}

} //namespace mcu