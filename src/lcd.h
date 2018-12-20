#pragma once

#include "pin.h"
#include "delay.h"
#include "timers.h"

namespace mcu {

class LCD : TickSubscriber
{
   enum Init {reset = 0b0000, _4_bit_mode = 0x2, two_line_5x8_font = 0x8, display_on = 0xF, 
              display_clear = 0x8, set_mode = 0x6 };
   
   Pin& rs;
   Pin& rw;
   Pin& e;
   Pin& db4;
   Pin& db5;
   Pin& db6;
   Pin& db7;
   GPIO& port;

   LCD (Pin& rs, Pin& rw, Pin& e, Pin& db4, Pin& db5, Pin& db6, Pin& db7)
      : rs {rs}
      , rw {rw}
      , e  {e}
      , db4 {db4}
      , db5 {db5}
      , db6 {db6}
      , db7 {db7}
      , port {make_reference<Periph::GPIOB>()}
      {}

   void init();
   void send_data(Init data);

   void nibble()
   { 
      e = false;
      delay<500>();
      e = true;
      delay<500>();

      // while (not e) {e = true;}
      // e = false;
   }


   

public:

   template <class RS, class RW, class E, class DB4, class DB5, class DB6, class DB7>
   static auto& make()
   {
      auto& screen = *new LCD 
      {
         Pin::template make_new<RS,  PinMode::Output>(),
         Pin::template make_new<RW,  PinMode::Output>(), 
         Pin::template make_new<E,   PinMode::Output>(), 
         Pin::template make_new<DB4, PinMode::Output>(), 
         Pin::template make_new<DB5, PinMode::Output>(), 
         Pin::template make_new<DB6, PinMode::Output>(), 
         Pin::template make_new<DB7, PinMode::Output>(),
      };

      screen.init();

      return screen;
   }

