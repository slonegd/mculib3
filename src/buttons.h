#pragma once

#include "timers.h"
#include "literals.h"
#if not defined (TEST)
   #include "pin.h"
#endif
#include <tuple>
#include <algorithm>

namespace mcu {

class Button : TickSubscriber
{
   Pin& pin;

   size_t time {0};
   bool short_press {false};
   bool long_press {false};
   bool increment  {false};
   bool inverted   {false};
   int  inc {1};
   int  value{0};
   int  tmp{0};

   Button (Pin& pin, bool inverted) : pin {pin}, inverted {inverted} {}

public:

   template<class Pin_, bool inverted = false> 
   static auto& make()
   {
      auto& button = *new Button {Pin::template make_new<Pin_, PinMode::Input>(), inverted};
      return button;
   }

   bool push()
   {
      if (inverted) {
         if (not pin) 
            subscribe();
      } else {
         if (pin)
            subscribe();
      }

      return time >= 10 and not short_press ? (short_press = true) : false;
   }

   bool push_long()
   {
      if (inverted) {
         if (not pin) 
            subscribe();
      } else {
         if (pin)
            subscribe();
      }

      return time >= 1000 and not long_press ? (long_press = true) : false;
   }
   static constexpr uint16_t MinPressed = 10_ms;
   static constexpr uint16_t LongPressed = 1_s;

   operator int()
   {
      if (increment) {
         if (time <= 2000)
            value = (time >= tmp + 200) ? tmp = time, inc : 0;
         else if (time < 3000)
            value = (time >= tmp + 100) ? tmp = time, inc : 0;
         else if (time < 4000)
            value = (time >= tmp + 50 ) ? tmp = time, inc : 0;
         else if (time < 5000)
            value = (time >= tmp + 25 ) ? tmp = time, inc : 0;
         else if (time >= 5000) {
            value = (time >= tmp + 25 ) ? tmp = time, inc : 0;
            inc += (time % 1000) == 0;
         }
         
      } else if (push()) 
         value = 1;
       else if (push_long()) {
         increment = true;
         tmp = time;
         value = 1;
      } else 
         value = 0;

      return value;
   }

   void notify () override
   {
      if (inverted) {
         if (pin) {
            unsubscribe();
            time = 0;
            short_press = false;
            long_press = false;
            increment  = false;
            inc        = 1;
         } else 
            time++;
      } else {
         if (not pin) {
            unsubscribe();
            time = 0;
            short_press = false;
            long_press = false;
            increment  = false;
            inc        = 1;
         } else 
            time++;
      }
   }

   bool is_set()
   {
      if (inverted) return not pin;
      else return pin;
   }

   struct Some 
   {
      Button& one;
      Button& two;
      Some(Button& one, Button& two) : one{one}, two{two} {}
      operator bool () {
         if (one.is_set() and two.is_set()) {
            one.subscribe();
            two.subscribe(); 
         }
         bool result{false};
         result = one.time >= 10 and two.time >= 10 and (not one.short_press or not two.short_press);
         if (result)
            one.short_press = two.short_press = true;
         return result;
      }


      bool push_long() {
         if (one.is_set() and two.is_set()) {
            one.subscribe();
            two.subscribe(); 
         }
         bool result{false};
         result = one.time >= 1000 and two.time >= 1000 and (not one.long_press or not two.long_press);
         if (result)
            one.long_press = two.long_press = true;
         return result;
      }
   };

   Some operator and (Button& other) {return Some{*this, other};}
   friend Some;

};


// bool operator and (Button& _1, Button& _2) {return _1.push_long() and _2.push_long();}


// auto accelerate(auto i)
//    {
//       if(push_long()) {

//       }
//    }

//    size_t operator++ (time)
//    {
//       if (time > 1000){

//       } else if (time > 2000) {

//       } else if (time > 3000) {
         
//       }
//    }

// auto button = std::tuple {
//          *new Button {Pin::template make_new<Pins, PinMode::Input>()} ... 
//       };
//       return button;


// class Double_press
// {
//    Button& _1;
//    Button& _2;
//    Double_press (Button& _1, Button& _2) : _1 {_1}, _2 {_2} {}
//    bool operator and (Button& _1, Button& _2) {return _1.push_long() and _2.push_long();}
// };
// // template <int qty>
// class Button : TickSubscriber
// {
//    // using Array = std::array<Pin, qty>;
//    // Array pins;
//    Pin pin;
//    Timer timer;

//    bool brief_pressed {false};
//    bool long_pressed  {false};

// public:

//    static constexpr uint16_t shot_press = 10_ms;
//    static constexpr uint16_t long_press = 1_s;

//    Button (pin) : pin{pin} {timer.timeSet = 100_s; subscribe();}

//    template <class ... Pins>
//    // static auto <sizeof ... (Pins)> make ()
//    auto make()
//    {
//       // std::array<Pin, sizeof ... (Pins)> pins {Pin::template make<Pins, PinMode::Input>() ... };
//       // Buttons_ <sizeof ... (Pins)> buttons {pins};
//       return std::tuple {Button {Pin::template make<Pins, PinMode::Input>() ... };
//    }

//    // запускает/останавливает таймер отслеживания времени нажатия
//    void notify() override
//    {
//       bool tmp = std::any_of(pins.cbegin(), pins.cend(), [](auto p){return p.is_set();});
//       if (tmp) {
//          timer.start();
//       } else {
//          timer.stop();
//       }
//    }

   // определяет событие нажатия кнопки (или кнопок если их несколько)
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


   // using Buttons = Buttons_<0>;

}
