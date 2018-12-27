#pragma once

#define USE_MOCK_DMA
#include "periph_dma.h"
#include <iostream>

namespace mock {

class DMA : public mcu::DMA
{
   std::ostream* process {nullptr};
   DMA() = default;
public:
   static DMA& make()
   {
      static DMA dma;
      return dma;
   }
   void set_stream (std::ostream& s) { process = &s; }

};

} // namespace mock {

namespace mcu {
   SFINAE(DMA1,DMA) make_reference() { return mock::DMA::make(); }
}