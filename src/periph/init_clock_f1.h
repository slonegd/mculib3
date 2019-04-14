#include "periph_rcc.h"
#include "periph_flash.h"
#include "literals.h"

template<size_t f_ext, size_t f_cpu>
std::enable_if_t<f_ext == 8_MHz and f_cpu == 72_MHz>
init_clock()
{
    REF(FLASH)
        .enable()
        .set(mcu::FLASH::Latency::_48_72MHz);
    
    REF(RCC)
    // mcu::make_reference<mcu::Periph::RCC>()
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
