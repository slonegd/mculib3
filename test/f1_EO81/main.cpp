#define STM32F103xB
#define F_OSC   8000000UL
#define F_CPU   72000000UL
// #include <iostream>
#include "init_clock.h"
#include "example_hd44780.h"

/// эта функция вызывается первой в startup файле
extern "C" void init_clock() { init_clock<F_OSC, F_CPU>(); }

int main()
{
    // values dont need
    auto _ = mcu::example::hd44780<
        EO81::RS , EO81::RW , EO81::E,
        EO81::DB4, EO81::DB5, EO81::DB6, EO81::DB7
    >();

    while(1){
        __WFI();
    }
}