   void send ();
   void notify() override {}

};

void LCD::init()
{
   delay<20000>();
   rs = false;
   rw = false;
   e = true;
   
   db4 = true;
   db5 = true;
   db6 = false;
   db7 = false;
   nibble();
   delay<5000>();

   db4 = true;
   db5 = true;
   db6 = false;
   db7 = false;
   nibble();
   delay<100>();

   db4 = true;
   db5 = true;
   db6 = false;
   db7 = false;
   nibble();
   delay<40>();




   db4 = false;
   db5 = true;
   db6 = false;
   db7 = false;
   nibble();
   delay<40>();


   // 4-bit
   db4 = false;
   db5 = true;
   db6 = false;
   db7 = false;
   nibble();
   delay<40>();
   db4 = false;
   db5 = false;
   db6 = false;
   db7 = true;
   nibble();
   delay<40>();

   // // D
   // db4 = false;
   // db5 = false;
   // db6 = false;
   // db7 = false;
   // nibble();
   // delay<40>();
   // db4 = false;
   // db5 = false;
   // db6 = true;
   // db7 = true;
   // nibble();
   // delay<40>();

   // // clear
   // db4 = false;
   // db5 = false;
   // db6 = false;
   // db7 = false;
   // nibble();
   // delay<40>();
   // db4 = 1;
   // db5 = false;
   // db6 = false;
   // db7 = false;
   // nibble();
   // delay<40>();

   // // mode
   // db4 = false;
   // db5 = false;
   // db6 = false;
   // db7 = false;
   // nibble();
   // delay<40>();
   // db4 = 0;
   // db5 = 1;
   // db6 = 1;
   // db7 = false;
   // nibble();
   // delay<40>();



   // shift cursor
   db4 = true;
   db5 = false;
   db6 = false;
   db7 = false;
   nibble();
   delay<40>();
   db4 = false;
   db5 = true;
   db6 = false;
   db7 = false;
   nibble();
   delay<40>();

   // display on
   db4 = false;
   db5 = false;
   db6 = false;
   db7 = false;
   nibble();
   delay<40>();
   db4 = 0;
   db5 = 0;
   db6 = 1;
   db7 = true;
   nibble();
   delay<50>();

   // dir shift
   db4 = false;
   db5 = false;
   db6 = false;
   db7 = false;
   nibble();
   delay<40>();
   db4 = false;
   db5 = true;
   db6 = true;
   db7 = false;
   nibble();
   delay<40>();
   
   // reset cursor 
   db4 = false;
   db5 = false;
   db6 = false;
   db7 = false;
   nibble();
   delay<40>();
   db4 = false;
   db5 = true;
   db6 = false;
   db7 = false;
   nibble();
   delay<40>();

   // clear display
   db4 = false;
   db5 = false;
   db6 = false;
   db7 = false;
   nibble();
   delay<40>();
   db4 = true;
   db5 = false;
   db6 = false;
   db7 = false;
   nibble();
   delay<40>();

   // db4 = false;
   // db5 = false;
   // db6 = false;
   // db7 = true;
   // nibble();
   // delay<40>();
   // db4 = false;
   // db5 = false;
   // db6 = false;
   // db7 = false;
   // nibble();
   // delay<40>();



}


















void LCD::send()
{
   rs = true;
   rw = false;
   db7 = false; db6 = true; db5 = false; db4 = false;
   nibble();
   delay<40>();
   db7 = true; db6 = false; db5 = false; db4 = false;
   nibble();
   delay<40>();

   rs = true;
   rw = false;
   db7 = false; db6 = true; db5 = true; db4 = false;
   nibble();
   delay<40>();
   db7 = false; db6 = true; db5 = false; db4 = true;
   nibble();
   delay<40>();

   rs = true;
   rw = false;
   db7 = false; db6 = true; db5 = true; db4 = false;
   nibble();
   delay<40>();
   db7 = true; db6 = true; db5 = false; db4 = false;
   nibble();
   delay<40>();

   rs = true;
   rw = false;
   db7 = false; db6 = true; db5 = true; db4 = false;
   nibble();
   delay<40>();
   db7 = true; db6 = true; db5 = false; db4 = false;
   nibble();
   delay<40>();

   rs = true;
   rw = false;
   db7 = false; db6 = true; db5 = true; db4 = false;
   nibble();
   delay<40>();
   db7 = true; db6 = true; db5 = true; db4 = true;
   nibble();
   delay<40>();

   rs = true;
   rw = false;
   db7 = false; db6 = false; db5 = true; db4 = false;
   nibble();
   delay<40>();
   db7 = false; db6 = false; db5 = false; db4 = false;
   nibble();
   delay<40>();

   rs = true;
   rw = false;
   db7 = true; db6 = false; db5 = true; db4 = false;
   nibble();
   delay<40>();
   db7 = true; db6 = false; db5 = false; db4 = false;
   nibble();
   delay<40>();

   rs = true;
   rw = false;
   db7 = false; db6 = true; db5 = true; db4 = true;
   nibble();
   delay<40>();
   db7 = false; db6 = false; db5 = false; db4 = false;
   nibble();
   delay<40>();

   rs = true;
   rw = false;
   db7 = true; db6 = false; db5 = true; db4 = true;
   nibble();
   delay<40>();
   db7 = true; db6 = false; db5 = false; db4 = false;
   nibble();
   delay<40>();

   rs = true;
   rw = false;
   db7 = true; db6 = false; db5 = true; db4 = true;
   nibble();
   delay<40>();
   db7 = false; db6 = false; db5 = 1; db4 = 1;
   nibble();
   delay<40>();

   rs = true;
   rw = false;
   db7 = 0; db6 = 1; db5 = true; db4 = 0;
   nibble();
   delay<40>();
   db7 = false; db6 = 1; db5 = 0; db4 = 1;
   nibble();
   delay<40>();

   rs = true;
   rw = false;
   db7 = 1; db6 = 0; db5 = true; db4 = 1;
   nibble();
   delay<40>();
   db7 = 1; db6 = 1; db5 = 1; db4 = 1;
   nibble();
   delay<40>();

   rs = true;
   rw = false;
   db7 = 0; db6 = 0; db5 = true; db4 = 0;
   nibble();
   delay<40>();
   db7 = 0; db6 = 0; db5 = 0; db4 = 1;
   nibble();
   delay<40>();
}



} // namespace mcu



// class Sparse_port {

// public:
//    void write (uint16_t data)
//    {
//       auto tmp {mask};
//       size_t index {0};
//       uint16_t v {0};
//       while (tmp) {
//          index = __builtin_ffs(tmp);
//          tmp &= ~(1 << (index-1));
//          v |= (1 << (index-1));
//       }

//       gpio.set (mask, v);
//    }
//    uint16_t mask;

//    GPIO& gpio;

//    templae<class ... Pins>
//    static auto& make()
//    {
//       Sparse_port port{};
//       port.mask = (1 << Pins::n | ...);
//    }


// };

//   5 4 1 0
// 2 0 0 1 0
// BSRR 5+16 1


// uint16_t write (uint16_t data)
// {
//     auto tmp {11000};
//     size_t index {0};
//     size_t i {0};
//     uint16_t v {0};
//     std::bitset<16> d {data};
//     while (tmp) {
//         index = __builtin_ffs(tmp);
//         tmp &= ~(1 << (index-1));
//         if (d[i++])
//             v |= (1 << (index-1));
//     }
//     return v;
// }