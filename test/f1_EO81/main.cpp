#define STM32F103xB
#define F_OSC   8000000UL
#define F_CPU   72000000UL
// #include <iostream>
#include "init_clock.h"
// #include "example_hd44780.h"
#include "example_select_screen.h"
#include "buttons_new.h"

/// эта функция вызывается первой в startup файле
extern "C" void init_clock() { init_clock<F_OSC, F_CPU>(); }

int main()
{
    // // value dont need
    // auto _ = mcu::example::hd44780<
    //     EO81::RS , EO81::RW , EO81::E,
    //     EO81::DB4, EO81::DB5, EO81::DB6, EO81::DB7
    // >();

    // value dont need
    auto _ = mcu::example::select_screen<
        EO81::RS , EO81::RW , EO81::E,
        EO81::DB4, EO81::DB5, EO81::DB6, EO81::DB7,
        EO81::Up, EO81::Down
    >();

    volatile auto i {0};
    auto button = Button_new<EO81::Up>();
    button.set_down_callback([&]{ i++; });

    while(1){
        __WFI();
    }
}

                                                           