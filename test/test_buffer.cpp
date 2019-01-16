#define BOOST_TEST_MODULE test_buffer
#include <boost/test/unit_test.hpp>

#include "buffer.h"

BOOST_AUTO_TEST_SUITE (test_suite_main)

BOOST_AUTO_TEST_CASE (member_count)
{
   auto buffer = Buffer<uint8_t,10>{};
   buffer << std::array<uint8_t, 2>{1,2};
   BOOST_CHECK_EQUAL (buffer[0], 1);
}

BOOST_AUTO_TEST_SUITE_END()
