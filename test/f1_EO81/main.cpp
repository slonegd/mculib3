#define STM32F103xB
#define F_OSC   8000000UL
#define F_CPU   72000000UL
// #include <iostream>
#include "rcc.h"
#include "periph_flash.h"
#include "pin.h"
// // #include "timers.h"
// // #include "pwm.h"
// #include "uart.h"
// #include "fifo.h"
// # include "modbus_slave.h"
// #include "modbus_master.h"
#include "buttons.h"


/// эта функция вызывается первой в startup файле
extern "C" void init_clock ()
{
   mcu::make_reference<mcu::Periph::FLASH>()
      .enable()
      .set(mcu::FLASH::Latency::_48_72MHz);
   

   mcu::make_reference<mcu::Periph::RCC>()
      .on_HSE()
      .wait_HSE_ready()
      .set      (mcu::RCC::AHBprescaler::AHBnotdiv)
      .set_APB1 (mcu::RCC::APBprescaler::APBdiv2)
      .set_APB2 (mcu::RCC::APBprescaler::APBnotdiv)
      .set      (mcu::RCC::PLLsource::HSE)
      .set      (mcu::RCC::PLLmultiplier::_9)
      .on_PLL()
      .wait_PLL_ready()
      .set      (mcu::RCC::SystemClock::CS_PLL);
}

  

int main()
{
   // auto button = mcu::Button::make<mcu::PB8>();

   auto led = mcu::Pin::make_new<mcu::PC9, mcu::PinMode::Output>();

   // if (button.push())
      led = true;
   

}
