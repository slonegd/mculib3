#define BOOST_TEST_MODULE test_net_buffer
#include <boost/test/unit_test.hpp>

#include "net_buffer.h"

BOOST_AUTO_TEST_SUITE (test_suite_main)

BOOST_AUTO_TEST_CASE (ctor)
{
   auto buffer = Net_buffer<10>{};
   BOOST_CHECK_EQUAL (buffer.size(), 0);
   BOOST_CHECK_EQUAL (buffer.begin(), buffer.end());
}

BOOST_AUTO_TEST_CASE (size)
{
   auto buffer = Net_buffer<10>{};

   buffer.set_size (5);
   BOOST_CHECK_EQUAL (buffer.size(), 5);
   BOOST_CHECK_EQUAL (std::distance(buffer.begin(), buffer.end()), 5);

   buffer.set_size (11);
   BOOST_CHECK_EQUAL (buffer.size(), 10);
   BOOST_CHECK_EQUAL (std::distance(buffer.begin(), buffer.end()), 10);
}

BOOST_AUTO_TEST_CASE (output_operator)
{
   auto buffer = Net_buffer<10>{};
   buffer << uint8_t(1);
   BOOST_CHECK_EQUAL (buffer[0], 1);
   BOOST_CHECK_EQUAL (buffer.size(), 1);
   BOOST_CHECK_EQUAL (std::distance(buffer.begin(), buffer.end()), 1);

   buffer << uint16_t(0x0203);
   BOOST_CHECK_EQUAL (buffer[1], 2);
   BOOST_CHECK_EQUAL (buffer[2], 3);
   BOOST_CHECK_EQUAL (buffer.size(), 3);
   BOOST_CHECK_EQUAL (std::distance(buffer.begin(), buffer.end()), 3);

   buffer << std::array{uint8_t(5), uint8_t(4)};
   BOOST_CHECK_EQUAL (buffer[3], 5);
   BOOST_CHECK_EQUAL (buffer[4], 4);
   BOOST_CHECK_EQUAL (buffer.size(), 5);
   BOOST_CHECK_EQUAL (std::distance(buffer.begin(), buffer.end()), 5);

   for (auto i {5}; i < 10; ++i) {
      buffer << uint8_t(i+1);
      BOOST_CHECK_EQUAL (buffer[i], i+1);
      BOOST_CHECK_EQUAL (buffer.size(), i+1);
      BOOST_CHECK_EQUAL (std::distance(buffer.begin(), buffer.end()), i+1);
   }
   BOOST_CHECK_EQUAL (buffer.size(), 10);

   buffer << uint8_t(11);
   BOOST_CHECK_EQUAL (buffer[9], 10);
   BOOST_CHECK_EQUAL (buffer[0], 1);
   BOOST_CHECK_EQUAL (buffer.size(), 10);
   BOOST_CHECK_EQUAL (std::distance(buffer.begin(), buffer.end()), 10);

}

BOOST_AUTO_TEST_SUITE_END()
