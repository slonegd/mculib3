#pragma once

#if defined (USE_MOCK_SYSTICK) or defined (USE_MOCK_TIMER)
#define NS mock
using namespace mock;
#else
#define NS
using namespace mcu;
#endif

class Delay {

   uint32_t begin {0};
   uint32_t rest  {0};
   bool first_time {false};

   NS::Timer timer {};

public:

   Delay() {first_time = true;}
   
   bool ms (uint32_t us)
   {
      timer.start(us);

      if (not timer.event()){ return true; } 
      else {
         timer.stop();
         return false;
      }
   }

   bool us (uint32_t us)
   {
      if (us >= 1000) {
         if (ms(us/1000))
            return false;
         else 
            return true;
      } else {
         uint32_t time = us * (F_CPU / 1'000'000);
         auto& tick = mcu::make_reference<mcu::Periph::SysTick>();
   
         if (first_time) { 
            begin = tick.get();
            if (time >= begin) {
               rest = time - begin;
            } 

            first_time = false; 
         } 

         if (begin >= time) {
            if (begin - tick.get() < time ) { 
               return true; 
            } else  {
               first_time = true;  
               return false;
            }
         } else {
            if (tick.get() < begin) {
               return true;
            } else {
               if (tick.get_load() - tick.get() < rest)
                  return true; 
               else {
                  first_time = true;
                  return false;
               } 
            }   
         }
         return true;
      }
   }
   


   

};
