#pragma once

#include "pin.h"
#include "delay.h"

namespace mcu {

class LCD 
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
   Delay& delay;

   LCD (Pin& rs, Pin& rw, Pin& e, Pin& db4, Pin& db5, Pin& db6, Pin& db7)
      : rs {rs}
      , rw {rw}
      , e  {e}
      , db4 {db4}
      , db5 {db5}
      , db6 {db6}
      , db7 {db7}
      , delay{Delay::make<Periph::TIM4>()}
      {}

   void init();
   void send_data(Init data);

   void nibble() { 
      while (not e) {e = true;}
      e = false;
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

};

void LCD::init()
{
   // rs = false;
   // rw = false;
   // e = false;
   delay.us(20);
   // db4 = true;
   // db5 = true;
   // db6 = false;
   // db7 = false;
   // nibble();
   delay.us(5);
   // db4 = true;
   // db5 = true;
   // db6 = false;
   // db7 = false;
   // nibble();
   delay.us(1);
   // db4 = true;
   // db5 = true;
   // db6 = false;
   // db7 = false;
   // nibble();
   delay.us(1);
   // send_data(Init::_4_bit_mode);
   // nibble();
   delay.us(1);
   // send_data(Init::_4_bit_mode);
   // nibble();
   // send_data(Init::two_line_5x8_font);
   // nibble();
   delay.us(1);
   // send_data(Init::reset);
   // nibble();
   // send_data(Init::display_on);
   // nibble();
   delay.us(1);
   // send_data(Init::reset);
   // nibble();
   // send_data(Init::set_mode);
   // nibble();
   delay.us(1);
   // send_data(Init::reset);
   // nibble();
   // send_data(Init::display_clear);
   // nibble();
   delay.us(1);
   // send_data(Init::reset);
   // nibble();
   // send_data(Init::_4_bit_mode);
   // nibble();
   delay.us(1);
   // send_data(Init::reset);
   // nibble();
}

void LCD::send_data(Init data)
{
   if      (data == Init::reset)             {db4 = false; db5 = false; db6 = false; db7 = false;}
   else if (data == Init::_4_bit_mode)       {db5 = true;  db4 = false; db6 = false; db7 = false;}
   else if (data == Init::two_line_5x8_font) {db7 = true;  db4 = false; db5 = false; db6 = false;}
   else if (data == Init::display_on)        {db7 = true;  db4 = true;  db5 = true;  db6 = true; }
   else if (data == Init::display_clear)     {db4 = true;  db5 = false; db6 = false; db7 = false;}
   else if (data == Init::set_mode)          {db5 = true;  db6 = true;  db4 = false; db7 = false;}

}

void LCD::send()
{
   rs = true;
   rw = false;
   db7 = true; db4 = false; db6 = false; db5 = false;
   e = true;
   e = false;
}

} // namespace mcu



// class Sparse_port {
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


// }

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