#define STM32F030x6
#define F_OSC   8000000UL
#define F_CPU   48000000UL
#include "RCC.h"
#include "pin.h"
#include "timers.h"

/// эта функция вызываеться первой в startup файле
extern "C" void init_clock (void)
{
   // FLASH->ACR |= FLASH_ACR_PRFTBE;
//    FLASH::set (FLASH::Latency::_1);

   RCC::setAHBprescaler (RCC_ral::AHBprescaler::AHBnotdiv);
   RCC::setAPBprecsaler (RCC_ral::APBprescaler::APBnotdiv);
   RCC::systemClockSwitch (RCC_ral::SystemClockSwitch::CS_PLL);
   RCC::setPLLsource (RCC_ral::PLLsource::HSIdiv2);
   RCC::setPLLmultiplier (RCC_ral::PLLmultiplier::_12);
   RCC::PLLon();
   RCC::waitPLLready();
}


int main()
{
   CONFIGURE_PIN(PC8, Output);
   Timer timer {200};

   while(1) {

      if (timer.event())
         PC8::toggle();
      
   } // while(1) {

}