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
        afio.set_external_interrupt<Pin_>();
        if constexpr (inverted)
            exti.set_trigger (EXTI::Edge::rising, Pin_::n);
        else
            exti.set_trigger (EXTI::Edge::falling, Pin_::n);
        exti.enable_interrupt (Pin_::n);

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
    AFIO& afio {REF(AFIO)};
    EXTI& exti {REF(EXTI)};
    size_t tick_cnt {0};
    bool down_executed     {false};
    bool long_push_executed {false};

    void notify() override { 
        tick_cnt++;
        
        if (is_unpush()) {
            if (down_executed)
                execute (up_callback);

            if (not long_push_executed)
                execute (click_callback);

            tick_unsubscribe();
            tick_cnt = 0;
            down_executed      = false;
            long_push_executed = false;
            return;
        }

        if (tick_cnt >= 10_ms and not down_executed) {
            down_executed = true;
            execute (down_callback);
            execute (increment_callback, 1); 
            return;
        }
        
        if (tick_cnt >= 1_s and not long_push_executed) {
            long_push_executed = true;
            execute (long_push_callback);
            return;
        }
        // TODO increment_callback
    }

    void interrupt() override { tick_subscribe(); }

    bool is_unpush() { return inverted ? pin.is_set() : not pin.is_set(); }
};

} //namespace mcu

