#pragma once

#include <string>
#include <stdexcept>
#include "variadic_macros.h"


namespace test {

struct static_assert_exception : std::logic_error
{
   static_assert_exception (const char* what) : std::logic_error {what} {};
   virtual ~static_assert_exception() noexcept {}
};

template<bool> struct try_static_assert;

template<> struct try_static_assert<true>
{
   explicit try_static_assert (const char*) { }
};


template<> struct try_static_assert<false>
{
   explicit try_static_assert (const char* what) {
      throw static_assert_exception (what);
   }
};

} // namespace test

std::string static_assert_message;

// A macro redefinition of `static_assert`
#define static_assert(...) \
   try { \
      test::try_static_assert<POP_LAST(__VA_ARGS__)> {(LAST(__VA_ARGS__))}; \
   } catch (test::static_assert_exception const & e) { \
        static_assert_message = e.what(); \
        throw e; \
   }

#define STATIC_ASSERTATION_REQUIRED(act,message) \
   static_assert_message = ""; \
   BOOST_CHECK_THROW (act, test::static_assert_exception); \
   BOOST_CHECK_EQUAL (static_assert_message, message)
