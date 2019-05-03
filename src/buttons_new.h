#pragma once

#include "timers.h"
#include "literals.h"
#include "function.h"
#include "pin.h"

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
    virtual void set_increment_callback (Callback<int>) = 0;
};


namespace mcu {

class Button_new : TickSubscriber, Button_event {
public:
    void set_down_callback      (Callback<> v)    override { down_callback      = v; }
    void set_up_callback        (Callback<> v)    override { up_callback        = v; }
    void set_click_callback     (Callback<> v)    override { click_callback     = v; }
    void set_increment_callback (Callback<int> v) override { increment_callback = v; }

    template<class Pin_, bool inverted = false>
    static Button_new& make() {
        static auto button = Button_new (Pin::make<Pin_,mcu::PinMode::Input>());
        return button;
    }

private:
    Callback<> down_callback;
    Callback<> up_callback;
    Callback<> click_callback;
    Callback<int> increment_callback;
    Pin& pin;

    Button_new(Pin& pin) : pin {pin} {}

    void notify() override {}

};

} //namespace mcu

