#define BOOST_TEST_MODULE test_hd44780
#include <boost/test/unit_test.hpp>
#include <thread>

#define TEST
#define F_CPU   72000000UL
#define STM32F103xB


#include "timeout.h"
#include "mock_systick.h"
#include "mock_rcc.h"
#include "mock_afio.h"
#include "mock_gpio.h"
#include "mock_timer.h"
#include "mock_delay.h"
#include "hd44780.h"
#include "process.h"

BOOST_AUTO_TEST_SUITE (test_suite_main)

using RS  = mcu::PC12;
using RW  = mcu::PD2;
using E   = mcu::PB3;
using DB4 = mcu::PB4;
using DB5 = mcu::PB5;
using DB6 = mcu::PB6;
using DB7 = mcu::PB7;

auto& process = mock::Process::make();
auto& port = mcu::make_reference<mcu::Periph::GPIOB>(); 

std::array<char, 80> buffer {'H', 'e', 'l', 'l', 'o'};
auto& e = Pin::make<E>();
auto& rs = Pin::make<RS>();
auto& rw = Pin::make<RW>();


BOOST_AUTO_TEST_CASE(make_init)
{
   process.clear();
   HD44780::make<RS, RW, E, DB4, DB5, DB6, DB7>(buffer);

   BOOST_CHECK_EQUAL (process.str(), 
      "инициализация вывода 3 порта GPIOB в режиме Output" "\n"
      "инициализация вывода 4 порта GPIOB в режиме Output" "\n"
      "инициализация вывода 5 порта GPIOB в режиме Output" "\n"
      "инициализация вывода 6 порта GPIOB в режиме Output" "\n"
      "инициализация вывода 7 порта GPIOB в режиме Output" "\n"
      "установка задержки 20 миллисекунд"                  "\n"
      "установка вывода 3 порта GPIOB"                     "\n"
      "GPIOB pins set: 4 5 pins clear: 6 7"                "\n"
      "сброс вывода 3 порта GPIOB"                         "\n"
      "установка вывода 3 порта GPIOB"                     "\n"
      "GPIOB pins set: 4 5 pins clear: 6 7"                "\n"
      "сброс вывода 3 порта GPIOB"                         "\n"
      "установка вывода 3 порта GPIOB"                     "\n"
      "GPIOB pins set: 4 5 pins clear: 6 7"                "\n"
      "сброс вывода 3 порта GPIOB"                         "\n"
      "установка вывода 3 порта GPIOB"                     "\n"
      "GPIOB pins set: 5 pins clear: 4 6 7"                "\n"
      "сброс вывода 3 порта GPIOB"                         "\n"
      "установка вывода 3 порта GPIOB"                     "\n"
      "GPIOB pins set: 5 pins clear: 4 6 7"                "\n" 
      "сброс вывода 3 порта GPIOB"                         "\n"
      "установка вывода 3 порта GPIOB"                     "\n"
      "GPIOB pins set: 7 pins clear: 4 5 6"                "\n"
      "сброс вывода 3 порта GPIOB"                         "\n"
      "установка вывода 3 порта GPIOB"                     "\n"
      "GPIOB pins set: pins clear: 4 5 6 7"                "\n"
      "сброс вывода 3 порта GPIOB"                         "\n"
      "установка вывода 3 порта GPIOB"                     "\n"
      "GPIOB pins set: 6 7 pins clear: 4 5"                "\n"
      "сброс вывода 3 порта GPIOB"                         "\n"
      "установка вывода 3 порта GPIOB"                     "\n"
      "GPIOB pins set: pins clear: 4 5 6 7"                "\n"
      "сброс вывода 3 порта GPIOB"                         "\n"
      "установка вывода 3 порта GPIOB"                     "\n"
      "GPIOB pins set: 5 6 pins clear: 4 7"                "\n"
      "сброс вывода 3 порта GPIOB"                         "\n"
      "установка вывода 3 порта GPIOB"                     "\n"
      "GPIOB pins set: pins clear: 4 5 6 7"                "\n"
      "сброс вывода 3 порта GPIOB"                         "\n"
      "установка вывода 3 порта GPIOB"                     "\n"
      "GPIOB pins set: 4 pins clear: 5 6 7"                "\n"
      "сброс вывода 3 порта GPIOB"                         "\n"
      "установка вывода 3 порта GPIOB"                     "\n"
      "GPIOB pins set: 7 pins clear: 4 5 6"                "\n"
      "сброс вывода 3 порта GPIOB"                         "\n"
      "установка вывода 3 порта GPIOB"                     "\n"
      "GPIOB pins set: pins clear: 4 5 6 7"                "\n"
      "сброс вывода 3 порта GPIOB"                         "\n"
      "установка вывода 3 порта GPIOB"                     "\n"
   );

  
}

BOOST_AUTO_TEST_SUITE_END()