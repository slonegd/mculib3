#define BOOST_TEST_MODULE test_pwm
#include <boost/test/unit_test.hpp>

#define F_CPU  48'000'000UL
#define STM32F030x6
#define TEST


#include <iostream>
#include <type_traits>
#include "mock_rcc.h"
#include "mock_gpio.h"
#include "mock_tim.h"
#include "encoder.h"

auto& process = mock::Process::make();

BOOST_AUTO_TEST_SUITE (test_suite_main)

BOOST_AUTO_TEST_CASE (make)
{
   process.clear();

   decltype (auto) encoder = Encoder::make<mcu::Periph::TIM1, mcu::PA8, mcu::PA9>();
   
   BOOST_CHECK_EQUAL ( process.str(),
      "включение тактирования GPIOA"                             "\n"
      "инициализация вывода 8 порта GPIOA в режиме Alternate 2"  "\n"
      "включение тактирования GPIOA"                             "\n"
      "инициализация вывода 9 порта GPIOA в режиме Alternate 2"  "\n"
      "включение тактирования TIM1"                              "\n"
      "TIM1: Установлен режим слейва =  энкодера 3"              "\n"
      "TIM1: Режим сравнения канала №1 вход"                     "\n"
      "TIM1: Режим сравнения канала №2 вход"                     "\n"
      "TIM1: регистр CNT обнулен"                                "\n"
      "TIM1: Разрешение работы"                                  "\n"
   );
}

BOOST_AUTO_TEST_CASE (operator_equal)
{
   decltype (auto) encoder = Encoder::make<mcu::Periph::TIM1, mcu::PA8, mcu::PA9>();
   process.clear();
   encoder = 200;
   BOOST_CHECK_EQUAL ( process.str(),
      "TIM1: CNT = 200"                             "\n"
   );

   process.clear();
   encoder = -543;
   BOOST_CHECK_EQUAL ( process.str(),
      "TIM1: CNT = 64993"                             "\n" //65534 - 534
   );
}

BOOST_AUTO_TEST_CASE (operator_int16)
{
   decltype (auto) encoder = Encoder::make<mcu::Periph::TIM1, mcu::PA8, mcu::PA9>();
   encoder = 1345;
   int16_t v = encoder;
   BOOST_CHECK_EQUAL (v, 1345);

   encoder = -567;
   v = encoder;
   BOOST_CHECK_EQUAL (v, -567);
   
}

BOOST_AUTO_TEST_SUITE_END()