#pragma once

#include "timers.h"
#include "literals.h"
#if not defined (TEST)
    #include "pin.h"
#endif
#include <tuple>
#include <algorithm>

namespace mcu {

class Button : TickSubscriber
{
    Pin& pin;

    int time {0};
    bool short_press {false};
    bool long_press {false};
    bool increment  {false};
    bool inverted   {false};
    bool click_     {false};
    int  inc {1};
    int  value{0};
    int  tmp{0};

    Button (Pin& pin, bool inverted) : pin {pin}, inverted {inverted} {}

public:

    template<class Pin_, bool inverted = false> 
    static auto& make()
    {
        static auto button = Button {Pin::template make<Pin_, PinMode::Input>(), inverted};
        return button;
    }

    bool push()
    {
        if (inverted xor pin)
            tick_subscribe();

        return time >= 10 and not short_press ? (short_press = true) : false;
    }

    bool push_long()
    {
        if (inverted xor pin)
            tick_subscribe();

        return time >= 1000 and not long_press ? (long_press = true) : false;
    }

    // bool click()
    // {
    //    if (inverted) {
    //       if (not pin) 
    //          tick_subscribe();
    //    } else {
    //       if (pin)
    //          tick_subscribe();
    //    }
    //    if (time > 10 and time < 1000)
    //       click_ = true;
    //    return not pin and click_ ? (not (click_ = false)) : false;
    // }

    operator int()
    {
        if (increment) {
            if (time <= 2000)
                value = (time >= tmp + 200) ? tmp = time, inc : 0;
            else if (time < 3000)
                value = (time >= tmp + 100) ? tmp = time, inc : 0;
            else if (time < 4000)
                value = (time >= tmp + 50 ) ? tmp = time, inc : 0;
            else if (time < 5000)
                value = (time >= tmp + 25 ) ? tmp = time, inc : 0;
            else if (time >= 5000) {
                value = (time >= tmp + 25 ) ? tmp = time, inc : 0;
                inc += (time % 1000) == 0;
            }
            
        } else if (push()) 
            value = 1;
         else if (push_long()) {
            increment = true;
            tmp = time;
            value = 1;
        } else 
            value = 0;

        return value;
    }

    void notify () override
    {
        if (inverted xor pin) {
            time++;
        } else {
            tick_unsubscribe();
            time = 0;
            short_press = false;
            long_press = false;
            increment  = false;
            inc        = 1;
        }
    }

    bool is_push() { return inverted xor pin; }

    struct Proxy_buttons 
    {
        Button& one;
        Button& two;

        Proxy_buttons(Button& one, Button& two) : one{one}, two{two} {}

        operator bool() {
            if (one.is_push()) one.tick_subscribe(); 
            if (two.is_push()) two.tick_subscribe();
            auto result =
                    one.time >= 10
                and two.time >= 10
                and (not one.short_press
                  or not two.short_press
                );
            if (result)
                one.short_press = two.short_press = true;
            return result;
        }                  

        bool click()
        {
            if (one.is_push() and two.is_push()) {
                one.tick_subscribe();
                two.tick_subscribe(); 
            }
            bool result{false};
            if (one.time > 100 and one.time < 200 and two.time > 100 and two.time < 200)
                one.click_ = two.click_ = true;
            result = not one.is_push() and not two.is_push() and (one.click_ or two.click_) and not  one.long_press and not two.long_press;
            if (result)
                one.click_ = two.click_ = false;
            return result;
        }                          

        bool push_long() {
            if (one.is_push()) one.tick_subscribe(); 
            if (two.is_push()) two.tick_subscribe();
            auto result =
                    one.time >= 1000
                and two.time >= 1000
                and (not one.long_press
                  or not two.long_press
                );
            if (result)
                one.long_press = two.long_press = true;
            return result;
        }
    };

    Proxy_buttons operator and (Button& other) {return Proxy_buttons{*this, other};}
    friend Proxy_buttons;

};

} //namespace mcu

