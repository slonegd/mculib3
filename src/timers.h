#pragma once

#include <cstdint>
#include "subscriber.h"
#if not defined (TEST)
   #include "systick.h"
#endif


struct TickUpdater : Publisher
{
#if not defined(TEST) 
   TickUpdater() { mcu::make_reference<mcu::Periph::SysTick>().initInterrupt<1>(); }
#endif
   // using List::clear_subscribe;
} tickUpdater;

extern "C" void SysTick_Handler()
{
   tickUpdater.notify();
}


class TickSubscriber : Subscriber
{
protected:
   bool subscribed {false};
   void subscribe();
   void unsubscribe();
public:
   
};



class Timer : TickSubscriber
{
public:
   Timer() = default;
   Timer (uint32_t ms) { start(ms); }

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
   subscribe();
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
   unsubscribe();
}


bool     Timer::done()       { return timePassed_ >= timeSet; }
void     Timer::pause()      { unsubscribe(); }
void     Timer::start()      { subscribe();  }
bool     Timer::isCount()    { return subscribed; }
uint32_t Timer::timePassed() { return timePassed_; }
uint32_t Timer::timeLeft(  ) { return timeSet - timePassed_; }
bool     Timer::isGreater (uint32_t val) { return timePassed_ > val; }



void TickSubscriber::unsubscribe()
{
   if (subscribed) {
      subscribed = false;
      tickUpdater.unsubscribe (this);
   }
}


void TickSubscriber::subscribe()
{
   if (not subscribed) {
      subscribed = true;
      tickUpdater.subscribe (this);
   }
}