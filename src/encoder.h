#pragma once

#include "periph_tim.h"
#include "periph_rcc.h"
#include "pin.h"

class Encoder
{
   Pin& pin_a;
   Pin& pin_b;
   TIM& tim;
   Encoder(Pin& pin_a, Pin& pin_b, TIM& tim)
      : pin_a{pin_a}
      , pin_b{pin_b}
      , tim{tim}
   {}
public:
   template <mcu::Periph tim_, class Pin_a, class Pin_b>
   static auto& make ()
   {
      TIM::pin_static_assert<tim_, Pin_a>();
      TIM::pin_static_assert<tim_, Pin_b>();

      constexpr auto pin_mode_a = TIM::pin_mode<tim_, Pin_a>();
      constexpr auto pin_mode_b = TIM::pin_mode<tim_, Pin_b>();
      constexpr auto channel_a = TIM::channel<tim_, Pin_a>();
      constexpr auto channel_b = TIM::channel<tim_, Pin_b>();

      static Encoder encoder
      {
         Pin::make<Pin_a, pin_mode_a>(),
         Pin::make<Pin_b, pin_mode_b>(),
         mcu::make_reference<tim_>()
      };

      mcu::make_reference<mcu::Periph::RCC>().clock_enable<tim_>();

      encoder.tim.template compare_enable<TIM::Channel::_3>()
                 .set(TIM::SlaveMode::Encoder3)
                 .template set<TIM::Channel::_3>(TIM::CompareMode::ActiveOnMatch)
                 .template set<channel_a>(TIM::SelectionCompareMode::Input)
                 .template set<channel_b>(TIM::SelectionCompareMode::Input)
                 .clear_counter()
                 .counter_enable();

      return encoder;
   }
   void set_compare(int16_t);
   int16_t operator=   (int16_t);
   int16_t operator!=  (int16_t);
   int16_t operator+   (int16_t);
   int16_t operator-   (int16_t);
   bool    operator>   (int16_t);
   bool    operator<   (int16_t);
   bool    operator>=  (int16_t);
   bool    operator<=  (int16_t);
           operator    int16_t(){return static_cast<int16_t>(tim.get_counter());}
};