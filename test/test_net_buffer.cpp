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

BOOST_AUTO_TEST_CASE (input_operator)
{
   auto buffer = Net_buffer<10>{};
   buffer << uint8_t(1)
          << uint8_t(2)
          << uint8_t(3)
          << uint8_t(4)
          << uint8_t(5)
          << uint8_t(6)
          << uint8_t(7)
          << uint8_t(8)
          << uint8_t(9)
          << uint8_t(10);

   std::array<uint8_t, 2> v8;
   buffer >> v8[0] >> v8[1];
   BOOST_CHECK_EQUAL (v8[0], 1);
   BOOST_CHECK_EQUAL (v8[1], 2);
   BOOST_CHECK_EQUAL (buffer.size(), 8);
   BOOST_CHECK_EQUAL (std::distance(buffer.begin(), buffer.end()), 8);

   std::array<uint16_t, 4> v16;
   buffer >> v16[0] >> v16[1] >> v16[2] >> v16[3];
   BOOST_CHECK_EQUAL (v16[0], 0x0304);
   BOOST_CHECK_EQUAL (v16[1], 0x0506);
   BOOST_CHECK_EQUAL (v16[2], 0x0708);
   BOOST_CHECK_EQUAL (v16[3], 0x090A);
   BOOST_CHECK_EQUAL (buffer.size(), 0);
   BOOST_CHECK_EQUAL (std::distance(buffer.begin(), buffer.end()), 0);

   std::fill (buffer.begin(), buffer.end(), 255);
   buffer >> v8[0] >> v8[1];
   BOOST_CHECK_EQUAL (v8[0], 1);
   BOOST_CHECK_EQUAL (v8[1], 2);
   BOOST_CHECK_EQUAL (buffer.size(), 0);
   BOOST_CHECK_EQUAL (std::distance(buffer.begin(), buffer.end()), 0);

   buffer >> v16[0] >> v16[1] >> v16[2] >> v16[3];
   BOOST_CHECK_EQUAL (v16[0], 0x0304);
   BOOST_CHECK_EQUAL (v16[1], 0x0506);
   BOOST_CHECK_EQUAL (v16[2], 0x0708);
   BOOST_CHECK_EQUAL (v16[3], 0x090A);
   BOOST_CHECK_EQUAL (buffer.size(), 0);
   BOOST_CHECK_EQUAL (std::distance(buffer.begin(), buffer.end()), 0);
}

BOOST_AUTO_TEST_CASE (pop_back)
{
   auto buffer = Net_buffer<3>{};
   buffer << uint8_t(1)
          << uint8_t(2)
          << uint8_t(3);

   BOOST_CHECK_EQUAL (buffer.pop_back(), 3);
   BOOST_CHECK_EQUAL (buffer.size(), 2);
   BOOST_CHECK_EQUAL (std::distance(buffer.begin(), buffer.end()), 2);

   BOOST_CHECK_EQUAL (buffer.pop_back(), 2);
   BOOST_CHECK_EQUAL (buffer.size(), 1);
   BOOST_CHECK_EQUAL (std::distance(buffer.begin(), buffer.end()), 1);

   BOOST_CHECK_EQUAL (buffer.pop_back(), 1);
   BOOST_CHECK_EQUAL (buffer.size(), 0);
   BOOST_CHECK_EQUAL (std::distance(buffer.begin(), buffer.end()), 0);

   BOOST_CHECK_EQUAL (buffer.pop_back(), 0);
   BOOST_CHECK_EQUAL (buffer.size(), 0);
   BOOST_CHECK_EQUAL (std::distance(buffer.begin(), buffer.end()), 0);
}

BOOST_AUTO_TEST_CASE (pop_front)
{
   auto buffer = Net_buffer<3>{};
   buffer << uint8_t(1)
          << uint8_t(2)
          << uint8_t(3);

   BOOST_CHECK_EQUAL (buffer.pop_front(), 1);
   BOOST_CHECK_EQUAL (buffer.size(), 2);
   BOOST_CHECK_EQUAL (std::distance(buffer.begin(), buffer.end()), 2);

   BOOST_CHECK_EQUAL (buffer.pop_front(), 2);
   BOOST_CHECK_EQUAL (buffer.size(), 1);
   BOOST_CHECK_EQUAL (std::distance(buffer.begin(), buffer.end()), 1);

   BOOST_CHECK_EQUAL (buffer.pop_front(), 3);
   BOOST_CHECK_EQUAL (buffer.size(), 0);
   BOOST_CHECK_EQUAL (std::distance(buffer.begin(), buffer.end()), 0);

   BOOST_CHECK_EQUAL (buffer.pop_front(), 0);
   BOOST_CHECK_EQUAL (buffer.size(), 0);
   BOOST_CHECK_EQUAL (std::distance(buffer.begin(), buffer.end()), 0);
}

BOOST_AUTO_TEST_CASE (front)
{
   auto buffer = Net_buffer<3>{};
   buffer << uint8_t(1)
          << uint8_t(2)
          << uint8_t(3);
   BOOST_CHECK_EQUAL (buffer[0], 1);

   BOOST_CHECK_EQUAL (buffer.front(), 1);
   BOOST_CHECK_EQUAL (buffer.size(), 3);
   BOOST_CHECK_EQUAL (std::distance(buffer.begin(), buffer.end()), 3);

   buffer.front() = 4;

   BOOST_CHECK_EQUAL (buffer.front(), 4);
   BOOST_CHECK_EQUAL (buffer[0], 4);
   BOOST_CHECK_EQUAL (buffer.size(), 3);
   BOOST_CHECK_EQUAL (std::distance(buffer.begin(), buffer.end()), 3);

   uint8_t _;
   buffer >> _;
   BOOST_CHECK_EQUAL (buffer.size(), 2);
   BOOST_CHECK_EQUAL (std::distance(buffer.begin(), buffer.end()), 2);
}

BOOST_AUTO_TEST_SUITE_END()
