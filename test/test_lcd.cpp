#define BOOST_TEST_MODULE test_lcd
#include <boost/test/unit_test.hpp>
#include <algorithm>
#include <array>

#include "lcd_ram.h"

BOOST_AUTO_TEST_SUITE (test_suite_main)

   LCD buffer;
   // std::stringstream string;

BOOST_AUTO_TEST_CASE(constructor)
{
   std::array<char,80> test_buffer;
   test_buffer.fill(' ');
   bool equal = std::equal(buffer.begin(), buffer.end(), test_buffer.begin());
}

BOOST_AUTO_TEST_CASE(string)
{
   std::string string {"Hello, World!"};
   buffer << string;

   bool equal = std::equal(buffer.begin(), buffer.begin() + string.size(), string.begin());
   BOOST_CHECK_EQUAL(equal, true);

   std::string next_string {"Alex_Plus"};
   buffer << next_string;

   equal = std::equal(buffer.begin() + string.size(), 
                      buffer.begin() + string.size() + next_string.size(),
                      next_string.begin());
   BOOST_CHECK_EQUAL(equal, true);
}

BOOST_AUTO_TEST_CASE(line)
{
   std::string string {"Hello, World!"};
   buffer.set_line(0) << string;
   bool string_0 = std::equal(buffer.begin(), buffer.begin() + string.size(), string.begin());
   buffer.set_line(1) << string;
   bool string_1 = std::equal(buffer.begin() + 20, buffer.begin() + 20 + string.size(), string.begin());
   buffer.set_line(2) << string;
   bool string_2 = std::equal(buffer.begin() + 40, buffer.begin() + 40 + string.size(), string.begin());
   buffer.set_line(3) << string;
   bool string_3 = std::equal(buffer.begin() + 60, buffer.begin() + 60 + string.size(), string.begin());

   BOOST_CHECK_EQUAL(string_0, true);
   BOOST_CHECK_EQUAL(string_1, true);
   BOOST_CHECK_EQUAL(string_2, true);
   BOOST_CHECK_EQUAL(string_3, true);
}

BOOST_AUTO_TEST_CASE(cursor)
{
   std::string string {"hello, World!"};
   
   buffer.set_line(0).set_cursor(5);
   BOOST_CHECK_EQUAL(buffer.get_position(), 5);
   buffer << string;
   bool string_0 = std::equal(buffer.begin() + 5, buffer.begin() + 5 + string.size(), string.begin());
   BOOST_CHECK_EQUAL(string_0, true);


   buffer.set_line(0).set_cursor(61);
   BOOST_CHECK_EQUAL(buffer.get_position(), 61);

   buffer.set_line(1).set_cursor(5);
   BOOST_CHECK_EQUAL(buffer.get_position(), 25);

   buffer.set_line(2).set_cursor(5);
   BOOST_CHECK_EQUAL(buffer.get_position(), 45);

   buffer.set_line(3).set_cursor(5);
   BOOST_CHECK_EQUAL(buffer.get_position(), 65);
}

BOOST_AUTO_TEST_CASE(numeric)
{
   std::string _123 {"123"};
   std::string _2019 {"2019"};
   buffer.set_line(0) << 123;
   bool _123_ = std::equal(buffer.begin(), buffer.begin() + _123.size(), _123.begin());
   buffer << 2019;
   bool _2019_ = std::equal(buffer.begin() + _123.size(), buffer.begin() + _123.size() + _2019.size(), _2019.begin());
   BOOST_CHECK_EQUAL(_123_,  true);
   BOOST_CHECK_EQUAL(_2019_, true);
}

BOOST_AUTO_TEST_CASE(central)
{
   std::string string {"Alex_Plus"};
   std::string centre_string {"     Alex_Plus      "};
   buffer.set_line(3).central() << string;
   BOOST_CHECK_EQUAL(std::equal(buffer.begin() + 60, 
                                buffer.begin() + 60 + centre_string.size(), 
                                centre_string.begin()), true);
}


BOOST_AUTO_TEST_SUITE_END()