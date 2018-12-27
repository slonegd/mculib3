#include "literals.h"

BOOST_AUTO_TEST_SUITE (test_suite_main)

std::stringstream process{};
auto& rcc   = mcu::make_reference<mcu::Periph::RCC>();
auto& gpioa = mcu::make_reference<mcu::Periph::GPIOA>();
auto& gpiob = mcu::make_reference<mcu::Periph::GPIOB>();
auto& gpioc = mcu::make_reference<mcu::Periph::GPIOC>();

auto set_stream = [&](){
     rcc.set_stream (process);
   gpioa.set_stream (process);
   gpiob.set_stream (process);
   gpioc.set_stream (process);
};
auto clear_stream = [&](){ process.str(std::string{}); };

BOOST_AUTO_TEST_CASE (make)
{
   set_stream();

   Pin::make<mcu::PA0,mcu::PinMode::Output>();
   BOOST_CHECK_EQUAL (process.str(), 
      "включение тактирования GPIOA\n"
      "инициализация вывода 0 порта GPIOA в режиме Output\n"
   );
   clear_stream();

   Pin::make<mcu::PA0,mcu::PinMode::Input>();
   BOOST_CHECK_EQUAL (process.str(), 
      "включение тактирования GPIOA\n"
      "инициализация вывода 0 порта GPIOA в режиме Input\n"
   );
   clear_stream();

   Pin::make<mcu::PB1,mcu::PinMode::Alternate_1>();
   BOOST_CHECK_EQUAL (process.str(), 
      "включение тактирования GPIOB\n"
      "инициализация вывода 1 порта GPIOB в режиме Alternate 1\n"
   );
   clear_stream();

   Pin::make<mcu::PC15,mcu::PinMode::Alternate_2>();
   BOOST_CHECK_EQUAL (process.str(), 
      "включение тактирования GPIOC\n"
      "инициализация вывода 15 порта GPIOC в режиме Alternate 2\n"
   );
   clear_stream();

   Pin::make<mcu::PC0,mcu::PinMode::Alternate_7>();
   BOOST_CHECK_EQUAL (process.str(), 
      "включение тактирования GPIOC\n"
      "инициализация вывода 0 порта GPIOC в режиме Alternate 7\n"
   );
   clear_stream();
}

BOOST_AUTO_TEST_CASE (set)
{
   set_stream();
   auto& pa0 = Pin::make<mcu::PA0,mcu::PinMode::Output>();
   auto& pc9 = Pin::make<mcu::PC9,mcu::PinMode::Output>();
   clear_stream();

   pa0.set();
   BOOST_CHECK_EQUAL (process.str(), 
      "установка вывода 0 порта GPIOA\n"
   );
   clear_stream();

   pc9.set();
   BOOST_CHECK_EQUAL (process.str(), 
      "установка вывода 9 порта GPIOC\n"
   );
   clear_stream();
}

BOOST_AUTO_TEST_CASE (clear)
{
   set_stream();
   auto& pa1 = Pin::make<mcu::PA1,mcu::PinMode::Output>();
   auto& pb8 = Pin::make<mcu::PB8,mcu::PinMode::Output>();
   clear_stream();

   pa1.clear();
   BOOST_CHECK_EQUAL (process.str(), 
      "сброс вывода 1 порта GPIOA\n"
   );
   clear_stream();

   pb8.clear();
   BOOST_CHECK_EQUAL (process.str(), 
      "сброс вывода 8 порта GPIOB\n"
   );
   clear_stream();
}

