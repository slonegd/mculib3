#define BOOST_TEST_MODULE test_button
#include <boost/test/unit_test.hpp>

#define F_CPU   72000000UL
#define STM32F103xB
#define TEST

#include "mock_pin.h"
#include "buttons.h"

BOOST_AUTO_TEST_SUITE (test_suite_main)

enum button {up_ = 0, down_ = 1};

template <class Function>
void process (int n, Function f)
{
   while (n--) {
      SysTick_Handler();
      f();
   }
}

auto passed_ms = [&](int n){while (n--) SysTick_Handler();};

auto& up   = mcu::Button::make<mcu::PA1>();
auto& down = mcu::Button::make<mcu::PA2, true>();

BOOST_AUTO_TEST_CASE(push)
{
   mcu::pins[button::up_]->set = true;
   process(10, [&](){
      BOOST_CHECK_EQUAL(up.push(), false);
   });
   passed_ms(1);
   BOOST_CHECK_EQUAL(up.push(), true); // return true only once
   BOOST_CHECK_EQUAL(up.push(), false);
   mcu::pins[button::up_]->set = false;
   passed_ms(1); // unsubscribe
}

BOOST_AUTO_TEST_CASE(push_long)
{
   mcu::pins[button::up_]->set = true;
   process(1000, [&](){
      BOOST_CHECK_EQUAL(up.push_long(), false);
   });
   passed_ms(1);
   BOOST_CHECK_EQUAL(up.push_long(), true); // return true only once
   BOOST_CHECK_EQUAL(up.push_long(), false);
   mcu::pins[button::up_]->set = false;
   passed_ms(1); // unsubscribe
}

BOOST_AUTO_TEST_CASE(increment)
{
   int var = 0;
   mcu::pins[button::up_]->set = true;
   process(10, [&](){
      var += up;
      BOOST_CHECK_EQUAL(var, 0);
   });
   process(1, [&](){
      var += up;
      BOOST_CHECK_EQUAL(var, 1);
   });

   process(989, [&](){
      var += up;
      BOOST_CHECK_EQUAL(var, 1);
   });
   process(1, [&](){
      var += up;
      BOOST_CHECK_EQUAL(var, 2);
   });

   process(200, [&](){var += up;});
   BOOST_CHECK_EQUAL(var, 3);

   process(200, [&](){var += up;});
   BOOST_CHECK_EQUAL(var, 4);

   process(600, [&](){var += up;});
   BOOST_CHECK_EQUAL(var, 7);

   process(100, [&](){var += up;});
   BOOST_CHECK_EQUAL(var, 8);

   process(200, [&](){var += up;});
   BOOST_CHECK_EQUAL(var, 10);

   process(700, [&](){var += up;});
   BOOST_CHECK_EQUAL(var, 17);

   process(50, [&](){var += up;});
   BOOST_CHECK_EQUAL(var, 18);

   process(950, [&](){var += up;});
   BOOST_CHECK_EQUAL(var, 37);

   process(25, [&](){var += up;});
   BOOST_CHECK_EQUAL(var, 38);

   process(975, [&](){var += up;});
   BOOST_CHECK_EQUAL(var, 77);

   process(25, [&](){var += up;});
   BOOST_CHECK_EQUAL(var, 79);

   process(25, [&](){var += up;});
   BOOST_CHECK_EQUAL(var, 81);

   process(25, [&](){var += up;});
   BOOST_CHECK_EQUAL(var, 83);

   process(25, [&](){var += up;});
   BOOST_CHECK_EQUAL(var, 85);

   process(900, [&](){var += up;});
   BOOST_CHECK_EQUAL(var, 157);

   process(1000, [&](){var += up;});
   BOOST_CHECK_EQUAL(var, 277);

   process(1000, [&](){var += up;});
   BOOST_CHECK_EQUAL(var, 437);


   mcu::pins[button::up_]->set = false;
   passed_ms(1); // unsubscribe
}

BOOST_AUTO_TEST_CASE(push_inverted)
{
   mcu::pins[button::down_]->set = false;
   process(10, [&](){
      BOOST_CHECK_EQUAL(down.push(), false);
   });
   passed_ms(1);
   BOOST_CHECK_EQUAL(down.push(), true); // return true only once
   BOOST_CHECK_EQUAL(down.push(), false);
   mcu::pins[button::down_]->set = true;
   passed_ms(1); // unsubscribe
}

BOOST_AUTO_TEST_CASE(push_long_inverted)
{
   mcu::pins[button::down_]->set = false;
   process(1000, [&](){
      BOOST_CHECK_EQUAL(down.push_long(), false);
   });
   passed_ms(1);
   BOOST_CHECK_EQUAL(down.push_long(), true); // return true only once
   BOOST_CHECK_EQUAL(down.push_long(), false);
   mcu::pins[button::down_]->set = true;
   passed_ms(1); // unsubscribe
}

