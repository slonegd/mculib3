#pragma once

#include <string>
#include <stdexcept>


namespace test {

struct static_assert_exception : std::logic_error
{
   static_assert_exception (const char* what) : std::logic_error {what} {};
   static_assert_exception() : std::logic_error {""} {};
};

std::string static_assert_message {};

void static_assert_ (bool condition, const char* what)
{
   try {
      if (not condition) throw test::static_assert_exception {what};
   } catch (test::static_assert_exception const & e) {
        static_assert_message = e.what();
        throw e;
   }
}

void static_assert_ (bool condition)
{
   if (not condition) throw test::static_assert_exception {};
}

} // namespace test

// A macro redefinition of `static_assert`
#define static_assert(...) test::static_assert_ (__VA_ARGS__)

#define STATIC_ASSERTATION_REQUIRED(act,message) \
   test::static_assert_message = ""; \
   BOOST_CHECK_THROW (act, test::static_assert_exception); \
   BOOST_CHECK_EQUAL (test::static_assert_message, message)