BOOST_AUTO_TEST_CASE (is_set)
{
   set_stream();
   auto& pa2 = Pin::make<mcu::PA2,mcu::PinMode::Output>();
   auto& pc3 = Pin::make<mcu::PC3,mcu::PinMode::Output>();
   clear_stream();


   BOOST_CHECK_EQUAL (pa2.is_set(), false);
   BOOST_CHECK_EQUAL (pc3.is_set(), false);

   gpioa.mock.set (2, true);
   BOOST_CHECK_EQUAL (pa2.is_set(), true);
   BOOST_CHECK_EQUAL (pc3.is_set(), false);

   gpioc.mock.set (2, true);
   BOOST_CHECK_EQUAL (pa2.is_set(), true);
   BOOST_CHECK_EQUAL (pc3.is_set(), false);

   gpioc.mock.set (3, true);
   BOOST_CHECK_EQUAL (pa2.is_set(), true);
   BOOST_CHECK_EQUAL (pc3.is_set(), true);

   clear_stream();
}

BOOST_AUTO_TEST_CASE (toggle)
{
   set_stream();
   auto& pa3 = Pin::make<mcu::PA3,mcu::PinMode::Output>();
   clear_stream();

   pa3.toggle();
   BOOST_CHECK_EQUAL (process.str(), 
      "переключение вывода 3 порта GPIOA, а именно установка\n"
   );
   clear_stream();

   gpioa.mock.set (3, true);
   pa3.toggle();
   BOOST_CHECK_EQUAL (process.str(), 
      "переключение вывода 3 порта GPIOA, а именно сброс\n"
   );
   clear_stream();
}

BOOST_AUTO_TEST_CASE (simple_assignment_operator)
{
   set_stream();
   auto& pa0 = Pin::make<mcu::PA0,mcu::PinMode::Output>();
   clear_stream();

   auto b = pa0 = true;
   BOOST_CHECK_EQUAL (process.str(), 
      "установка вывода 0 порта GPIOA\n"
   );
   BOOST_CHECK_EQUAL (b, true);
   clear_stream();

   b = pa0 = false;
   BOOST_CHECK_EQUAL (process.str(), 
      "сброс вывода 0 порта GPIOA\n"
   );
   BOOST_CHECK_EQUAL (b, false);
   clear_stream();
}

BOOST_AUTO_TEST_CASE (bitwise_XOR_assignment_operator)
{
   set_stream();
   auto& pa0 = Pin::make<mcu::PA0,mcu::PinMode::Output>();
   clear_stream();

   auto b = pa0 ^= true;
   BOOST_CHECK_EQUAL (process.str(), 
      "переключение вывода 0 порта GPIOA, а именно установка\n"
   );
   BOOST_CHECK_EQUAL (b, true);
   clear_stream();

   gpioa.mock.set (0, true);
   b = pa0 ^= true;
   BOOST_CHECK_EQUAL (process.str(), 
      "переключение вывода 0 порта GPIOA, а именно сброс\n"
   );
   BOOST_CHECK_EQUAL (b, false);
   clear_stream();

   b = pa0 ^= false;
   BOOST_CHECK_EQUAL (process.str(), "");
   BOOST_CHECK_EQUAL (b, true);
   clear_stream();

   gpioa.mock.set (0, false);
   b = pa0 ^= false;
   BOOST_CHECK_EQUAL (process.str(), "");
   BOOST_CHECK_EQUAL (b, false);
   clear_stream();
}

BOOST_AUTO_TEST_CASE (bool_operator)
{
   auto& pb10 = Pin::make<mcu::PB10,mcu::PinMode::Input>();
   auto& pc11 = Pin::make<mcu::PC11,mcu::PinMode::Output>();

   BOOST_CHECK_EQUAL (bool(pb10), false);
   BOOST_CHECK_EQUAL (bool(pc11), false);

   gpiob.mock.set (10, true);
   BOOST_CHECK_EQUAL (bool(pb10), true);
   BOOST_CHECK_EQUAL (bool(pc11), false);

   gpioc.mock.set (11, true);
   BOOST_CHECK_EQUAL (bool(pb10), true);
   BOOST_CHECK_EQUAL (bool(pc11), true);
   clear_stream();
}


BOOST_AUTO_TEST_SUITE_END()
