// #include <windows>
// const int NotUsed = system( "color 20" );
#define STM32F030x6
#define F_OSC   8000000UL
#define F_CPU   48000000UL
#include "periph_rcc.h"
// #include "flash.h"
// #include "timers.h"
// #include "periph_dma.h"
// #include "example/example_adc.h"
// #include "example/example_modbus_master.h"
#include "example_flash.h"
// #include "literals.h"
// #include "pwm_.h"
// #include "encoder.h"


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
   mcu::example::flash();
   // decltype(auto) encoder = Encoder::make<mcu::Periph::TIM1, mcu::PA8, mcu::PA9>();
   // int16_t v;
    // REF(RCC).clock_enable<mcu::Periph::TIM1>();
    // mcu::example::adc_average();
    // mcu::example::modbus_master();

   //  auto& pwm = PWM::make<mcu::Periph::TIM3, mcu::PC8>();
   // pwm.out_enable();
   // decltype(auto) pwm_ = PWM::make<mcu::Periph::TIM3, mcu::PC9>();
   // pwm.frequency = 26000;
   // pwm_.frequency = 26000;
   // pwm_.out_enable();


   // Timer timer {10};
   // Timer timer_ {20};
   // int i {0};
   // int p {0};

   // while(1) {
   //    v = encoder;
      // while (i < 100 ) {
      //    if (timer.event()) {
      //       pwm.duty_cycle = p++;
      //       pwm_.duty_cycle = i++;
      //       ++i;
      //       ++p;
      //    }
      // }
      // while (i > 0) {
      //    if (timer_.event()) {
      //       pwm.duty_cycle = p--;
      //       pwm_.duty_cycle = i--;
      //    }
      // }
   // } // while(1) {
}
