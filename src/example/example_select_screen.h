#pragma once
#include "hd44780.h"
// #include "string_buffer.h"
#include "timers.h"
#include "select_screen.h"
#include "button.h"

namespace EO81 {
    using E    = mcu::PB3;       
    using RW   = mcu::PD2;       
    using RS   = mcu::PC12;      
    using DB4  = mcu::PB4;       
    using DB5  = mcu::PB5;
    using DB6  = mcu::PB6;    
    using DB7  = mcu::PB7;
    using Up   = mcu::PB8;
    using Down = mcu::PB9;
}

namespace mcu::example {

template<class RS, class RW, class E, class DB4, class DB5, class DB6, class DB7, class Up, class Down>
struct select_screen : TickSubscriber {

    String_buffer lcd{};
    HD44780& _;
    Button<Up> up{};
    Button<Down> down{};
    Buttons<Up,false,Down,false> enter = Buttons<Up,false,Down,false>(up, down);
    Timer timer{50_ms};
    
    Select_screen<6> screen;

    select_screen()
        : lcd  {}
        , _    { HD44780::make<RS, RW, E, DB4, DB5, DB6, DB7>(lcd.get_buffer()) }
        , screen {
              lcd
            // TODO пришлосб обрамлять лямбдой, потому что мой Function не работает с методами
            // доработать Function, чтоб можно было писать
            // Up_publisher   {up.set_click_callback}
            , Up_publisher    { [this](auto c){   up.set_click_callback(c);}     }
            , Down_publisher  { [this](auto c){ down.set_click_callback(c);}     }
            , Enter_publisher { [this](auto c){enter.set_click_callback(c);}     }
            , Out_publisher   { [this](auto c){enter.set_long_push_callback(c);} }
            , Out_callback {       []{}}
            , Line {"Аварии"      ,[]{}}
            , Line {"Наработка"   ,[]{}}
            , Line {"Конфигурация",[]{}}
            , Line {"Лог работы"  ,[]{}}
            , Line {"Настройки"   ,[]{}}
            , Line {"Что-то ещё"  ,[]{}}
        }
    {
        screen.init();
        tick_subscribe();
    }

    void notify() override
    {
        if (timer.event())
            screen.draw();
    }

};


} // namespace mcu::example {