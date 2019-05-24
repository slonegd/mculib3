#define BOOST_TEST_MODULE test_button
#include <boost/test/unit_test.hpp>

#define F_CPU   72000000UL
#define STM32F103xB

#include "mock_gpio.h"
#include "mock_systick.h"
#include "button_old.h"

BOOST_AUTO_TEST_SUITE (test_suite_main)

template <class Function>
void do_n_ticks (int n, Function f)
{
    while (n--) {
        SysTick_Handler();
        f();
    }
}

auto passed_ms = [&](int n){while (n--) SysTick_Handler();};

constexpr auto inverted = true;

auto& but     = mcu::Button::make<mcu::PA1>();
auto& inv_but = mcu::Button::make<mcu::PA2, inverted>();

BOOST_AUTO_TEST_CASE(push)
{
    REF(GPIOA).mock.set (1, true);
    do_n_ticks (10, [&]{
        BOOST_CHECK_EQUAL(but.push(), false);
    });
    passed_ms(1);
    BOOST_CHECK_EQUAL(but.push(), true); // return true only once
    BOOST_CHECK_EQUAL(but.push(), false);
    REF(GPIOA).mock.set (1, false);
    passed_ms(1); // unsubscribe
}

BOOST_AUTO_TEST_CASE(push_long)
{
    REF(GPIOA).mock.set (1, true);
    do_n_ticks (1000, [&]{
        BOOST_CHECK_EQUAL(but.push_long(), false);
    });
    passed_ms(1);
    BOOST_CHECK_EQUAL(but.push_long(), true); // return true only once
    BOOST_CHECK_EQUAL(but.push_long(), false);
    REF(GPIOA).mock.set (1, false);
    passed_ms(1); // unsubscribe
}

BOOST_AUTO_TEST_CASE(increment)
{
    int var = 0;
    REF(GPIOA).mock.set (1, true);
    do_n_ticks (10, [&]{
        var += but;
        BOOST_CHECK_EQUAL(var, 0);
    });
    do_n_ticks (1, [&]{
        var += but;
        BOOST_CHECK_EQUAL(var, 1);
    });

    do_n_ticks (989, [&]{
        var += but;
        BOOST_CHECK_EQUAL(var, 1);
    });
    do_n_ticks (1, [&]{
        var += but;
        BOOST_CHECK_EQUAL(var, 2);
    });

    do_n_ticks (200, [&]{var += but;});
    BOOST_CHECK_EQUAL(var, 3);

    do_n_ticks (200, [&]{var += but;});
    BOOST_CHECK_EQUAL(var, 4);

    do_n_ticks (600, [&]{var += but;});
    BOOST_CHECK_EQUAL(var, 7);

    do_n_ticks (100, [&]{var += but;});
    BOOST_CHECK_EQUAL(var, 8);

    do_n_ticks (200, [&]{var += but;});
    BOOST_CHECK_EQUAL(var, 10);

    do_n_ticks (700, [&]{var += but;});
    BOOST_CHECK_EQUAL(var, 17);

    do_n_ticks (50, [&]{var += but;});
    BOOST_CHECK_EQUAL(var, 18);

    do_n_ticks (950, [&]{var += but;});
    BOOST_CHECK_EQUAL(var, 37);

    do_n_ticks (25, [&]{var += but;});
    BOOST_CHECK_EQUAL(var, 38);

    do_n_ticks (975, [&]{var += but;});
    BOOST_CHECK_EQUAL(var, 77);

    do_n_ticks (25, [&]{var += but;});
    BOOST_CHECK_EQUAL(var, 79);

    do_n_ticks (25, [&]{var += but;});
    BOOST_CHECK_EQUAL(var, 81);

    do_n_ticks (25, [&]{var += but;});
    BOOST_CHECK_EQUAL(var, 83);

    do_n_ticks (25, [&]{var += but;});
    BOOST_CHECK_EQUAL(var, 85);

    do_n_ticks (900, [&]{var += but;});
    BOOST_CHECK_EQUAL(var, 157);

    do_n_ticks (1000, [&]{var += but;});
    BOOST_CHECK_EQUAL(var, 277);

    do_n_ticks (1000, [&]{var += but;});
    BOOST_CHECK_EQUAL(var, 437);


    REF(GPIOA).mock.set (1, false);
    passed_ms(1); // unsubscribe
}

