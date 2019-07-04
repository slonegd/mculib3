// #include <windows>
// const int NotUsed = system( "color 20" );
#define STM32F030x6
#define F_OSC   8000000UL
#define F_CPU   48000000UL
#include "periph_rcc.h"
#include "flash.h"
#include "timers.h"
#include "periph_dma.h"
// #include "example/example_adc.h"
// #include "example/example_modbus_master.h"
#include "literals.h"
#include "pwm_.h"
#include "counter.h"
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
   
   Timer timer {200_ms};
   
   decltype(auto) counter = Counter::make<mcu::Periph::TIM1, mcu::PA9>(10);
   
   // decltype(auto) encoder = Encoder::make<mcu::Periph::TIM1, mcu::PA8, mcu::PA9>();
   uint16_t v{0};
   bool save{false};

    // REF(RCC).clock_enable<mcu::Periph::TIM1>();
    // mcu::example::adc_average();
    // mcu::example::modbus_master();

   // auto& pwm = PWM::make<mcu::Periph::TIM3, mcu::PB0>(1000);
   // pwm.frequency = 5_kHz;
   // pwm.duty_cycle = 500;
   // pwm.out_enable();


   
   // decltype(auto) pwm_ = PWM::make<mcu::Periph::TIM3, mcu::PB1>(1000);
   decltype (auto) led_blue = Pin::make<mcu::PC8, mcu::PinMode::Output>();
   // led_blue = true;
   // pwm.frequency = 26000;
   // pwm_.frequency = 26000;
   // pwm_.out_enable();


   // Timer timer {50};
   // Timer timer_ {50};
   // int i {0};
   // int p {0};
   // pwm.duty_cycle = 50;
   // pwm_.duty_cycle = 606;

   while(1) {

      led_blue ^= timer.event();
      // if (led_blue and not save) {
      //    v++;
      //    save = true;
      // }
      // else if (not led_blue)
      // {
      //    save = false;
      // }
      
      v = counter;
   //    // v = encoder;
   //    while (i < 1000 ) {
   //       if (timer.event()) {
   //          pwm.duty_cycle = p++;
   //          pwm_.duty_cycle = i++;
   //          ++i;
   //          ++p;
   //       }
   //    }
   //    while (i > 0) {
   //       if (timer_.event()) {
   //          pwm.duty_cycle = p--;
   //          pwm_.duty_cycle = i--;
   //          // i--;
   //       }
   //    }
   } // while(1) {
}
