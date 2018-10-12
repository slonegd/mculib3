#pragma once

#include "gpio.h"

namespace mcu {

class Pin {
   GPIO& port;
   const int n;
public:
   Pin (GPIO& port, int n) : port{port}, n{n} {}

   template<Periph p, int n, PinMode mode> static auto make()
   {
      Pin pin { mcu::GPIO::make_reference<p>(), n };
      pin.port.init<n,mode>();
      return pin;
   }

   void toggle() { port.toggle(n);        }
   void set()    { port.set(n);           }
   void clear()  { port.clear(n);         }
   bool is_set() { return port.is_set(n); }

   bool operator= (bool v) { v ? set() : clear(); return v; }
   operator bool() { return is_set(); }
};

} // namespace mcu {