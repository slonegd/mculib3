#pragma once

#include <bitset>
#include "pin.h"
#include "delay.h"
#include "meta.h"
#include "timers.h"
#include "buttons.h"

namespace mcu {

struct bit_set {
   size_t value;
   constexpr bit_set (size_t value) : value{value} {}
   constexpr bool operator[] (size_t bit)
   {
      return value & (1 << bit); 
   }
   constexpr void set (size_t bit)
   {
      value |= (1 << bit);
   }
};

template<class DB4, class DB5, class DB6, class DB7>
static constexpr std::pair<uint32_t,uint32_t> BSRR_value (char data)
{
   bit_set d {data};
   bit_set bsrr_high {0};
   bit_set bsrr_low  {0};

   bsrr_low.set  (d[0] ? DB4::n : DB4::n+16);
   bsrr_low.set  (d[1] ? DB5::n : DB5::n+16);
   bsrr_low.set  (d[2] ? DB6::n : DB6::n+16);
   bsrr_low.set  (d[3] ? DB7::n : DB7::n+16);
   bsrr_high.set (d[4] ? DB4::n : DB4::n+16);
   bsrr_high.set (d[5] ? DB5::n : DB5::n+16);
   bsrr_high.set (d[6] ? DB6::n : DB6::n+16);
   bsrr_high.set (d[7] ? DB7::n : DB7::n+16);

   return std::pair<uint32_t, uint32_t>{bsrr_high.value, bsrr_low.value};
}


class LCD : TickSubscriber
{
   using BSRR = std::pair<uint32_t, uint32_t>;
   
   size_t index {0};
   bool is_data {false};
   size_t position {0};
   size_t line{1};
   
   static constexpr unsigned char  convert_HD44780[64] =
   {
   	0x41,0xA0,0x42,0xA1,0xE0,0x45,0xA3,0xA4,
   	0xA5,0xA6,0x4B,0xA7,0x4D,0x48,0x4F,0xA8,
   	0x50,0x43,0x54,0xA9,0xAA,0x58,0xE1,0xAB,
   	0xAC,0xE2,0xAD,0xAE,0xAD,0xAF,0xB0,0xB1,
   	0x61,0xB2,0xB3,0xB4,0xE3,0x65,0xB6,0xB7,
   	0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0x6F,0xBE,
   	0x70,0x63,0xBF,0x79,0xE4,0x78,0xE5,0xC0,
   	0xC1,0xE6,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7
   };
   
   std::array<uint8_t, 80> string_1;
   // std::array<uint8_t, 20> string_2;
   // std::array<uint8_t, 20> string_3;
   // std::array<uint8_t, 20> string_4;

   enum Set {_4_bit_mode = 0x28, shift_cursor_right = 0x14, 
             display_on  = 0x0C, dir_shift_right = 0x06,
             cursor_zero = 0x02, display_clear = 0x01, set_to_zero = 0x80 };

   enum Step {_1, _2, _3, _4, _5, _6, _7, _8} step {Step::_1};
   
   Pin& rs;
   Pin& rw;
   Pin& e;
   const std::array<BSRR, 256> chars;
   GPIO& port;
   Button& button;

   LCD (Pin& rs, Pin& rw, Pin& e, const std::array<BSRR, 256> chars, GPIO& port, Button& button)
      : rs {rs}
      , rw {rw}
      , e  {e}
      , chars {chars}
      , port{port}
      , button {button}
      {string_1.fill(' ');
      // , string_2.fill(' '), string_3.fill(' '), string_4.fill(' ');
      }

   void init();

   void strob_e()
   { 
      e = false;
      delay<100>();
      e = true;
      delay<100>();
   }

   void high_nibble(char letter)
   {
      port.atomic_write(chars[letter].first);
   }

   void low_nibble(char letter)
   {
      port.atomic_write(chars[letter].second);
   }

   void instruction (uint16_t command)
   {
      rs = false;
      high_nibble(command);
      strob_e();
      // delay<50>();
      low_nibble(command);
      strob_e();
      delay<50>();
   }

public:

   template <class RS, class RW, class E, class DB4, class DB5, class DB6, class DB7, class But>
   static auto& make()
   {
      // static_assert
      
      static auto screen = LCD 
      {
         Pin::template make_new<RS,  PinMode::Output>(),
         Pin::template make_new<RW,  PinMode::Output>(), 
         Pin::template make_new<E,   PinMode::Output>(),
         Generate<decltype(BSRR_value<DB4, DB5, DB6, DB7>), &BSRR_value<DB4, DB5, DB6, DB7>, 256, BSRR>::table,
         make_reference<DB4::periph>(),
         Button::template make<But>(),
      };

      Pin::template make_new<DB4, PinMode::Output>();
      Pin::template make_new<DB5, PinMode::Output>();
      Pin::template make_new<DB6, PinMode::Output>();
      Pin::template make_new<DB7, PinMode::Output>();


      screen.init();
      screen.subscribe();

      return screen;
   }

   void notify() override;

   LCD& operator<< (std::string_view string)
   {

      if (position < string_1.size()) {
         if (string.length() > string_1.size()) {
            std::copy(string.begin(), string.begin() + string_1.size(), string_1.begin() + position);
            std::copy(string.begin() + string_1.size(), string.end(), string_1.begin() + position);
            position += string.length() - string_1.size();
         } else if (string.length() <= string_1.size()) {
            std::copy(string.begin(), string.end(), string_1.begin() + position);
            position += string.length();
         } 
      } else if (position > string_1.size() - 1) {
         position = 0;
         std::copy(string.begin(), string.end(), string_1.begin() + position);
      }

      return *this;
   }


};


void LCD::init()
{
   delay<20000>();
   rs = false;
   rw = false;
   e = true;
   
   low_nibble(0x03);
   strob_e();
   delay<5000>();
   low_nibble(0x03);
   strob_e();
   delay<100>();
   low_nibble(0x03);
   strob_e();
   delay<50>();
   low_nibble(0x02);
   strob_e();
   delay<50>();
   instruction (_4_bit_mode);
   // instruction (shift_cursor_right);
   instruction (display_on);
   instruction (dir_shift_right);
   // instruction (cursor_zero);
   instruction (display_clear);
   instruction (set_to_zero);
}

void LCD::notify()
{
   switch (step) {
      case _1:
            rs = true;
            rw = false;
            high_nibble(string_1[index]);
            e = true;
            step = Step::_2;
      break;
      case _2:
         e = false;
         low_nibble(string_1[index]);
         e = true;
         step = Step::_3;
      break;
      case _3:
         e = false;
         index++;
         if (index == 20)
            index = 40;
         else if (index == 60)
            index = 20;
         else if (index == 40)
            index = 60;
         if (index < 80)
            step = Step::_1;
         if (index >= 80) {
            index = 0;
            step = Step::_1;
         }
      break;
   }
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