BOOST_AUTO_TEST_CASE(push_inverted)
{
    REF(GPIOA).mock.set (2, false);
    do_n_ticks (10, [&]{
        BOOST_CHECK_EQUAL(inv_but.push(), false);
    });
    passed_ms(1);
    BOOST_CHECK_EQUAL(inv_but.push(), true); // return true only once
    BOOST_CHECK_EQUAL(inv_but.push(), false);
    REF(GPIOA).mock.set (2, true);
    passed_ms(1); // unsubscribe
}

BOOST_AUTO_TEST_CASE(push_long_inverted)
{
    REF(GPIOA).mock.set (2, false);
    do_n_ticks (1000, [&]{
        BOOST_CHECK_EQUAL(inv_but.push_long(), false);
    });
    passed_ms(1);
    BOOST_CHECK_EQUAL(inv_but.push_long(), true); // return true only once
    BOOST_CHECK_EQUAL(inv_but.push_long(), false);
    REF(GPIOA).mock.set (2, true);
    passed_ms(1); // unsubscribe
}

BOOST_AUTO_TEST_CASE(decrement)
{
    int var = 437;

    REF(GPIOA).mock.set (2, false);
    inv_but.push();
    do_n_ticks (10, [&]{var -= inv_but;});
    BOOST_CHECK_EQUAL(var, 436);

    do_n_ticks (1000, [&]{var -= inv_but;});
    BOOST_CHECK_EQUAL(var, 435);

    do_n_ticks (200, [&]{var -= inv_but;});
    BOOST_CHECK_EQUAL(var, 434);

    do_n_ticks (200, [&]{var -= inv_but;});
    BOOST_CHECK_EQUAL(var, 433);

    do_n_ticks (600, [&]{var -= inv_but;});
    BOOST_CHECK_EQUAL(var, 430);

    do_n_ticks (100, [&]{var -= inv_but;});
    BOOST_CHECK_EQUAL(var, 429);

    do_n_ticks (200, [&]{var -= inv_but;});
    BOOST_CHECK_EQUAL(var, 427);

    do_n_ticks (700, [&]{var -= inv_but;});
    BOOST_CHECK_EQUAL(var, 420);

    do_n_ticks (50, [&]{var -= inv_but;});
    BOOST_CHECK_EQUAL(var, 419);

    do_n_ticks (950, [&]{var -= inv_but;});
    BOOST_CHECK_EQUAL(var, 400);

    do_n_ticks (25, [&]{var -= inv_but;});
    BOOST_CHECK_EQUAL(var, 399);

    do_n_ticks (975, [&]{var -= inv_but;});
    BOOST_CHECK_EQUAL(var, 360);

    do_n_ticks (25, [&]{var -= inv_but;});
    BOOST_CHECK_EQUAL(var, 358);

    do_n_ticks (25, [&]{var -= inv_but;});
    BOOST_CHECK_EQUAL(var, 356);

    do_n_ticks (25, [&]{var -= inv_but;});
    BOOST_CHECK_EQUAL(var, 354);

    do_n_ticks (25, [&]{var -= inv_but;});
    BOOST_CHECK_EQUAL(var, 352);

    do_n_ticks (900, [&]{var -= inv_but;});
    BOOST_CHECK_EQUAL(var, 280);

    do_n_ticks (1000, [&]{var -= inv_but;});
    BOOST_CHECK_EQUAL(var, 160);

    do_n_ticks (1000, [&]{var -= inv_but;});
    BOOST_CHECK_EQUAL(var, 0);

    REF(GPIOA).mock.set (2, true);
    passed_ms(1); // unsubscribe
}

BOOST_AUTO_TEST_CASE(both_together_push)
{
    REF(GPIOA).mock.set (1, true);
    REF(GPIOA).mock.set (2, false);
    do_n_ticks (10, [&]{
        BOOST_CHECK_EQUAL(bool (but and inv_but), false);
    });
    passed_ms(1);
    BOOST_CHECK_EQUAL(bool (but and inv_but), true);
    BOOST_CHECK_EQUAL(bool (but and inv_but), false);

    REF(GPIOA).mock.set (1, false);
    REF(GPIOA).mock.set (2, true);
    passed_ms(1); // unsubscribe
}

