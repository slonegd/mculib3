#pragma once

#include "periph_tim.h"
#include "pin.h"
#include "literals.h"

#if defined(USE_MOCK_TIM)
using TIM = mock::TIM;
#else
using TIM = mcu::TIM;
#endif

class PWM
{
   TIM &tim;
   Pin &pin;
   uint16_t CNT{0xFFFF};

   const TIM::Channel channel;
   const TIM::EnableMask enable_mask;

   const uint16_t max_duty_cycle;
   constexpr static uint16_t min_frequency{F_CPU / 0xFFFF};

   class Frequency
   {
      uint16_t value{0};
      PWM &parent;

   public:
      Frequency(PWM &parent) : parent{parent} {}
      operator uint16_t() { return value; }
      void operator+=(int16_t v) { operator=(value + v); }
      void operator=(uint16_t v)
      {
         if ((v != value) and (v >= parent.min_frequency))
         {
            value = v;
            parent.CNT = (F_CPU / value - 1) / 2;
            parent.tim.set_auto_reload(parent.CNT);
            parent.duty_cycle.update();
         }
      }
   };

   class Duty_cycle
   {
      uint16_t value{0};
      PWM &parent;

   public:
      Duty_cycle(PWM &parent) : parent{parent} {}
      operator uint16_t() { return value; }
      uint16_t operator+=(int16_t v) { return operator=(value + v); }
      uint16_t operator=(uint16_t v)
      {
         if ((v != value) and (v >= 0) and (v <= parent.max_duty_cycle))
         {
            value = v;
            parent.tim.set_compare(parent.channel, parent.CNT * value / 1000);
         }

         return v;
      }
      void update()
      {
         parent.tim.set_compare(parent.channel, parent.CNT * value / 1000);
      }
   };

public:
   PWM(TIM &tim, Pin &pin, TIM::Channel channel, TIM::EnableMask enable_mask, uint16_t max_duty_cycle)
       : tim{tim}, pin{pin}, channel{channel}, enable_mask{enable_mask}, max_duty_cycle{max_duty_cycle}
   {
   }

   Frequency frequency{*this};
   Duty_cycle duty_cycle{*this};

   template <mcu::Periph tim_, class Pin_>
   static auto &make(uint16_t max_duty_cycle = 500)
   {
      TIM::pin_static_assert<tim_, Pin_>();

      constexpr auto pin_mode = TIM::pin_mode<tim_, Pin_>();
      constexpr auto channel_ = TIM::channel<tim_, Pin_>();

      static PWM pwm{
          mcu::make_reference<tim_>(),
          Pin::make<Pin_, pin_mode>(),
          TIM::channel<tim_, Pin_>(),
          TIM::enable_mask<channel_>(),
          max_duty_cycle};
      mcu::make_reference<mcu::Periph::RCC>().clock_enable<tim_>();
      pwm.tim.template set<channel_>(TIM::CompareMode::PWMmode)
          .template preload_enable<channel_>()
          .auto_reload_enable()
          .counter_enable();

      pwm.frequency = pwm.min_frequency;
      // pwm.out_disable();
      return pwm;
   }

   // void duty_cycle(uint16_t v){tim.set_compare(channel, CNT*v/100);}
   // void freq (uint16_t v){CNT = F_CPU / v; tim.set_auto_reload(CNT);}
   void out_enable() { tim.compare_enable(enable_mask); }
   void out_disable() { tim.compare_disable(enable_mask); }

   void out_enable(bool v) { v ? tim.compare_enable(channel) : tim.compare_disable(channel);}

   bool operator^= (bool v) 
   {
      auto tmp {tim.is_compare(channel)};
      if (v) tmp ? tim.compare_disable(channel) : tim.compare_enable(channel);
      return tmp ^ v;
   }
};