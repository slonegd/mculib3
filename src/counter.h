#pragma once

#include "periph_tim.h"
#include "periph_rcc.h"
#include "pin.h"
#include "interrupt.h"

#if defined(USE_MOCK_TIM)
using TIM = mock::TIM;
#else
using TIM = mcu::TIM;
#endif


class Counter
{
   Pin& pin;
   TIM& tim;
   Interrupt& interrupt_tim;
   const uint16_t pulse_per_value;

   uint16_t value{0};

   void TIMinterrupt ()
   {
      if (tim.update_interrupt()) {
         value++;
      }
   }

   using Parent = Counter;

   struct tim_interrupt : Interrupting
   {
       Parent& parent;
       tim_interrupt (Parent& parent) : parent(parent) {
           parent.interrupt_tim.subscribe (this);
       }
       void interrupt() override {parent.TIMinterrupt();} 
   } tim_ {*this};

   Counter(Pin&pin, TIM& tim, Interrupt& interrupt_tim, uint16_t pulse_per_value)
      : pin {pin}
      , tim {tim}
      , interrupt_tim {interrupt_tim}
      , pulse_per_value {pulse_per_value}
   {}

public:

   template <mcu::Periph tim_, class Pin_>
   static auto& make(uint16_t pulse_per_value)
   {
      TIM::pin_static_assert<tim_, Pin_>();

      constexpr auto pin_mode_ = TIM::pin_mode<tim_, Pin_>();
      constexpr auto channel_ = TIM::channel<tim_, Pin_>();

      static Counter counter
      {
         Pin::make<Pin_, pin_mode_>(),
         mcu::make_reference<tim_>(),
         get_interrupt<tim_>(),
         pulse_per_value
      };

      mcu::make_reference<mcu::Periph::RCC>().clock_enable<tim_>();

      counter.tim.template set<channel_>(TIM::SelectionCompareMode::Input)
                 .template set<channel_>(TIM::Filter::Div_0_N_8)
                 .template set<channel_>(TIM::Polarity::rising)
                 .template set_trigger<channel_>()
                 .set(TIM::SlaveMode::ExternalClock)
                 .set_auto_reload(pulse_per_value);
                 
                 
      get_interrupt<tim_>().enable();
      counter.tim.update_interrupt_enable();
      counter.tim.counter_enable();

      return counter;
   }

   operator uint16_t(){return value;}
   void reset() {value = 0;}
};