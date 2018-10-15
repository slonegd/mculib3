#define STM32F030x6
#define F_OSC   8000000UL
#define F_CPU   48000000UL
#include "rcc.h"
#include "pin.h"
#include "timers.h"

// аналогичный файл есть в каталоге mculib2
// библиотека | пустой | без таймера | с таймером
// mculib2    | 448    | 796 (+348)  | 1012 (+216)
// mculib3    | 448    | 584 (+136)  | 812  (+228)
// не знаю почему, но компилятор не заинлайнил
// функции RCC для случая с mculib2
// + иницализаия порта в mculib3 не полная, как в mculib2,
// а только необходимая
// некоторое увеличени с тайсером от того, что в mculib2
// используется не atomic версия toggle

/// эта функция вызываеться первой в startup файле
extern "C" void init_clock ()
{
   // FLASH::set (FLASH::Latency::_1);

   mcu::RCC::make_reference()
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
   auto pc8 = mcu::Pin::make<mcu::PC8, mcu::PinMode::Output>();
   Timer timer {200};

   while(1) {

      if (timer.event())
         pc8 ^= 1;

   } // while(1) {

}