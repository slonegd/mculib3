#define STM32F405xx
#define F_OSC   8000000UL
#define F_CPU   168000000UL

// #include <iostream>

#include "periph_rcc.h"
#include "pin.h"
// #include "buttons.h"
#include "flash.h"
#include "literals.h"

#include "strings.h"


/// эта функция вызываеться первой в startup файле
extern "C" void init_clock ()
{
   mcu::make_reference<mcu::Periph::FLASH>()
      .set (mcu::FLASH::Latency::_5);

   mcu::make_reference<mcu::Periph::RCC>()
      .on_HSE()
      .wait_HSE_ready()
      .set      (mcu::RCC::AHBprescaler::AHBnotdiv)
      .set_APB1 (mcu::RCC::APBprescaler::APBdiv4)
      .set_APB2 (mcu::RCC::APBprescaler::APBdiv2)
      .set      (mcu::RCC:: SystemClock::CS_PLL)
      .set_PLLM<4>()
      .set_PLLN<168>()
      .set      (mcu::RCC::     PLLPdiv::PLLdiv2)
      // .set_PLLQ<4>()
      .set      (mcu::RCC::   PLLsource::sHSE)
      .on_PLL()
      .wait_PLL_ready();
}


int main()
{
   auto led = mcu::Pin::make<mcu::PC8, mcu::PinMode::Output>();

   struct Data {
      size_t d1 {1};
      size_t d2 {2};
   };

   Flash<Data, mcu::FLASH::Sector::_11> flash{};
   Timer timer {1_s};

   volatile auto size1 = sizeof(cp1251);
   volatile auto size2 = sizeof(utf8);
   
   while(1) {
      
      timer.event ([&](){
         led.toggle();
         flash.d1++;
      });

      __WFI();

   } // while(1) {

}