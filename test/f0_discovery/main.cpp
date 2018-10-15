#define STM32F030x6
#define F_OSC   8000000UL
#define F_CPU   48000000UL
#include "rcc.h"
#include "pin.h"

// аналогичный файл есть в каталоге mculib2
// библиотека | размер пустой | размер
// mculib2    | 448           | 796
// mculib3    | 448           | 584
// не знаю почему, но компилятор не заинлайнил
// функции RCC для случая с mculib2
// + иницализаия порта в mculib3 не полная, как в mculib2,
// а только необходимая

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
   pc8 = true;

   while(1) {
      
   } // while(1) {

}