#pragma once

#include "timers.h"
#include "literals.h"
#include "function.h"
#include "pin.h"
#include "interrupt.h"
#include "periph_exti.h"


// суть
// класс работает от прерываний
// запоминает коллбеки на действия
// down, up, click, long_push, increment
// и вызывает эти коллбеки, когда действие наступило
// аналогично для двух кнопок одновременно

struct Button_event {
    virtual void set_down_callback      (Callback<>)    = 0;
    virtual void set_up_callback        (Callback<>)    = 0;
    virtual void set_click_callback     (Callback<>)    = 0;
    virtual void set_long_push_callback (Callback<>)    = 0;
    virtual void set_increment_callback (Callback<int>) = 0;
};


namespace mcu {

class Button_new : Button_event, TickSubscriber, Interrupting {
public:
    void set_down_callback      (Callback<> v)    override { down_callback      = v; }
    void set_up_callback        (Callback<> v)    override { up_callback        = v; }
    void set_click_callback     (Callback<> v)    override { click_callback     = v; }
    void set_long_push_callback (Callback<> v)    override { long_push_callback = v; }
    void set_increment_callback (Callback<int> v) override { increment_callback = v; }

    template<class Pin_, bool inverted = false>
    static Button_new& make() {
        static auto button = Button_new (Pin::make<Pin_,mcu::PinMode::Input>());
        // TODO настроить внешние прерывания
        // TODO разрешить прерывания
        return button;
    }

private:
    Callback<> down_callback;
    Callback<> up_callback;
    Callback<> click_callback;
    Callback<> long_push_callback;
    Callback<int> increment_callback;
    Pin& pin;
    size_t tick_cnt {0};
    bool long_push_executed {false};

    Button_new(Pin& pin) : pin {pin} {}

    void notify() override { 
        tick_cnt++;
        if (tick_cnt >= 1_s and not long_push_executed) {
            long_push_executed = true;
            long_push_callback(); 
        }
        // TODO increment_callback
    }
    
    void interrupt() override {
        if (is_down()) { // кнопку нажали TODO преварить коммент в функцию
            execute (down_callback);
            execute (increment_callback, 1);
            tick_subscribe();
            // TODO переписать прерывание на другой фронт
            return;
        }
        execute (up_callback);
        execute (click_callback);
        tick_unsubscribe();
        // TODO переписать прерывание на другой фронт
        tick_cnt = 0;
        long_push_executed = false;
    }

    bool is_down() { return not subscribed; } // потому что подписываемся при нажатии

};

} //namespace mcu

