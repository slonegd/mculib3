#define BOOST_TEST_MODULE test_lcd
#include <boost/test/unit_test.hpp>
#include <algorithm>
#include <array>

#include "string_buffer.h"
#include "cyrrilyc.h"

// FIX сделать тесты независимыми от предыдущих

BOOST_AUTO_TEST_SUITE (test_suite_main)

    String_buffer lcd;
    std::string_view screen {lcd.get_buffer().begin(), 80};

BOOST_AUTO_TEST_CASE(constructor)
{
    BOOST_CHECK_EQUAL(screen,
                            "                    "
                            "                    "
                            "                    "
                            "                    ");
}

BOOST_AUTO_TEST_CASE(string)
{
    lcd << "Hello, World!";

    BOOST_CHECK_EQUAL(screen,
                            "Hello, World!       "
                            "                    "
                            "                    "
                            "                    ");


    lcd << "Alex_Plus";

    BOOST_CHECK_EQUAL(screen,
                            "Hello, World!Alex_Pl"
                            "us                  "
                            "                    "
                            "                    ");

}

BOOST_AUTO_TEST_CASE(numeric)
{
    lcd << 2019;

    BOOST_CHECK_EQUAL(screen,
                            "Hello, World!Alex_Pl"
                            "us2019              "
                            "                    "
                            "                    ");

}

BOOST_AUTO_TEST_CASE(line_and_cursor)
{
    lcd.line(2).cursor(8) << "Alex_Plus";

    BOOST_CHECK_EQUAL(screen,
                            "Hello, World!Alex_Pl"
                            "us2019              "
                            "        Alex_Plus   "
                            "                    ");
    
}

BOOST_AUTO_TEST_CASE(center)
{
    lcd.line(3).center() << "Test_buffer";

    BOOST_CHECK_EQUAL(screen,
                            "Hello, World!Alex_Pl"
                            "us2019              "
                            "        Alex_Plus   "
                            "    Test_buffer     ");

}

BOOST_AUTO_TEST_CASE(rus)
{
    lcd.line(3) << Hello_World_rus;
    auto result = std::string {Hello_World_rus};

    BOOST_CHECK_EQUAL(screen,
                            "Hello, World!Alex_Pl"
                            "us2019              "
                            "        Alex_Plus   "
                             + result + "fer     ");
}

BOOST_AUTO_TEST_CASE(line_overflow)
{
    lcd << "Hello, World!";
    auto result = std::string {Hello_World_rus};
    
    BOOST_CHECK_EQUAL(screen,
                            "orld!, World!Alex_Pl"
                            "us2019              "
                            "        Alex_Plus   "
                             + result + "Hello, W");
}

BOOST_AUTO_TEST_CASE(screen_overflow)
{
    lcd.line(0) << "The Dursleys had everything they wanted, but they also had a secret, and"
                        " their greatest fear was that somebody would discover it.";

    BOOST_CHECK_EQUAL(screen,
                            "reatest fear was tha"
                            "t somebody would dis"
                            "cover it. also had a"
                            " secret, and their g");
}

BOOST_AUTO_TEST_CASE(shift_cursor)
{
    lcd.line(0).cursor(25) << " Cursor is here";

    BOOST_CHECK_EQUAL(screen,
                            "reatest fear was tha"
                            "t som Cursor is here"
                            "cover it. also had a"
                            " secret, and their g");

    lcd.line(0) << "                    "
                   "                    "
                   "                    "
                   "                    ";

    BOOST_CHECK_EQUAL(screen,
        "                    "
        "                    "
        "                    "
        "                    ");
}

BOOST_AUTO_TEST_CASE(width)
{
    lcd.line(0).width(4) << 25;

    BOOST_CHECK_EQUAL(screen,
        "  25                "
        "                    "
        "                    "
        "                    ");
}

BOOST_AUTO_TEST_CASE(next_line_)
{
    lcd << next_line;
    lcd << 52;

    BOOST_CHECK_EQUAL(screen,
        "  25                "
        "52                  "
        "                    "
        "                    ");
}

BOOST_AUTO_TEST_CASE(next_line_last_line)
{
    lcd.clear();
    lcd.line(3) << 64 << next_line;
    lcd << 78;

    BOOST_CHECK_EQUAL(screen,
        "78                  "
        "                    "
        "                    "
        "64                  ");
}

BOOST_AUTO_TEST_CASE(negative_number)
{
    lcd.clear();
    lcd << -101;

    BOOST_CHECK_EQUAL(screen,
        "-101                "
        "                    "
        "                    "
        "                    ");
}



BOOST_AUTO_TEST_SUITE_END()