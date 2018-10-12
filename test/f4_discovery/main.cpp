#define STM32F405xx
#define F_OSC   8000000UL
#define F_CPU   168000000UL

#include "rcc.h"


/// эта функция вызываеться первой в startup файле
extern "C" void init_clock ()
{
   // FLASH::set (FLASH::Latency::_5);

   mcu::RCC::make_reference()
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
   mcu::RCC::make_reference().clock_enable<Periph::GPIOA>();
   
   while(1) {
      
   } // while(1) {

}