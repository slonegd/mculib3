#define BOOST_TEST_MODULE test_dynarray
#include <boost/test/unit_test.hpp>

#include "newdelete.h" // считает количество аллокаций памяти глобальными new/delete
#include "dynarray.h"

BOOST_AUTO_TEST_SUITE (test_suite_main)


BOOST_AUTO_TEST_CASE(ctor)
{
    auto start = mock::alloc_counter;
    Dyn_array<int> ar{};
    BOOST_CHECK (start == mock::alloc_counter); // dont used heap
    BOOST_CHECK_EQUAL(ar.size()   , 0);
    BOOST_CHECK_EQUAL(ar.address(), 0);
}

BOOST_AUTO_TEST_CASE(add_size)
{
    auto start = mock::alloc_counter;
    Dyn_array<int> ar{};
    ar.add_size(10);
    BOOST_CHECK (start == mock::alloc_counter); // dont used heap
    BOOST_CHECK_EQUAL(ar.size(), 10);
    BOOST_CHECK (ar.address() != 0);
}

BOOST_AUTO_TEST_CASE(double_add_size)
{
    auto start = mock::alloc_counter;
    Dyn_array<int> ar{};
    ar.add_size(10);
    BOOST_CHECK (start == mock::alloc_counter); // dont used heap
    BOOST_CHECK_EQUAL(ar.size(), 10);
    auto address = ar.address();
    BOOST_CHECK (address != 0);

    ar.add_size(5);
    BOOST_CHECK (start == mock::alloc_counter); // dont used heap
    BOOST_CHECK_EQUAL(ar.size(), 15);
    BOOST_CHECK_EQUAL(address, ar.address());
}

BOOST_AUTO_TEST_CASE(bad_double_add_size)
{
    auto start = mock::alloc_counter;
    Dyn_array<int> ar{};
    ar.add_size(10);
    BOOST_CHECK (start == mock::alloc_counter); // dont used heap
    BOOST_CHECK_EQUAL(ar.size(), 10);
    auto address = ar.address();
    BOOST_CHECK (address != 0);

    heap.allocate (1);

    ar.add_size(5);
    BOOST_CHECK (start == mock::alloc_counter); // dont used heap
    BOOST_CHECK_EQUAL(ar.size(), 15);
    BOOST_CHECK_EQUAL(ar.address(), 0);
}

BOOST_AUTO_TEST_SUITE_END()