BOOST_AUTO_TEST_CASE(both_together_push_long)
{
    REF(GPIOA).mock.set (1, true);
    REF(GPIOA).mock.set (2, false);
    do_n_ticks (1000, [&]{
        BOOST_CHECK_EQUAL(bool ((but and inv_but).push_long()), false);
    });
    passed_ms(1);
    BOOST_CHECK_EQUAL(bool ((but and inv_but).push_long()), true);

    REF(GPIOA).mock.set (1, false);
    REF(GPIOA).mock.set (2, true);
    passed_ms(1); // unsubscribe
}

BOOST_AUTO_TEST_CASE(both_push)
{
    REF(GPIOA).mock.set (1, true);
    do_n_ticks (5, [&]{
        BOOST_CHECK_EQUAL(bool (but and inv_but), false);
    });

    REF(GPIOA).mock.set (2, false);
    do_n_ticks (5, [&]{
        BOOST_CHECK_EQUAL(bool (but and inv_but), false);
    });
    passed_ms(1);
    BOOST_CHECK_EQUAL(bool (but and inv_but), false);
    do_n_ticks (4, [&]{
        BOOST_CHECK_EQUAL(bool (but and inv_but), false);
    });
    passed_ms(1);
    BOOST_CHECK_EQUAL(bool (but and inv_but), true);
    BOOST_CHECK_EQUAL(bool (but and inv_but), false);

    REF(GPIOA).mock.set (1, false);
    REF(GPIOA).mock.set (2, true);
    passed_ms(1); // unsubscribe
}

BOOST_AUTO_TEST_CASE(both_push_long)
{
    REF(GPIOA).mock.set (1, true);
    do_n_ticks (100, [&]{
        BOOST_CHECK_EQUAL(bool ((but and inv_but).push_long()), false);
    });
    
    REF(GPIOA).mock.set (2, false);
    do_n_ticks (900, [&]{
        BOOST_CHECK_EQUAL(bool ((but and inv_but).push_long()), false);
    });
    passed_ms(1);
    BOOST_CHECK_EQUAL(bool ((but and inv_but).push_long()), false);
    do_n_ticks (99, [&]{
        BOOST_CHECK_EQUAL(bool ((but and inv_but).push_long()), false);
    });
    passed_ms(1);
    BOOST_CHECK_EQUAL(bool ((but and inv_but).push_long()), true);
    BOOST_CHECK_EQUAL(bool ((but and inv_but).push_long()), false);

    REF(GPIOA).mock.set (1, false);
    REF(GPIOA).mock.set (2, true);
    passed_ms(1); // unsubscribe
}

BOOST_AUTO_TEST_CASE(both_push_after_push)
{
    REF(GPIOA).mock.set (1, true);
    do_n_ticks (10, [&]{
        BOOST_CHECK_EQUAL(but.push(), false);
    });
    passed_ms(1);
    BOOST_CHECK_EQUAL(but.push(), true);
    BOOST_CHECK_EQUAL(but.push(), false);

    REF(GPIOA).mock.set (2, false);
    do_n_ticks (10, [&]{
        BOOST_CHECK_EQUAL(bool (but and inv_but), false);
    });
    passed_ms(1);
    BOOST_CHECK_EQUAL(bool (but and inv_but), true);
    BOOST_CHECK_EQUAL(bool (but and inv_but), false);

    REF(GPIOA).mock.set (1, false);
    REF(GPIOA).mock.set (2, true);
    passed_ms(1); // unsubscribe
}

BOOST_AUTO_TEST_CASE(both_not_push)
{
    REF(GPIOA).mock.set (1, true);
    REF(GPIOA).mock.set (2, false);
    do_n_ticks (5, [&]{
        BOOST_CHECK_EQUAL(bool (but and inv_but), false);
    });

    REF(GPIOA).mock.set (2, true);
    do_n_ticks (5, [&]{
        BOOST_CHECK_EQUAL(bool (but and inv_but), false);
    });
    passed_ms(1);
    BOOST_CHECK_EQUAL(bool (but and inv_but), false);

    REF(GPIOA).mock.set (1, false);
    REF(GPIOA).mock.set (2, true);
    passed_ms(1); // unsubscribe
}


BOOST_AUTO_TEST_SUITE_END()