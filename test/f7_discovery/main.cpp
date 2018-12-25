#define STM32F746xx
#define F_CPU   216'000'000UL

#include "init_clock.h"
#include "timers.h"
#include "pin.h"
#include "flash.h"
#include "meta.h"


volatile auto& rcc = mcu::make_reference<mcu::Periph::RCC>();


/// эта функция вызываеться первой в startup файле
extern "C" void init_clock()
{
   init_clock<25_MHz,F_CPU>();
}

struct Data {
   int d1 {1};
   int d2 {2};
};
Flash<Data, mcu::FLASH::Sector::_7> flash {};

int main()
{
   auto pin = Pin::make<mcu::PI1, mcu::PinMode::Output>();

   Timer timer {200_ms};
   
   while (1) {

      if (timer.event())
         pin ^= 1;
      
   } // while(1) {

}