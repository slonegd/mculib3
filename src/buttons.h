#pragma once

#include "timers.h"
#include "literals.h"
#include "pin.h"
#include <array>
#include <algorithm>

namespace mcu {


template <int qty>
class Buttons_ : TickSubscriber
{
   using Array = std::array<Pin, qty>;
   Array pins;
   Timer timer;

public:

   static constexpr uint16_t MinPressed = 10_ms;
   static constexpr uint16_t LongPressed = 1_s;

   Buttons_ (Array pins) : pins {pins} {}

   template <class ... Pins>
   static Buttons_ <sizeof ... (Pins)> make ()
   {
      std::array<Pin, sizeof ... (Pins)> pins {Pin::template make<Pins, PinMode::Input>() ... };
      Buttons_ <sizeof ... (Pins)> buttons {pins};
      return buttons;
   }
   static constexpr uint16_t MinPressed = 10_ms;
   static constexpr uint16_t LongPressed = 1_s;

//    Buttons()
//    : pushHandeledFlag     {false},
//      longPushHandeledFlag {false}
//    {
//       template<size_t ... n, PinMode::Input> void GPIO::init();
//       timer.timeSet = 100_s; // 100 s просто большое время
//       subscribe();
//    }

   // запускает/останавливает таймер отслеживания времени нажатия
   void notify() override
   {
      bool tmp = std::any_of(pins.cbegin(), pins.cend(), [](auto p){return p.is_set();});
      if (tmp) {
         timer.start();
      } else {
         timer.stop();
      }
   }

   // // определяет событие нажатия кнопки (или кнопок если их несколько)
   // template<class ... Buttons_> bool push()
   // {
   //    bool tmp = (is_set() or ...);
   //    tmp &= timer.isGreater(MinPressed);
   //    return tmp;
   // }

   // // определяет событие долгого нажатия кнопки (или кнопок если их несколько)
   // template<class ... Buttons_> bool longPush()
   // {
   //    bool tmp = (Buttons_::isSet() or ...);
   //    tmp &= timer.isGreater(LongPressed);
   //    return tmp;
   // }

   // // определяет событие нажатия хоть какой-нибудь кнопки
   // bool anyPush()
   // {
   //    return timer.isGreater(MinPressed) and !pushHandeledFlag;
   // }


   // // блокирует события нажатия кнопок до отпускания
   // void blockAllEvents()
   // {
   //    pushHandeledFlag = true;
   //    longPushHandeledFlag = true;
   // }


   // // блокирует событие простого нажатия кнопок до отпускания
   // void blockPushEvents()
   // {
   //    pushHandeledFlag = true;
   // }



};

   using Buttons = Buttons_<0>;

}
