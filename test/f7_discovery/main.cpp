#define STM32F746xx
#define F_CPU   216'000'000UL

#include "init_clock.h"
#include "timers.h"
#include "pin.h"


volatile auto& rcc = mcu::make_reference<mcu::Periph::RCC>();


/// эта функция вызываеться первой в startup файле
extern "C" void init_clock()
{
   init_clock_<F_CPU>();
}


int main()
{
   
   auto pin = mcu::Pin::make<mcu::PI1, mcu::PinMode::Output>();

   Timer timer {200_ms};
   
   while (1) {

      if (timer.event())
         pin ^= 1;
      
   } // while(1) {

}