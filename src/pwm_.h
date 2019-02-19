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
   // const __IO uint32_t& compare_value;
   
   const TIM::Channel channel;
   const mcu::TIM::EnableMask enable_mask;

public:
   PWM (TIM& tim
      , Pin& pin
      , TIM::Channel channel
      // ,__IO uint32_t& compare_value
      , TIM::EnableMask enable_mask
   ) 
      : tim{tim}
      , pin{pin}
      , channel {channel}
      // , compare_value {compare_value}
      , enable_mask {enable_mask}
   {}
   template <Periph tim_, class Pin_>
   static auto& make ()
   {
      constexpr auto pin_mode = TIM::pin_mode<tim_, Pin_>();
      constexpr auto channel_  = TIM::channel<tim_, Pin_>();
      static_assert (
         channel_ != TIM::Channel::error,
         "Вывод контроллера не поддерживает функцию ШИМ с этим таймером"
      );
      static PWM pwm {
         make_reference<tim_>(),
         Pin::make<Pin_, pin_mode>(),
         TIM::channel<tim_, Pin_>(),
         // make_reference<tim_>().get_compare_reference<channel>(),
         TIM::enable_mask<channel_>()
      };
      make_reference<Periph::RCC>().clock_enable<tim_>();
      pwm.tim.template set<channel_> (TIM::CompareMode::PWMmode)
             .auto_reload_enable()
            //  .main_output_enable()
             .counter_enable();
      return pwm;
   }

   // struct Frequency {
   //    uint16_t value{0};
   //    void operator= (uint16_t v) {tim.set_auto_reload(0xFFFF*v/100);}
   // } frequency;

   // struct Duty_cycle {
   //    uint16_t value{0};
   // } duty_cycle;

   void duty_cycle(uint16_t v){tim.set_compare(channel, CNT*v/100);}
   void freq (uint16_t v){CNT = F_CPU / v; tim.set_auto_reload(CNT);}
   void out_enable()  { tim.compare_enable (enable_mask);}
   void out_disable() { tim.compare_disable(enable_mask);}
};