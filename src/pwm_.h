#pragma once

#include "periph_tim.h"
#include "pin.h"
#include "literals.h"

#if defined(USE_MOCK_GPIO)
using namespace mock;
#else
using namespace mcu;
#endif

class PWM 
{
   TIM& tim;
   Pin& pin;
   uint16_t CNT {0xFFFF};
   
   const TIM::Channel channel;
   const mcu::TIM::EnableMask enable_mask;

   const uint16_t max_duty_cycle;
   constexpr static uint16_t min_frequency {F_CPU / 0xFFFF};

   class Frequency {
      PWM& parent;
   public:
      Frequency (PWM& parent) : parent {parent} {}
      uint16_t value{0};
           operator    uint16_t () {return value;}
      void operator+= ( int16_t v) {operator= (value + v); }
      void operator=  (uint16_t v)
      {
         if ((v != value) and (v >= parent.min_frequency)) {
            value = v;
            parent.CNT = F_CPU / value;
            parent.tim.set_auto_reload(parent.CNT);
            parent.duty_cycle.update();
         }
      }
   };

   class Duty_cycle {
      PWM& parent;
   public:
      Duty_cycle (PWM& parent) : parent {parent} {}
      uint16_t value{0};
           operator    uint16_t () {return value;}
      void operator+= ( int16_t v) {operator= (value + v); }
      void operator=  (uint16_t v)
      {
         if ((v != value) and (v > 0) and (v <= parent.max_duty_cycle)) {  
            value = v;
            parent.tim.set_compare(parent.channel, parent.CNT * value/100);
         }
      }
      void update() {
         parent.tim.set_compare(parent.channel, parent.CNT * value/100);
      }
   };

public:
   PWM (TIM& tim
      , Pin& pin
      , TIM::Channel channel
      , TIM::EnableMask enable_mask
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
   
   template <Periph tim_, class Pin_>
   static auto make (uint16_t max_duty_cycle = 100)
   {
      TIM::pin_static_assert<tim_, Pin_>();
      
      constexpr auto pin_mode = TIM::pin_mode<tim_, Pin_>();
      constexpr auto channel_  = TIM::channel<tim_, Pin_>();
   
      static PWM pwm {
         make_reference<tim_>(),
         Pin::make<Pin_, pin_mode>(),
         TIM::channel<tim_, Pin_>(),
         TIM::enable_mask<channel_>(),
         max_duty_cycle
      };
      make_reference<Periph::RCC>().clock_enable<tim_>();
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