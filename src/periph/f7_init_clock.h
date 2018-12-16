#include "rcc.h"
#include "literals.h"

// template<size_t f_ext, size_t f_cpu>
// void init_clock_() {};


template<size_t f_ext, size_t f_cpu>
std::enable_if_t<f_ext == 8_MHz and f_cpu == 216_MHz>
init_clock_()
{
   mcu::make_reference<mcu::Periph::RCC>()
      .on_HSE()
      .wait_HSE_ready()
      .set      (mcu::RCC::AHBprescaler::AHBnotdiv)
      .set_APB1 (mcu::RCC::APBprescaler::APBdiv2)
      .set_APB2 (mcu::RCC::APBprescaler::APBdiv4)
      .set      (mcu::RCC:: SystemClock::CS_PLL)
      .set_PLLM<4>()
      .set_PLLN<216>()
      .set      (mcu::RCC::PLLPdiv::PLLdiv2)
      .set      (mcu::RCC::PLLsource::HSE)
      .on_PLL()
      .wait_PLL_ready();
}
