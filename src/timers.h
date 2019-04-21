#pragma once

#include <cstdint>
#include "subscriber.h"
#include "systick.h"
#include "literals.h"
#include "function.h"
#ifdef USE_MOCK_SYSTICK
using mock::SysTick;
#else
using mcu::SysTick;
#endif



enum Faster {
     x2 = 2, x4 = 4, x5 = 5, x8 = 8, x10 = 10,
};


struct TickUpdater : Publisher
{
    TickUpdater() { REF(SysTick).initInterrupt<1000_us>(); }
    template<Faster multiplier> // multiplier how many subticks in tick
    void subscribe_subtick (Subscriber& v);
    void interrupt();


private:
    struct Subtick : Publisher {} subtick {};
    size_t subtick_cnt {0};
    size_t subtick_qty {1};
} tickUpdater{};



extern "C" void SysTick_Handler()
{
    tickUpdater.interrupt();
}



class TickSubscriber : Subscriber
{
protected:
    bool subscribed {false};
    void tick_subscribe();
    void tick_unsubscribe();
    template<Faster multiplier> // multiplier how many subticks in tick
    void tick_subscribe();
};



class Timer : TickSubscriber
{
public:
    Timer() = default;
    Timer (uint32_t ms) { start(ms); }
    ~Timer () {tick_unsubscribe();}

    void     start   (uint32_t ms); /// запускает счёт с текущего значения счётчика, устанавливает время
    bool     event();   /// возвращает true, если таймер натикал и перезапускает его
    bool     done();    /// возвращает true, если таймер натикал и НЕ перезапускает его
    void     pause();   /// останавливает счёт, но не сбрасывает счётчик
    void     start();   /// запускает счёт с текущего значения счётчика
    void     stop();    /// останавливает счёт и сбрасывает счётчик
    bool     isGreater (uint32_t val); /// возвращает true, если счётчик натикал больше значения val
    bool     isCount(); /// возвращает true если считает, но не досчитал
    uint32_t timePassed(); /// возвращает сколько натикал
    uint32_t timeLeft();   /// возвращает сколько осталось
    template<class function>
    void     event (function); /// выполняет function, когда дотикал и перезапускает таймер
    volatile uint32_t timeSet {0};


private:
    volatile uint32_t timePassed_ {0};	

    void notify() override { timePassed_++; }
};








template<Faster multiplier> // how many subticks in tick
void TickUpdater::subscribe_subtick (Subscriber& v)
{
    subtick.subscribe(v);
    subtick_qty = multiplier;
    REF(SysTick).initInterrupt<1000_us/multiplier>();
}



template<class Function>
inline void every_qty_cnt_call(size_t& cnt, size_t qty, Function call)
{
    if (not(cnt++ % qty))
        call();
}



void TickUpdater::interrupt()
{
    subtick.notify();
    every_qty_cnt_call (subtick_cnt, subtick_qty, [this]{
        notify();
    });
}



void Timer::start (uint32_t ms)
{
    timeSet = ms;
    tick_subscribe();
}



bool Timer::event()
{
    if (timePassed_ >= timeSet) {
        timePassed_ = 0;
        return (true);
    } else {
        return (false);
    }
}



template<class Functor>
void Timer::event (Functor functor)
{
    if (timePassed_ >= timeSet) {
        timePassed_ = 0;
        functor();
    }
}



void Timer::stop()
{
    timePassed_ = 0;
    tick_unsubscribe();
}



bool     Timer::done()       { return timePassed_ >= timeSet; }
void     Timer::pause()      { tick_unsubscribe(); }
void     Timer::start()      { tick_subscribe();  }
bool     Timer::isCount()    { return subscribed; }
uint32_t Timer::timePassed() { return timePassed_; }
uint32_t Timer::timeLeft(  ) { return timeSet - timePassed_; }
bool     Timer::isGreater (uint32_t val) { return timePassed_ > val; }



void TickSubscriber::tick_unsubscribe()
{
    if (subscribed) {
        subscribed = false;
        tickUpdater.unsubscribe (*this);
    }
}



void TickSubscriber::tick_subscribe()
{
    if (not subscribed) {
        subscribed = true;
        tickUpdater.subscribe (*this);
    }
}



template<Faster x> // на сколько быстрее
void TickSubscriber::tick_subscribe()
{
    if (not subscribed) {
        subscribed = true;
        tickUpdater.subscribe_subtick<x> (*this);
    }
}

