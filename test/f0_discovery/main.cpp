#define STM32F030x6
#define F_OSC   8000000UL
#define F_CPU   48000000UL
#include "rcc.h"
#include "pin.h"
#include "timers.h"
#include "pwm.h"

// аналогичный файл есть в каталоге mculib2
// библиотека | пустой | без таймера | с таймером  | pwm
// mculib2    | 448    | 796 (+348)  | 1012 (+216) | 1844 (+832)
// mculib3    | 448    | 584 (+136)  | 812  (+228) | 1548 (+736)
// не знаю почему, но компилятор не заинлайнил
// функции RCC для случая с mculib2
// + иницализаия порта в mculib3 не полная, как в mculib2,
// а только необходимая
// некоторое увеличени с таймером от того, что в mculib2
// используется не atomic версия toggle
// разница в 100 тактов с pwm так же из-за не полной инициализации пина

/// эта функция вызываеться первой в startup файле
extern "C" void init_clock ()
{
   // FLASH::set (FLASH::Latency::_1);

   mcu::make_reference<mcu::Periph::RCC>()
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

   auto pwm = mcu::PWM::make<mcu::Periph::TIM1, mcu::PA8>();
   pwm.fill_ratio = 500_from1000;
   pwm.frequency  = 20_kHz;
   pwm.out_enable();


   while(1) {

      if (timer.event()) {
         pc8 ^= 1;
      }

   } // while(1) {

}
