#pragma once

#include "periph_gpio.h"
#include "pins.h"
#include "meta.h"

#if defined(USE_MOCK_GPIO)
using GPIO = mock::GPIO;
#else
using GPIO = mcu::GPIO;
#endif

class Pin
{
   GPIO &port;
   const int n;

public:
   Pin(GPIO &port, int n) : port{port}, n{n} {}

   template <class Pin_, mcu::PinMode mode = mcu::PinMode::Input>
   static auto &make()
   {
      static Pin pin{mcu::make_reference<Pin_::periph>(), Pin_::n};
      mcu::make_reference<mcu::Periph::RCC>()
          .template clock_enable<Pin_::periph>();
      pin.port.template init<Pin_, mode>();
      return pin;
   }

   void set() { port.set(n); }
   void clear() { port.clear(n); }
   bool is_set() { return port.is_set(n); }
   void toggle() { port.toggle(n); }

   bool operator=(bool v)
   {
      v ? set() : clear();
      return v;
   }
   bool operator^=(bool v)
   {
      auto tmp{is_set()};
      if (v)
         toggle();
      return tmp ^ v;
   }
   operator bool() { return is_set(); }
};

template <mcu::PinMode mode, class... Pins>
meta::tuple_generate_t<Pin &, sizeof...(Pins)> make_pins()
{
   return std::tie(Pin::make<Pins, mode>()...);
}