BOOST_AUTO_TEST_CASE(decrement)
{
   int var = 437;

   mcu::pins[button::down_]->set = false;
   down.push();
   process(10, [&](){var -= down;});
   BOOST_CHECK_EQUAL(var, 436);

   process(1000, [&](){var -= down;});
   BOOST_CHECK_EQUAL(var, 435);

   process(200, [&](){var -= down;});
   BOOST_CHECK_EQUAL(var, 434);

   process(200, [&](){var -= down;});
   BOOST_CHECK_EQUAL(var, 433);

   process(600, [&](){var -= down;});
   BOOST_CHECK_EQUAL(var, 430);

   process(100, [&](){var -= down;});
   BOOST_CHECK_EQUAL(var, 429);

   process(200, [&](){var -= down;});
   BOOST_CHECK_EQUAL(var, 427);

   process(700, [&](){var -= down;});
   BOOST_CHECK_EQUAL(var, 420);

   process(50, [&](){var -= down;});
   BOOST_CHECK_EQUAL(var, 419);

   process(950, [&](){var -= down;});
   BOOST_CHECK_EQUAL(var, 400);

   process(25, [&](){var -= down;});
   BOOST_CHECK_EQUAL(var, 399);

   process(975, [&](){var -= down;});
   BOOST_CHECK_EQUAL(var, 360);

   process(25, [&](){var -= down;});
   BOOST_CHECK_EQUAL(var, 358);

   process(25, [&](){var -= down;});
   BOOST_CHECK_EQUAL(var, 356);

   process(25, [&](){var -= down;});
   BOOST_CHECK_EQUAL(var, 354);

   process(25, [&](){var -= down;});
   BOOST_CHECK_EQUAL(var, 352);

   process(900, [&](){var -= down;});
   BOOST_CHECK_EQUAL(var, 280);

   process(1000, [&](){var -= down;});
   BOOST_CHECK_EQUAL(var, 160);

   process(1000, [&](){var -= down;});
   BOOST_CHECK_EQUAL(var, 0);


   mcu::pins[button::down_]->set = true;
   passed_ms(1); // unsubscribe
}

BOOST_AUTO_TEST_CASE(both_together_push)
{
   mcu::pins[button::up_]->set = true;
   mcu::pins[button::down_]->set = false;
   process(10, [&](){
      BOOST_CHECK_EQUAL(bool (up and down), false);
   });
   passed_ms(1);
   BOOST_CHECK_EQUAL(bool (up and down), true);
   BOOST_CHECK_EQUAL(bool (up and down), false);

   mcu::pins[button::up_]->set = false;
   mcu::pins[button::down_]->set = true;
   passed_ms(1); // unsubscribe
}

BOOST_AUTO_TEST_CASE(both_together_push_long)
{
   mcu::pins[button::up_]->set = true;
   mcu::pins[button::down_]->set = false;
   process(1000, [&](){
      BOOST_CHECK_EQUAL(bool ((up and down).push_long()), false);
   });
   passed_ms(1);
   BOOST_CHECK_EQUAL(bool ((up and down).push_long()), true);

   mcu::pins[button::up_]->set = false;
   mcu::pins[button::down_]->set = true;
   passed_ms(1); // unsubscribe
}

BOOST_AUTO_TEST_CASE(both_push)
{
   mcu::pins[button::up_]->set = true;
   process(5, [&](){
      BOOST_CHECK_EQUAL(bool (up and down), false);
   });

   mcu::pins[button::down_]->set = false;
   process(5, [&](){
      BOOST_CHECK_EQUAL(bool (up and down), false);
   });
   passed_ms(1);
   BOOST_CHECK_EQUAL(bool (up and down), false);
   process(4, [&](){
      BOOST_CHECK_EQUAL(bool (up and down), false);
   });
   passed_ms(1);
   BOOST_CHECK_EQUAL(bool (up and down), true);
   BOOST_CHECK_EQUAL(bool (up and down), false);

   mcu::pins[button::up_]->set = false;
   mcu::pins[button::down_]->set = true;
   passed_ms(1); // unsubscribe
}

BOOST_AUTO_TEST_CASE(both_push_long)
{
   mcu::pins[button::up_]->set = true;
   process(100, [&](){
      BOOST_CHECK_EQUAL(bool ((up and down).push_long()), false);
   });
   
   mcu::pins[button::down_]->set = false;
   process(900, [&](){
      BOOST_CHECK_EQUAL(bool ((up and down).push_long()), false);
   });
   passed_ms(1);
   BOOST_CHECK_EQUAL(bool ((up and down).push_long()), false);
   process(99, [&](){
      BOOST_CHECK_EQUAL(bool ((up and down).push_long()), false);
   });
   passed_ms(1);
   BOOST_CHECK_EQUAL(bool ((up and down).push_long()), true);
   BOOST_CHECK_EQUAL(bool ((up and down).push_long()), false);

   mcu::pins[button::up_]->set = false;
   mcu::pins[button::down_]->set = true;
   passed_ms(1); // unsubscribe
}

BOOST_AUTO_TEST_CASE(both_push_after_push)
{
   mcu::pins[button::up_]->set = true;
   process(10, [&](){
      BOOST_CHECK_EQUAL(up.push(), false);
   });
   passed_ms(1);
   BOOST_CHECK_EQUAL(up.push(), true);
   BOOST_CHECK_EQUAL(up.push(), false);

   mcu::pins[button::down_]->set = false;
   process(10, [&](){
      BOOST_CHECK_EQUAL(bool (up and down), false);
   });

   passed_ms(1);
   BOOST_CHECK_EQUAL(bool (up and down), true);
   BOOST_CHECK_EQUAL(bool (up and down), false);

   mcu::pins[button::up_]->set = false;
   mcu::pins[button::down_]->set = true;
   passed_ms(1); // unsubscribe
}

BOOST_AUTO_TEST_CASE(both_not_push)
{
   mcu::pins[button::up_]->set = true;
   mcu::pins[button::down_]->set = false;
   process(5, [&](){
      BOOST_CHECK_EQUAL(bool (up and down), false);
   });

   mcu::pins[button::down_]->set = true;
   process(5, [&](){
      BOOST_CHECK_EQUAL(bool (up and down), false);
   });
   passed_ms(1);
   BOOST_CHECK_EQUAL(bool (up and down), false);

   mcu::pins[button::up_]->set = false;
   mcu::pins[button::down_]->set = true;
   passed_ms(1); // unsubscribe
}


BOOST_AUTO_TEST_SUITE_END()