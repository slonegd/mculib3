#pragma once

#include "periph_tim.h"
#include "pin.h"
#include "literals.h"

#if defined(USE_MOCK_GPIO)
using TIM_t = mock::TIM;
#else
using TIM_t = mcu::TIM;
#endif

class PWM 
{
   TIM_t& tim;
   Pin& pin;
   uint16_t CNT {0xFFFF};
   
   const TIM_t::Channel channel;
   const TIM_t::EnableMask enable_mask;

   uint16_t frequency_value {0};
   uint16_t duty_cycle_value{0};

   const uint16_t max_duty_cycle;
   constexpr static uint16_t min_frequency {(F_CPU / 0xFFFF) + 1};

   class Frequency {
      PWM& parent;
   public:
      Frequency (PWM& parent) : parent {parent} {}
           operator    uint16_t () {return parent.frequency_value;}
      void operator+= ( int16_t v) {operator= (parent.frequency_value + v); }
      void operator=  (uint16_t v)
      {
         if ((v != parent.frequency_value) and (v >= parent.min_frequency)) {
            parent.frequency_value = v;
            parent.CNT = (F_CPU / parent.frequency_value);
            parent.tim.set_auto_reload(parent.CNT);
            parent.duty_cycle.update();
         }
      }
   };

   class Duty_cycle {
      PWM& parent;
   public:
      Duty_cycle (PWM& parent) : parent {parent} {}
           operator    uint16_t () {return parent.duty_cycle_value;}
      void operator+= ( int16_t v) {operator= (parent.duty_cycle_value + v); }
      void operator=  (uint16_t v)
      {
         if ((v != parent.duty_cycle_value) and (v > 0) and (v <= parent.max_duty_cycle)) {  
            parent.duty_cycle_value = v;
            parent.tim.set_compare(parent.channel, parent.CNT * parent.duty_cycle_value/100);
         }
      }
      void update() {
         parent.tim.set_compare(parent.channel, parent.CNT * parent.duty_cycle_value/100);
      }
   };

public:
   PWM (TIM_t& tim
      , Pin& pin
      , TIM_t::Channel channel
      , TIM_t::EnableMask enable_mask
      , uint16_t max_duty_cycle
   ) 
      : tim{tim}
      , pin{pin}
      , channel {channel}
      , enable_mask {enable_mask}
      , max_duty_cycle {max_duty_cycle}
   {}

   Frequency  frequency {*this};
   Duty_cycle duty_cycle{*this};
   
   template <mcu::Periph tim_, class Pin_>
   static auto make (uint16_t max_duty_cycle = 100)
   {
      TIM_t::pin_static_assert<tim_, Pin_>();
      
      constexpr auto pin_mode = TIM_t::pin_mode<tim_, Pin_>();
      constexpr auto channel_ = TIM_t::channel<tim_, Pin_>();
   
      static PWM pwm {
         mcu::make_reference<tim_>(),
         Pin::make<Pin_, pin_mode>(),
         TIM_t::channel<tim_, Pin_>(),
         TIM_t::enable_mask<channel_>(),
         max_duty_cycle
      };
      mcu::make_reference<mcu::Periph::RCC>().clock_enable<tim_>();
      pwm.tim.template set<channel_> (TIM::CompareMode::PWMmode)
             .auto_reload_enable()
             .counter_enable();

      pwm.frequency = pwm.min_frequency;
      return pwm;
   }

   // void duty_cycle(uint16_t v){tim.set_compare(channel, CNT*v/100);}
   // void freq (uint16_t v){CNT = F_CPU / v; tim.set_auto_reload(CNT);}
   void out_enable()  { tim.compare_enable (enable_mask);}
   void out_disable() { tim.compare_disable(enable_mask);}
};