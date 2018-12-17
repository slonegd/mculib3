#pragma once

// #include "timers.h"
#include "tim.h"
#include "heap.h"

namespace mcu {


class Delay 
{
   TIM& tim;
   Delay (TIM& tim) : tim {tim}{}

public:

   template<Periph tim>
   static auto& make ()
   {
      auto& delay = *new Delay {make_reference<tim>()};
      delay.tim.template clock_enable<tim>()
               .set_prescaller((F_CPU/2)/1000000 + 1);

      return delay;
   }

   void us (uint16_t time)
   {
      tim.reload_value(time)
         .clear_counter()
         .update_generation()
         .counter_enable()
         .set (TIM::OnePulseMode::counterStop);

      while(not tim.is_interrupt()){}
      tim.clear_interrupt_flag();
         // .counter_disable();
   }


};

// template <uint16_t us>
// std::enable_if_t<us >= 1000> delay()
// {
//    Timer timer {us/1000};
//    while (not timer.event()){}
//    timer.stop();
// }

// template <uint16_t us>
// std::enable_if_t<us < 1000> delay()
// {
//    constexpr uint32_t time = us * (F_CPU / 1'000'000);
//    auto& tick = make_reference<Periph::SysTick>();
   
//    uint32_t current_time = tick.get();
//    if (tick.get() > time) {
//       while ((current_time - time) < tick.get()){}
//    } else {
//       uint32_t rest = time - tick.get();
//       current_time = tick.get();
//       while (current_time > tick.get()) {
//          current_time = tick.get();
//       }
//       current_time = tick.get();
//       while ((current_time - rest) < tick.get()){}
//    }
// }


} //namespace mcu