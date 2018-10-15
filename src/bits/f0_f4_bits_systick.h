#pragma once

#include <cstdint>

namespace mcu::SysTick_bits {

struct CTRL {
   enum Clock { external = 0b0, processor = 0b1 };

   bool     ENABLE    :1;  // bit 0: Indicates the enabled status of the SysTick counter
   bool     TICKINT   :1;  // bit 1: Indicates whether counting to 0 causes the status of the SysTick exception to change to pending
   Clock    CLKSOURCE :1;  // bit 2: Indicates the SysTick clock source
   uint32_t res1      :13; // bit 15-3: reserved 
   bool     COUNTFLAG :1;  // bit 16: Indicates whether the counter has counted to 0 since the last read of this register
   uint32_t res2      :15; // bit 17-31: reserved
};


struct CALIB {
   uint32_t TENMS :24; // Optionally, holds a reload value to be us ed for 10ms (100Hz) timing, subject to system clock
                       // skew errors. If this field is zero, the calibration value is not known.
   uint32_t res1  :6;  // bit 29-24: reserved 
   bool     SKEW  :1;  // bit 30 Indicates whether the 10ms calibration value is exact
   bool     NOREF :1;  // bit 31 Indicates whether the IMPLEMENTATION DEFINED reference clock is provided
};

} // namespace mcu::SysTick_bits {



