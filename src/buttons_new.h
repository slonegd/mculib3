#pragma once

#include "timers.h"
#include "literals.h"
#include "function.h"
#include "pin.h"
#include "interrupt.h"
#include "periph_exti.h"
#include "periph_rcc.h"


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

template<class Pin_, bool inverted = false>
class Button_new : Button_event, TickSubscriber, Interrupting {
public:
    void set_down_callback      (Callback<> v)    override { down_callback      = v; }
    void set_up_callback        (Callback<> v)    override { up_callback        = v; }
    void set_click_callback     (Callback<> v)    override { click_callback     = v; }
    void set_long_push_callback (Callback<> v)    override { long_push_callback = v; }
    void set_increment_callback (Callback<int> v) override { increment_callback = v; }

    Button_new() 
        : pin        {Pin::make<Pin_,mcu::PinMode::Input>()}
        , interrupt_ {get_external_interrupt(Pin_::n)}
    {
        REF(RCC).clock_enable<Periph::AFIO>();
        REF(AFIO).set_external_interrupt<Pin_>();
        if constexpr (inverted)
            REF(EXTI).set_trigger (EXTI::Edge::rising, Pin_::n);
        else
            REF(EXTI).set_trigger (EXTI::Edge::falling, Pin_::n);
        REF(EXTI).enable_interrupt (Pin_::n);

        interrupt_.subscribe(this);
        interrupt_.enable();
    }


private:
    Callback<>    down_callback;
    Callback<>    up_callback;
    Callback<>    click_callback;
    Callback<>    long_push_callback;
    Callback<int> increment_callback;
    Pin& pin;
    Interrupt& interrupt_;
    size_t tick_cnt {0};
    bool long_push_executed {false};

    void notify() override { 
        tick_cnt++;
        if (tick_cnt >= 1_s and not long_push_executed) {
            long_push_executed = true;
            execute (long_push_callback); 
        }
        // TODO increment_callback
    }

    void interrupt() override {
        if (is_downing()) {
            execute (down_callback);
            execute (increment_callback, 1);
            tick_subscribe();
            REF(EXTI).toggle_trigger (EXTI::Edge::both, Pin_::n);
            return;
        }
        execute (up_callback);
        execute (click_callback);
        tick_unsubscribe();
        REF(EXTI).toggle_trigger (EXTI::Edge::both, Pin_::n);
        tick_cnt = 0;
        long_push_executed = false;
    }

    bool is_downing() { return not subscribed; } // потому что подписываемся при нажатии

};

} //namespace mcu

