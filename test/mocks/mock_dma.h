#pragma once

#define USE_MOCK_DMA
#include "mock_rcc.h"
#include "periph_dma.h"
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

   auto& base() { return *static_cast<mcu::DMA*>(this); }

   // void clear_interrupt_flags(Channel v) override
   // {
   //    process << "очистка" << std::endl;
   //    if (go_depper)
   //       base().clear_interrupt_flags(v);
   //    else
   //       mock.is_tx_complete[v] = false;
   // }
   // bool is_transfer_complete_interrupt(Channel v) override
   // {
   //   if (go_depper)
   //       return base().is_transfer_complete_interrupt(v);
   //    else
   //       return mock.is_tx_complete[v];
   // }

   // struct Mock {
   //    DMA& parent;
   //    Mock (DMA& parent) : parent{parent} {}
   //    bool is_tx_complete[7] {false}; // 7 - placeholder
   //    void set_tx_complete (Channel v) ( is_tx_complete[v] = true; )
   // } mock {*this};
};

} // namespace mock {

namespace mcu {
   SFINAE(DMA1,mock::DMA) make_reference() { return mock::DMA::make(); }
}

// namespace mock {
//    auto& dma1 = REF(DMA1);
// }

#include "mock_dma_stream.h"