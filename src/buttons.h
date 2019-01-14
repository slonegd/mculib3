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

   int time {0};
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
      static auto button = Button {Pin::template make<Pin_, PinMode::Input>(), inverted};
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

} //namespace mcu

