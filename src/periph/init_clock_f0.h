#include "periph_rcc.h"
#include "periph_flash.h"
#include "literals.h"

template<size_t f_ext, size_t f_cpu>
std::enable_if_t<f_ext == 8_MHz and f_cpu == 48_MHz>
init_clock()
{
   mcu::make_reference<mcu::Periph::RCC>()
      .set (mcu::RCC::AHBprescaler::AHBnotdiv)
      .set (mcu::RCC::APBprescaler::APBnotdiv)
      .set (mcu::RCC::SystemClock::CS_PLL)
      .on_HSE()
      .wait_HSE_ready()
      .set (mcu::RCC::PLLsource::HSE)
      .set (mcu::RCC::PLLmultiplier::_12)
      .on_PLL()
      .wait_PLL_ready();
}


template<size_t f_cpu>
std::enable_if_t<f_cpu == 48_MHz>
init_clock()
{
   mcu::make_reference<mcu::Periph::RCC>()
      .set (mcu::RCC::AHBprescaler::AHBnotdiv)
      .set (mcu::RCC::APBprescaler::APBnotdiv)
      .set (mcu::RCC::SystemClock::CS_PLL)
      .set (mcu::RCC::PLLsource::HSIdiv2)
      .set (mcu::RCC::PLLmultiplier::_12)
      .on_PLL()
      .wait_PLL_ready();
}

