#pragma once

#include "pin.h"
#include "meta.h"
#include "delay.h"
#include "timers.h"

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

constexpr size_t symbol_n_function (size_t i)
{
    return i < 20 ? i      :
           i < 40 ? i + 20 :
           i < 60 ? i - 20 : i;
}

class Symbol_n {
   static constexpr auto table = meta::generate<symbol_n_function,80>;
    size_t index {0};
public:
   size_t operator++(int) { return table[index++ % 80]; }
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

template<class DB4, class DB5, class DB6, class DB7>
static constexpr std::pair<uint32_t,uint32_t> BSRR_command (char data)
{
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

class HD44780 : TickSubscriber
{
   using BSRR = std::pair<uint32_t, uint32_t>;

   char symbol;
   
   enum Set {_4_bit_mode   = 0x28, display_on  = 0x0C, dir_shift_right = 0x06,
             display_clear = 0x01, set_to_zero = 0x80 };

   enum Step {_1, _2, _3} step {Step::_1};

   Pin& rs;
   Pin& rw;
   Pin& e;
   GPIO& port;
   Symbol_n index{};
   const std::array<char, 80 >& buffer;
   const std::array<BSRR, 256>& chars;
   const std::array<BSRR, 256>& command;

   HD44780(Pin& rs, Pin& rw, Pin& e, GPIO& port
         , const std::array<char, 80 >& buffer
         , const std::array<BSRR, 256>& chars
         , const std::array<BSRR, 256>& command) : rs {rs}
                                                 , rw {rw}
                                                 , e  {e}
                                                 , port  {port}
                                                 , buffer{buffer}
                                                 , chars {chars}
                                                 , command {command}
   {}

   void init();

   void strob_e()
   { 
      e = false;
      delay<100>();
      e = true;
      delay<100>();
   }

   void instruction (uint32_t action)
   {
      rs = false;
      port.atomic_write(command[action].first);
      strob_e();
      port.atomic_write(command[action].second);
      strob_e();
      delay<50>();
   }


public:

   template <class RS, class RW, class E, class DB4, class DB5, class DB6, class DB7>
   static auto& make(const std::array<char, 80>& buffer)
   {
      static_assert ((all_is_same(DB4::periph, DB5::periph, DB6::periph, DB7::periph)),
                     "Пины для шины экрана должны быть на одном порту");

      static auto screen = HD44780 
      {
         Pin::template make<RS,  PinMode::Output>(),
         Pin::template make<RW,  PinMode::Output>(),
         Pin::template make<E,   PinMode::Output>(),
         make_reference<DB4::periph>(),
         buffer,
         meta::generate<BSRR_value<DB4, DB5, DB6, DB7>, 256>,
         meta::generate<BSRR_command<DB4, DB5, DB6, DB7>, 256>
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
};


void HD44780::init()
{
   delay<20000>();
   rs = false;
   rw = false;
   e = true;
   
   port.atomic_write(command[0x03].second);
   strob_e();
   delay<5000>();
   port.atomic_write(command[0x03].second);
   strob_e();
   delay<100>();
   port.atomic_write(command[0x03].second);
   strob_e();
   delay<50>();
   port.atomic_write(command[0x02].second);
   strob_e();
   delay<50>();
   instruction (_4_bit_mode);
   instruction (display_on);
   instruction (dir_shift_right);
   instruction (display_clear);
   instruction (set_to_zero);
}

void HD44780::notify()
{
   switch (step) {
      case _1:
         rw = false;
         rs = true;
         symbol = buffer[index++];
         port.atomic_write(chars[symbol].first);
         step = Step::_2;
      break;
      case _2:
         e = true;
         e = false;
         port.atomic_write(chars[symbol].second);
         step = Step::_3;
      break;
      case _3:
         e = true;
         e = false;
         step = Step::_1;
      break;
   }
}