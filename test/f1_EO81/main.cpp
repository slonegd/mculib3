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
// #include "buttons.h"
#include "lcd.h"


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
   
   auto& screen = mcu::LCD::make
   <mcu::PC12, mcu::PD2, mcu::PB3, mcu::PB4, mcu::PB5, mcu::PB6, mcu::PB7>();

}
