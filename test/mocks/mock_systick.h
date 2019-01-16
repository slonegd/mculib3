#pragma once

#define USE_MOCK_SYSTICK
#include "systick.h"
#include <iostream>
#include <thread>

namespace mock {

extern "C" void SysTick_Handler();

class SysTick : public mcu::SysTick
{
   std::ostream* process {nullptr};
   SysTick() = default;
   volatile size_t cnt {0};
public:
   
   static SysTick& make()
   {
      static SysTick systick;
      return systick;
   }
   bool interrupt {false};
   void set_stream (std::ostream& s) { process = &s; }
   friend std::ostream& operator<< (std::ostream&, const SysTick&);

   // void load (uint32_t v)   {if (process) *process << "SysTick reload value = " << v;}
   // void clear()             {if (process) *process << "Clear value";}
   // void setSource (Clock v) {if (process) *process << (v == Clock::external) ? "Selected external clock source" : "Selected processor clock source";}
   // void enableInterrupt()   {if (process) *process <<  "SysTick enable interrupt";}
   // void enable()            {if (process) *process << "SysTick enable";}
   // uint32_t get()           {if (process) *process << "SysTick current value = " << VAL; return VAL;}

   template <uint16_t ms> 
   void initInterrupt()
   {
      static_cast<mcu::SysTick*>(this)->initInterrupt<ms>();
      VAL = LOAD;
   }
   // template <class function>
   void tick() {
      using namespace std::chrono_literals;
      
      // while (1) {  
      //    volatile size_t delay {100};
      //    while (--delay) {}
         --VAL; ++cnt;
         if (VAL == 0) { 
            VAL = LOAD;
            SysTick_Handler();
            // std::thread (SysTick_Handler).detach();
            // std::this_thread::sleep_for(std::chrono::milliseconds(10));
         } 
      // }
   }

   auto now() { return cnt; }
   auto  nanoseconds_from (size_t begin) { return (cnt - begin) * 1'000'000'000 / F_CPU; }
   auto microseconds_from (size_t begin) { return  nanoseconds_from(begin)/1000; }
   auto milliseconds_from (size_t begin) { return microseconds_from(begin)/1000; }
   auto      seconds_from (size_t begin) { return milliseconds_from(begin)/1000; }
};


} //namespace mock

namespace mcu {
SFINAE(SysTick, mock::SysTick) make_reference() { return mock::SysTick::make();}
} //namespace mcu