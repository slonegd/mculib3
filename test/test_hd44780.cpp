#define USE_MOCK_GPIO
#define BOOST_TEST_MODULE test_hd44780
#include <boost/test/unit_test.hpp>

#include "hd44780.h"

BOOST_AUTO_TEST_SUITE (test_suite_main)

namespace mock {

struct LCD : public LCD {
   std::array<char, 80> screen;
};

}

BOOST_AUTO_TEST_CASE(make)
{
   
}

BOOST_AUTO_TEST_SUITE_END()