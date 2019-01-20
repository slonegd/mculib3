#pragma once

#define USE_MOCK_DMA
#include "mock_rcc.h"
#include "periph_dma.h"
#include "mock_dma_stream.h"
#include "process.h"

namespace mock {

class DMA : public mcu::DMA
{
   Process& process { Process::make() };
   DMA() = default;
public:
   static DMA& make()
   {
      static DMA dma;
      return dma;
   }
};

} // namespace mock {

namespace mcu {
   SFINAE(DMA1,DMA) make_reference() { return mock::DMA::make(); }
}