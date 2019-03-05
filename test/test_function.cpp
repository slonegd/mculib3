#define BOOST_TEST_MODULE test_function
#include <boost/test/unit_test.hpp>

#include "newdelete.h"
#include "function.h"
#include <functional>

BOOST_AUTO_TEST_SUITE (test_suite_main)

auto i1 {1};

auto foo() {
    return i1;
}

struct Callable {
    int i2 {2};
    auto operator()() {
        return i2;
    }

    int i3 {2};
    auto foo() {
        return i3;
    }

};

BOOST_AUTO_TEST_CASE(free_function)
{
    Function<int()> f {foo};
    BOOST_CHECK_EQUAL(f(), 1);
}

BOOST_AUTO_TEST_CASE(callable)
{
    Callable callable;
    Function<int()> f {callable};
    BOOST_CHECK_EQUAL(f(), 2);
}

BOOST_AUTO_TEST_CASE(lambda)
{
    int i3 {3};
    Function<int()> f {[&]{return i3;}};
    BOOST_CHECK_EQUAL(f(), 3);
}

BOOST_AUTO_TEST_CASE(no_heap)
{
    // необходимо задать некоторое количество объектов
    // , чтобы std::function вызвал new
    int i3 {3};
    int i4 {4};
    Callable callable;

    auto lambda = [&]{return i3 + i4 + callable();};

    auto start = mock::alloc_counter;
    std::function<int()> f1 {lambda};
    BOOST_CHECK (start != mock::alloc_counter); // used heap

    start = mock::alloc_counter;
    Function<int()> f2 {lambda};
    BOOST_CHECK (start == mock::alloc_counter); // dont used heap

}

BOOST_AUTO_TEST_SUITE_END()
