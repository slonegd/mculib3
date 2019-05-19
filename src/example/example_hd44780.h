#pragma once
#include "hd44780.h"
#include "string_buffer.h"

// dont forget -fexec-charset=cp1251 с русским языком

namespace EO81 {
    using E   = mcu::PB3;       
    using RW  = mcu::PD2;       
    using RS  = mcu::PC12;      
    using DB4 = mcu::PB4;       
    using DB5 = mcu::PB5;
    using DB6 = mcu::PB6;    
    using DB7 = mcu::PB7;
}

namespace mcu::example {

template<class RS, class RW, class E, class DB4, class DB5, class DB6, class DB7>
struct hd44780 : TickSubscriber {

    String_buffer lcd{};
    int tick_cnt{0};
    Timer timer{100_s};

    hd44780() {
        // FIX
        HD44780::make<RS, RW, E, DB4, DB5, DB6, DB7>(lcd.get_buffer());
        lcd.center() << "Привет";
        lcd.line(2) << "счет: " << next_line;
        tick_subscribe();
    }
    
    void notify() override
    {
        if (not(++tick_cnt % 50)) {
            lcd.line(2).cursor(6).width(2) << timer.timePassed()/1000;
        }
    }
};

} // namespace mcu::example {