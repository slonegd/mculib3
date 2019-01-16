#pragma once

#include "timers.h"

#ifdef USE_MOCK_SYSTICK
using namespace mock;
#else
using namespace mcu;
#endif


template <uint32_t us>
std::enable_if_t<us >= 1000> delay()
{
   constexpr uint32_t time = us/1000;
   Timer timer {time};
   while (not timer.event()){}
   timer.stop();
}

template <uint32_t us>
std::enable_if_t<us < 1000> delay()
{
   constexpr uint32_t time = us * (F_CPU / 1'000'000);
   auto& tick = mcu::make_reference<mcu::Periph::SysTick>();
   
   uint32_t current_time = tick.get();
   if (tick.get() > time) {
      while ((current_time - time) < tick.get()){}
   } else {
      uint32_t rest = time - tick.get();
      current_time = tick.get();
      while (current_time > tick.get()) {
         current_time = tick.get();
      }
      current_time = tick.get();
      while ((current_time - rest) < tick.get()){}
   }
}
