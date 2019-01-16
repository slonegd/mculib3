#pragma once


#include "periph.h"
#include "f0_f4_bits_systick.h"

namespace mcu {


class SysTick {
protected:
   __IO SysTick_bits::CTRL     CTRL;  // Offset: 0x000 (R/W)  SysTick Control and Status Register
   __IO uint32_t               LOAD;  // Offset: 0x004 (R/W)  SysTick Reload Value Register
   __IO uint32_t               VAL;   // Offset: 0x008 (R/W)  SysTick Current Value Register
   __IO SysTick_bits::CALIB    CALIB; // Offset: 0x00C (R/ )  SysTick Calibration Register
public:
   using Clock = SysTick_bits::CTRL::Clock;
   
   void load (uint32_t v)   { LOAD = v; }
   void clear()             { VAL = 0; }
   void setSource (Clock v) { CTRL.CLKSOURCE = v; }
   void enableInterrupt()   { CTRL.TICKINT = true; }
   void enable()            { CTRL.ENABLE = true; }
   uint32_t get()           { return VAL; }


   template <uint16_t ms> void initInterrupt()
   {
      constexpr uint32_t val = F_CPU / (1000 * ms) - 1;
      static_assert (
         val <= 0xFFFFFF,
         "число должно быть 24-битное"
      );
      load (val);
      clear();
      setSource (Clock::processor);
      enableInterrupt();
      enable();
   }
};

#if not defined(USE_MOCK_SYSTICK)
SFINAE(SysTick, SysTick) make_reference() { return *reinterpret_cast<SysTick*>(SysTick_BASE); }
#endif 

} // namespace mcu {
