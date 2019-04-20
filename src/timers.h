#pragma once

#include <cstdint>
#include "subscriber.h"
#include "systick.h"
#ifdef USE_MOCK_SYSTICK
using mock::SysTick;
#else
using mcu::SysTick;
#endif





struct TickUpdater : Publisher
{
   TickUpdater() { REF(SysTick).initInterrupt<1000>(); }
} tickUpdater;

extern "C" void SysTick_Handler()
{
   tickUpdater.notify();
}

enum Faster {
    x2 = 2, x4 = 4, x5 = 5, x10 = 10,
};

class TickSubscriber : Subscriber
{
protected:
   bool subscribed {false};
   void tick_subscribe();
   void tick_unsubscribe();
   template<Faster x> void tick_subscribe();
public:
   
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

   void notify() override;
};




void Timer::notify()
{
   timePassed_++;
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
      tickUpdater.subscribe<x> (*this);
   }
}

