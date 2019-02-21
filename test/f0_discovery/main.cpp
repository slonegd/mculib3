// #include <windows>
// const int NotUsed = system( "color 20" );
#define STM32F030x6
#define F_OSC   8000000UL
#define F_CPU   48000000UL
#include "periph_rcc.h"
#include "flash.h"
// #include "pin.h"
#include "timers.h"
// #include "periph_dma.h"
#include "pwm_.h"


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
   decltype(auto) pwm = PWM::make<mcu::Periph::TIM3, mcu::PC8>();
   pwm.out_enable();
   decltype(auto) pwm_ = PWM::make<mcu::Periph::TIM3, mcu::PC9>();
   pwm.frequency = 26000;
   pwm_.frequency = 26000;
   pwm_.out_enable();
//    auto& dma = mcu::make_reference<mcu::Periph::DMA1>();
//    dma.clear_interrupt_flags(dma.Channel::_1);
//    dma.is_transfer_complete_interrupt (dma.Channel::_1);

//    auto& pc8 = Pin::make<mcu::PC8, mcu::PinMode::Output>();
   Timer timer {10};
   Timer timer_ {20};
   int i {0};
   int p {0};

//    pwm.duty_cycle(15);

   while(1) {
      while (i < 100 ) {
         if (timer.event()) {
            pwm.duty_cycle = p++;
            pwm_.duty_cycle = i++;
            ++i;
            ++p;
         }
      }
      while (i > 0) {
         if (timer_.event()) {
            pwm.duty_cycle = p--;
            pwm_.duty_cycle = i--;
         }
      }
   } // while(1) {

}
