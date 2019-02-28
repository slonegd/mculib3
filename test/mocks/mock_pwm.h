#pragma once

#define USE_MOCK_PWM
#include "mock_tim.h"
#include "pwm_.h"
#include "process.h"

namespace mock {

class PWM : public ::PWM
{
   Process& process { Process::make() };
   PWM() = default;
   PWM(const ::PWM& v) : ::PWM(v) {}
   auto& base() { return *static_cast<::PWM*>(this); }
public:
   template <mcu::Periph tim, class Pin_out>
   static auto& make()
   {
      static auto pwm = PWM(::PWM::template make<tim, Pin_out>());
      pwm.process.clear();
      return pwm;
   }
   friend std::ostream& operator<< (std::ostream&, const TIM&);

};

} //namespace mock