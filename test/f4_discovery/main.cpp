#define STM32F405xx
#define F_OSC   8000000UL
#define F_CPU   168000000UL

#include "periph_rcc.h"
#include "periph_flash.h"
#include "pin.h"
#include "timers.h"
#include "literals.h"
#include "adc.h"
#include "delay.h"
#include "pwm_.h"
#include "button_old.h"
#include "encoder.h"
// #include "buttons.h"
// #include "spi.h"


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
      // .set_PLLQ<7>()
      .set      (mcu::RCC::   PLLsource::HSE)
      .on_PLL()
      .wait_PLL_ready();
}


int main()
{
   decltype(auto) encoder = Encoder::make<mcu::Periph::TIM8, mcu::PC6, mcu::PC7, true>();
   decltype(auto) pwm = PWM::make<mcu::Periph::TIM3, mcu::PC9>(490);
   // pwm.out_enable(); 
   pwm.duty_cycle = 400;
   // volatile decltype (auto) led_blue   = Pin::make<mcu::PD15, mcu::PinMode::Output>();
   // volatile decltype (auto) led_orange = Pin::make<mcu::PD13, mcu::PinMode::Output>();
   volatile decltype (auto) enter      = mcu::Button::make<mcu::PA8>(); 
   volatile decltype (auto) led_red    = Pin::make<mcu::PA15, mcu::PinMode::Output>();
   volatile decltype (auto) led_green  = Pin::make<mcu::PC10, mcu::PinMode::Output>();

   Timer timer{100};
   int16_t value;
   
   // constexpr auto conversion_on_channel {16};
   // constexpr auto _2V {2 * 16 * 4095/3.3}; 
   // auto step_pwm {10};
   //  struct {
   //      ADC_average& control     = ADC_average::make<mcu::Periph::ADC1>(conversion_on_channel);
   //      ADC_channel& voltage     = control.add_channel<mcu::PA2>();
   //  } adc{};

   // decltype(auto) pwm = PWM::make<mcu::Periph::TIM4, mcu::PD15>(999);
   // pwm.out_enable();

   //  adc.control.set_callback ([&]{
      //   pwm.duty_cycle = adc.voltage / 60;
      //   led_blue = adc.voltage < _2V;
   //  });
   //  adc.control.start();
   encoder = 18000;
   while(1){
      led_red = pwm ^= enter; 
      pwm.frequency = encoder;
      value = encoder;
      // led_green ^= timer.event();
      // led_red   ^= enter;
      led_green = value > 200 ? true : false;
      // pwm.duty_cycle += timer.event() ? step_pwm : 0;
      // step_pwm = (pwm.duty_cycle >= 990 or pwm.duty_cycle <= 10) ? -step_pwm : step_pwm;
      // value = pwm.duty_cycle;
      // pwm.duty_cycle = value >=  ? 0 : value;
      // if (pwm.duty_cycle >= 999)
      //    pwm.duty_cycle = 1;
   }
   
   // decltype(auto) spi = SPI_::make<mcu::Periph::SPI1, mcu::PA7, mcu::PA6, mcu::PA5, mcu::PA4, true>();
   // volatile decltype (auto) MOSI = Pin::make<mcu::PA7, mcu::PinMode::Alternate_5>();
   // volatile decltype (auto) MISO = Pin::make<mcu::PA6, mcu::PinMode::Alternate_5>();
   // volatile decltype (auto) CSK = Pin::make<mcu::PA5, mcu::PinMode::Alternate_5>();
   
   // auto &rcc = REF(RCC);
   // rcc.clock_enable<mcu::Periph::SPI1>();
   // auto &spi = REF(SPI1);
   // spi.set_mode(true)
   //    .set(mcu::SPI::Data_size::_8bits)
   //    .NSS_soft()
   //    .NSS_high()
   //    .set(SPI::Prescaler::div32)
   //    .enable();

   // while (1) {
      
      // spi.send(0x4);
      // while (not spi.is_tx_complete()) {}
   // }

//    volatile decltype (auto) led_blue   = Pin::make<mcu::PD15, mcu::PinMode::Output>();
//    volatile decltype (auto) led_orange = Pin::make<mcu::PD13, mcu::PinMode::Output>();
//    volatile decltype (auto) led_red    = Pin::make<mcu::PD14, mcu::PinMode::Output>();
//    volatile decltype (auto) led_green  = Pin::make<mcu::PD12, mcu::PinMode::Output>();
// //    Timer timer{500};
   
//    constexpr auto conversion_on_channel {16};
//    constexpr auto _2V {2 * 16 * 4095/2.9}; 
//     struct {
//         ADC_average& control     = ADC_average::make<mcu::Periph::ADC1>(conversion_on_channel);
//         ADC_channel& voltage     = control.add_channel<mcu::PA2>();
//         // ADC_channel& temperature = control.add_channel<mcu::PA1>();
//     } adc{};

//     auto& alarm       = Pin::make<mcu::PB0,mcu::PinMode::Output>();

//     adc.control.set_callback ([&]{
//         // led = adc.voltage < _2V;
//     });
//     adc.control.start();

//     Delay delay{};
//     auto& button = Button::make<mcu::PA0>();
//     // int i {0};
    
//    decltype(auto) pwm_blue = PWM::make<mcu::Periph::TIM4, mcu::PD15>();
//    pwm_blue.out_enable();
//    decltype(auto) pwm_green = PWM::make<mcu::Periph::TIM4, mcu::PD12>();
//    pwm_blue.frequency = 26000;
//    pwm_green.frequency = 26000;
//    pwm_green.out_enable();

//    decltype(auto) pwm_red = PWM::make<mcu::Periph::TIM4, mcu::PD14>();
//    pwm_red.frequency = 26000;
//    pwm_red.out_enable();

//    decltype(auto) pwm_orange = PWM::make<mcu::Periph::TIM4, mcu::PD13>();
//    pwm_orange.frequency = 26000;
//    pwm_orange.out_enable();


//    Timer timer {10};
//    Timer timer_ {20};
//    int i {0};
//    int p {0};

//    while(1) {
//       while (i < 100 ) {
//          if (timer.event()) {
//             pwm_red.duty_cycle = pwm_orange.duty_cycle = p++;
//             pwm_green.duty_cycle = pwm_blue.duty_cycle = i++;
//             ++i;
//             ++p;
//          }
//       }
//       while (i > 0) {
//          if (timer_.event()) {
//             pwm_red.duty_cycle = pwm_orange.duty_cycle = p--;
//             pwm_green.duty_cycle = pwm_blue.duty_cycle = i--;
//          }
//       }
// } // while(1) {

   //  while (1) {

      //   while (delay.ms(20)) {led_blue = led_red = true; led_orange = led_green = false;}
      //   while (delay.ms(20)) {led_orange = led_red = true; led_blue = led_green = false;}
      //   while (delay.ms(20)) {led_orange = led_green = true; led_blue = led_red = false;}
      //   while (delay.ms(20)) {led_blue = led_green = true; led_orange = led_red = false;}
      //   if (button) {i += 10; if (i > 100) i = 0;}
      //   pwm_red.duty_cycle = pwm_orange.duty_cycle = i;
      //    pwm_green.duty_cycle = pwm_blue.duty_cycle = i;


   //  }

}