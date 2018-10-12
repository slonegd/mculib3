#define STM32F030x6
#define F_OSC   8000000UL
#define F_CPU   48000000UL
#include "rcc.h"

/// эта функция вызываеться первой в startup файле
extern "C" void init_clock ()
{
   // FLASH::set (FLASH::Latency::_1);

   mcu::RCC::make()
      .set (mcu::RCC:: AHBprescaler::AHBnotdiv)
      .set (mcu::RCC:: APBprescaler::APBnotdiv)
      .set (mcu::RCC::  SystemClock::CS_PLL)
      .set (mcu::RCC::    PLLsource::HSIdiv2)
      .set (mcu::RCC::PLLmultiplier::_12)
      .on_PLL()
      .wait_PLL_ready();
}


int main()
{
   mcu::RCC::make().clock_enable<Periph::GPIOA>();

   while(1) {
      
   } // while(1) {

}