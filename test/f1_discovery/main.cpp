#define STM32F103xB
#define F_OSC   8000000UL
#define F_CPU   72000000UL
#include "rcc.h"
#include "pin.h"
// // #include "timers.h"
// // #include "pwm.h"
#include "uart.h"
// #include "fifo.h"
# include "modbus_slave.h"

/// эта функция вызываеться первой в startup файле
extern "C" void init_clock ()
{
   // FLASH::set (FLASH::Latency::_1);

//    mcu::make_reference<mcu::Periph::RCC>()
//       .set (mcu::RCC:: AHBprescaler::AHBnotdiv)
//       .set (mcu::RCC:: APBprescaler::APBnotdiv)
//       .set (mcu::RCC::  SystemClock::CS_PLL)
//       .set (mcu::RCC::    PLLsource::HSIdiv2)
//       .set (mcu::RCC::PLLmultiplier::_12)
//       .on_PLL()
//       .wait_PLL_ready();
}



int main()
{
//    auto pc8 = mcu::Pin::make<mcu::PC8, mcu::PinMode::Output>();
//    Timer timer {200};

//    auto pwm = mcu::PWM::make<mcu::Periph::TIM1, mcu::PA8>();
//    pwm.fill_ratio = 500_from1000;
//    pwm.frequency  = 20_kHz;
//    pwm.out_enable();

   auto uart = mcu::UART::make<mcu::Periph::USART1, mcu::PA9, mcu::PA10,mcu::PA8, mcu::PA12>();

//    while(1) {

//       if (timer.event()) {
//          pc8 ^= 1;
//       }

//    } // while(1) {
   // FIFO buffer;
   // buffer << 2;

   // buffer << 2;

   // struct InReg{};
   // struct OutReg{};

   // auto modbus = mcu::Modbus_slave::make<mcu::Periph::USART1, mcu::PA9, mcu::PA10, InReg, OutReg>();
}
