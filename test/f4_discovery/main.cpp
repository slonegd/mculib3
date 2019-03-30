#define STM32F405xx
#define F_OSC   8000000UL
#define F_CPU   168000000UL

#include "periph_rcc.h"
#include "periph_flash.h"
#include "pin.h"
#include "timers.h"
#include "literals.h"
// #include "adc.h"

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
      .set      (mcu::RCC::     PLLPdiv::_2)
      // .set_PLLQ<4>()
      .set      (mcu::RCC::   PLLsource::HSE)
      .on_PLL()
      .wait_PLL_ready();
}


int main()
{
   volatile decltype(auto) led = Pin::make<mcu::PD15, mcu::PinMode::Output>();
   // volatile decltype(auto) led1 = Pin::make<mcu::PA15, mcu::PinMode::Output>();

   Timer timer {500_ms};
   // Timer timer1 {100_ms};
   
   while(1) {
      led ^= timer.event();
      // led1 ^= timer1.event();
      __WFI();
   } // while(1) {

}