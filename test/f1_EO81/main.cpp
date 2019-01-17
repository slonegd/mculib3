#define STM32F103xB
#define F_OSC   8000000UL
#define F_CPU   72000000UL
// #include <iostream>
#include "periph_rcc.h"
#include "periph_flash.h"
// #include "pin.h"
// // #include "timers.h"
// // #include "pwm.h"
// #include "uart.h"
// #include "fifo.h"
# include "modbus_slave.h"
// #include "modbus_master.h"
// #include "buttons.h"
// #include "lcd_ram.h"


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
   struct In_regs {
      uint16_t d1;
      int16_t  d2;
   };
   struct Out_regs {
      uint16_t d1;
      int16_t  d2;
   };
   auto& modbus = Modbus_slave<In_regs, Out_regs>::make<
      mcu::Periph::USART1,
      mcu::PA9,
      mcu::PA10,
      mcu::PA11,
      mcu::PA12
   >(1, UART::Settings{});

   modbus ([](auto){});
}
