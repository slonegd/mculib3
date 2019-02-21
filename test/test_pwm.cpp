#define BOOST_TEST_MODULE test_pwm
#include <boost/test/unit_test.hpp>

#define F_CPU   72'000'000UL
#define STM32F405xx
#define TEST


#include <iostream>
#include <type_traits>
#include "mock_rcc.h"
#include "mock_gpio.h"
#include "mock_tim.h"
#include "pwm_.h"

auto& process = mock::Process::make();

BOOST_AUTO_TEST_SUITE (test_suite_main)

BOOST_AUTO_TEST_CASE (make)
{
   process.clear();

   decltype (auto) pwm = PWM::make<mcu::Periph::TIM3, mcu::PC9>();
   
   BOOST_CHECK_EQUAL ( process.str(),
      "включение тактирования GPIOC"                             "\n"
      "инициализация вывода 9 порта GPIOC в режиме Alternate 2"  "\n"
      "включение тактирования TIM3"                              "\n"
      "TIM3: Режим сравнения канала №4 ШИМ"                      "\n"
      "TIM3: Предзагрузка разрешена"                             "\n"
      "TIM3: Разрешение работы"                                  "\n"
      "TIM3: ARR = 65514"                                        "\n"
      "TIM3: Значение сравнения канала №4 = 0"                   "\n"
   );
}

BOOST_AUTO_TEST_CASE (frequency)
{
   decltype (auto) pwm = PWM::make<mcu::Periph::TIM3, mcu::PC9>();
   process.clear();

   pwm.frequency = 1099; // repeatedly
   
   BOOST_CHECK_EQUAL ( process.str(),"");

   process.clear();
   pwm.frequency = 26'000;
   
   BOOST_CHECK_EQUAL ( process.str(),
      "TIM3: ARR = 2769"                        "\n"
      "TIM3: Значение сравнения канала №4 = 0"  "\n"
   );

   process.clear();
   pwm.frequency += 230;
   BOOST_CHECK_EQUAL ( process.str(),
      "TIM3: ARR = 2744"                        "\n"
      "TIM3: Значение сравнения канала №4 = 0"  "\n"
   );

   process.clear();
   pwm.frequency += -570;
   BOOST_CHECK_EQUAL ( process.str(),
      "TIM3: ARR = 2805"                        "\n"
      "TIM3: Значение сравнения канала №4 = 0"  "\n"
   );

}

BOOST_AUTO_TEST_CASE (duty_cycle)
{
   decltype (auto) pwm = PWM::make<mcu::Periph::TIM3, mcu::PC9>();
   process.clear();

   pwm.frequency  = 26'000;
   pwm.duty_cycle = 50;

   BOOST_CHECK_EQUAL ( process.str(),
      "TIM3: ARR = 2769"                           "\n"
      "TIM3: Значение сравнения канала №4 = 0"     "\n"
      "TIM3: Значение сравнения канала №4 = 1384"  "\n"
   );

   process.clear();

   pwm.frequency = 22'000; //pwm.duty_cycle = 50;

   BOOST_CHECK_EQUAL ( process.str(),
      "TIM3: ARR = 3272"                           "\n"
      "TIM3: Значение сравнения канала №4 = 1636"  "\n" //pwm.duty_cycle = 50;
   );

   process.clear();
   pwm.duty_cycle = 75;

   BOOST_CHECK_EQUAL ( process.str(),
      "TIM3: Значение сравнения канала №4 = 2454"  "\n" //pwm.frequency = 22'000;
   );

   process.clear();
   pwm.duty_cycle += 5;

   BOOST_CHECK_EQUAL ( process.str(),
      "TIM3: Значение сравнения канала №4 = 2617"  "\n"
   );
}

BOOST_AUTO_TEST_CASE (out_enable_disable)
{
   decltype (auto) pwm = PWM::make<mcu::Periph::TIM3, mcu::PC9>();
   process.clear();
   pwm.out_enable();
   pwm.out_disable();
   BOOST_CHECK_EQUAL ( process.str(),
      "TIM3: Разрешена работа четвертого канала"  "\n"
      "TIM3: Работа четвертого канала запрещена"  "\n"
   );
}

BOOST_AUTO_TEST_SUITE_END()