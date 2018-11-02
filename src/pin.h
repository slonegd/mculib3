#pragma once

#include "gpio.h"
#include "pins.h"

namespace mcu {

class Pin {
   GPIO& port;
   const int n;
public:
   Pin (GPIO& port, int n) : port{port}, n{n} {}

   template<class Pin_, PinMode mode> static auto make()
   {
      Pin pin { mcu::make_reference<Pin_::periph>(), Pin_::n };
      pin.port.template clock_enable<Pin_::periph>()
              .template init<Pin_, mode>();
      return pin;
   }

   void toggle()    { port.toggle(n);          }
   void set()       { port.set(n);             }
   void clear()     { port.clear(n);           }
   bool is_set()    { return port.is_set(n);   }

   bool operator=  (bool v) { v ? set() : clear(); return v; }
   bool operator^= (bool v) { if (v) toggle();     return v; }
   operator bool() { return is_set(); }
};

} // namespace mcu {