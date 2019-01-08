#pragma once

#include "pin.h"
#include "delay.h"
#include "meta.h"
#include "timers.h"
#include "rus_string.h"

#if defined(USE_MOCK_GPIO)
using namespace mock;
#else
using namespace mcu;
#endif

template<class T, class ... Args>
constexpr bool all_is_same(T& t, Args& ... args)
{
    return ((t == args) and ...);
}

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
   data = data > 191 ? convert_HD44780[data - 192] : data;
   
   bit_set d {data};
   bit_set bsrr_high {0};
   bit_set bsrr_low  {0};

   bsrr_low. set (d[0] ? DB4::n : DB4::n+16);
   bsrr_low. set (d[1] ? DB5::n : DB5::n+16);
   bsrr_low. set (d[2] ? DB6::n : DB6::n+16);
   bsrr_low. set (d[3] ? DB7::n : DB7::n+16);
   bsrr_high.set (d[4] ? DB4::n : DB4::n+16);
   bsrr_high.set (d[5] ? DB5::n : DB5::n+16);
   bsrr_high.set (d[6] ? DB6::n : DB6::n+16);
   bsrr_high.set (d[7] ? DB7::n : DB7::n+16);

   return std::pair<uint32_t, uint32_t>{bsrr_high.value, bsrr_low.value};
}


class LCD : TickSubscriber
{
   using BSRR = std::pair<uint32_t, uint32_t>;
   
   size_t line     {0};
   size_t index    {0};
   size_t position {0};
   bool centre {false};
   
   std::array<char, 80> screen;

   enum Set {_4_bit_mode   = 0x28, display_on  = 0x0C, dir_shift_right = 0x06,
             display_clear = 0x01, set_to_zero = 0x80 };

   enum Step {_1, _2, _3} step {Step::_1};
   
   Pin& rs;
   Pin& rw;
   Pin& e;
   GPIO& port;
   const std::array<BSRR, 256> chars;
   static constexpr size_t screen_size = 80;

   LCD (Pin& rs, Pin& rw, Pin& e, GPIO& port, const std::array<BSRR, 256> chars)
      : rs {rs}
      , rw {rw}
      , e  {e}
      , port  {port}
      , chars {chars}
      {screen.fill(' ');}

   void init();

   void strob_e()
   { 
      e = false;
      delay<100>();
      e = true;
      delay<100>();
   }

   void instruction (uint32_t command)
   {
      rs = false;
      port.atomic_write(chars[command].first);
      strob_e();
      port.atomic_write(chars[command].second);
      strob_e();
      delay<50>();
   }

public:

   template <class RS, class RW, class E, class DB4, class DB5, class DB6, class DB7>
   static auto& make()
   {
      static_assert ((all_is_same(DB4::periph, DB5::periph, DB6::periph, DB7::periph)),
                     "Пины для шины экрана должны быть на одном порту");
      
      static auto screen = LCD 
      {
         Pin::template make<RS,  PinMode::Output>(),
         Pin::template make<RW,  PinMode::Output>(), 
         Pin::template make<E,   PinMode::Output>(),
         make_reference<DB4::periph>(),
         meta::generate<BSRR_value<DB4, DB5, DB6, DB7>, 256>
      };

      Pin::template make<DB4, PinMode::Output>();
      Pin::template make<DB5, PinMode::Output>();
      Pin::template make<DB6, PinMode::Output>();
      Pin::template make<DB7, PinMode::Output>();


      screen.init();
      screen.subscribe();

      return screen;
   }

   void notify() override;
   LCD& operator<< (std::string_view string);
   LCD& operator<< (size_t number);
   LCD& set_line   (size_t string);
   LCD& set_cursor (size_t cursor);
   LCD& central();
};


void LCD::init()
{
   delay<20000>();
   rs = false;
   rw = false;
   e = true;
   
   port.atomic_write(chars[0x03].second);
   strob_e();
   delay<5000>();
   port.atomic_write(chars[0x03].second);
   strob_e();
   delay<100>();
   port.atomic_write(chars[0x03].second);
   strob_e();
   delay<50>();
   port.atomic_write(chars[0x02].second);
   strob_e();
   delay<50>();
   instruction (_4_bit_mode);
   instruction (display_on);
   instruction (dir_shift_right);
   instruction (display_clear);
   instruction (set_to_zero);
}

void LCD::notify()
{
   switch (step) {
      case _1:
         // e = true;
         // e = false;
         rs = true;
         rw = false;
         port.atomic_write(chars[screen[index]].first);
         step = Step::_2;
      break;
      case _2:
         e = true;
         e = false;
         port.atomic_write(chars[screen[index]].second);
         step = Step::_3;
      break;
      case _3:
         e = true;
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



LCD& LCD::operator<< (std::string_view string)
{
   size_t rest_string = string.size();
      
   if (centre){
      if (rest_string < 20) {
         size_t pos = (20 - rest_string)/2;
         std::fill(screen.begin() + line*20, screen.begin() + line*20 + 20, ' ');
         std::copy(string.begin(), string.end(), screen.begin() + (line*20 + pos));
         centre = false;
      } else {
         std::copy(string.begin() + (rest_string - 20)/2, string.begin() + (rest_string - 20)/2 + 20, screen.begin() + line*20);
         centre = false;
      }
   } else {
      if (rest_string < screen_size - position) {
         std::copy(string.begin(), string.end(), screen.begin() + position);
         position += rest_string;
      } else {
         std::copy(string.begin(), string.begin() + (screen_size - position), screen.begin() + position);
         rest_string -= (screen_size - position);
         while (rest_string > screen_size) {
            auto string_begin = string.begin() + (string.size() - rest_string);
            auto string_end   = string_begin + screen_size;
            std::copy(string_begin, string_end, screen.begin());
            rest_string -= screen_size;
         }
         std::copy(string.begin() + (string.size() - rest_string), string.end(), screen.begin());
         position += rest_string;
      }
   }
   
   return *this;
}

LCD& LCD::operator<< (size_t number)
{
   char ch [10];
   snprintf(ch, sizeof(ch), "%d", number);
   // __itoa(number, ch, 10);
   // std::string_view s (ch);
   *this << ch;
   return *this;
}

LCD& LCD::central ()
{
   centre = true;
   return *this;
}

LCD& LCD::set_line(size_t string)
{
   line = string;
   position = line * 20;
   return *this;
}

LCD& LCD::set_cursor(size_t cursor)
{
   if      (line == 0) {
      if (cursor < 80)
         position = cursor;
      else position = cursor - 80 - (cursor/80) * 80;
   } else if (line == 1) {
      if (cursor < 60)
         position = cursor + 20;
      else position = cursor - 60 - (cursor/80) * 80;
   } else if (line == 2) {
      if (cursor < 40)
         position = cursor + 40;
      else position = cursor - 40 - (cursor/80) * 80;
   } else if (line == 3) {
      if (cursor < 20)
         position = cursor + 60;
      else position = cursor - 20 - (cursor/80) * 80;
   }
   
   // position = line == 1 ? cursor :
   //            line == 2 ? cursor + 20 :
   //            line == 3 ? cursor + 40 : cursor + 60;
   return *this;
}




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



