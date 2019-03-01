#define BOOST_TEST_MODULE test_list
#include <boost/test/unit_test.hpp>

#include "list.h"

BOOST_AUTO_TEST_SUITE (test_suite_main)

struct A : Listable<A> {
    int v;
    A (int v) : v{v} {}
    A() = default;
};

BOOST_AUTO_TEST_CASE(ctor)
{
    List<int> l;
    BOOST_CHECK(l.begin() == nullptr);
    BOOST_CHECK(l.end()   == nullptr);
}

BOOST_AUTO_TEST_CASE(push_back)
{
    List<A> l;
    A a1;
    l.push_back(a1);
    auto it1 = l.begin();
    BOOST_CHECK(  it1 == &a1);
    BOOST_CHECK(++it1 == l.end());

    A a2;
    l.push_back(a2);
    auto it2 = l.begin();
    BOOST_CHECK(  it2 == &a1);
    BOOST_CHECK(++it2 == &a2);
    BOOST_CHECK(++it2 == l.end());

    A a3;
    l.push_back(a3);
    auto it3 = l.begin();
    BOOST_CHECK(  it3 == &a1);
    BOOST_CHECK(++it3 == &a2);
    BOOST_CHECK(++it3 == &a3);
    BOOST_CHECK(++it3 == l.end());
}

BOOST_AUTO_TEST_CASE(push_front)
{
    List<A> l;
    A a1;
    l.push_front(a1);
    auto it1 = l.begin();
    BOOST_CHECK(  it1 == &a1);
    BOOST_CHECK(++it1 == l.end());

    A a2;
    l.push_front(a2);
    auto it2 = l.begin();
    BOOST_CHECK(  it2 == &a2);
    BOOST_CHECK(++it2 == &a1);
    BOOST_CHECK(++it2 == l.end());

    A a3;
    l.push_front(a3);
    auto it3 = l.begin();
    BOOST_CHECK(  it3 == &a3);
    BOOST_CHECK(++it3 == &a2);
    BOOST_CHECK(++it3 == &a1);
    BOOST_CHECK(++it3 == l.end());
}

BOOST_AUTO_TEST_CASE(remove_front)
{
    List<A> l;
    A a1; l.push_back(a1);
    A a2; l.push_back(a2);
    A a3; l.push_back(a3);

    l.remove(a1);
    auto it = l.begin();
    BOOST_CHECK(     it == &a2);
    BOOST_CHECK(   ++it == &a3);
    BOOST_CHECK(   ++it == l.end());
    BOOST_CHECK(a1.prev == nullptr);
    BOOST_CHECK(a1.next == nullptr);
}

BOOST_AUTO_TEST_CASE(remove_back)
{
    List<A> l;
    A a1; l.push_back(a1);
    A a2; l.push_back(a2);
    A a3; l.push_back(a3);

    l.remove(a3);
    auto it = l.begin();
    BOOST_CHECK(     it == &a1);
    BOOST_CHECK(   ++it == &a2);
    BOOST_CHECK(   ++it == l.end());
    BOOST_CHECK(a3.prev == nullptr);
    BOOST_CHECK(a3.next == nullptr);
}

BOOST_AUTO_TEST_CASE(remove_center)
{
    List<A> l;
    A a1; l.push_back(a1);
    A a2; l.push_back(a2);
    A a3; l.push_back(a3);

    l.remove(a2);
    auto it = l.begin();
    BOOST_CHECK(     it == &a1);
    BOOST_CHECK(   ++it == &a3);
    BOOST_CHECK(   ++it == l.end());
    BOOST_CHECK(a2.prev == nullptr);
    BOOST_CHECK(a2.next == nullptr);
}

BOOST_AUTO_TEST_CASE(remove_last)
{
    List<A> l;
    A a1; l.push_back(a1);

    l.remove(a1);
    BOOST_CHECK(l.begin() == nullptr);
    BOOST_CHECK(l.end()   == nullptr);
    BOOST_CHECK(a1.prev   == nullptr);
    BOOST_CHECK(a1.next   == nullptr);
}

BOOST_AUTO_TEST_CASE(insert_front)
{
    List<A> l;
    A a1; l.push_back(a1);
    A a2; l.push_back(a2);
    A a3; l.push_back(a3);

    A a4;
    l.insert(l.begin(), a4);

    auto it = l.begin();
    BOOST_CHECK(  it == &a4);
    BOOST_CHECK(++it == &a1);
    BOOST_CHECK(++it == &a2);
    BOOST_CHECK(++it == &a3);
    BOOST_CHECK(++it == l.end());
}

BOOST_AUTO_TEST_CASE(insert_back)
{
    List<A> l;
    A a1; l.push_back(a1);
    A a2; l.push_back(a2);
    A a3; l.push_back(a3);

    A a4;
    l.insert(l.end(), a4);

    auto it = l.begin();
    BOOST_CHECK(  it == &a1);
    BOOST_CHECK(++it == &a2);
    BOOST_CHECK(++it == &a3);
    BOOST_CHECK(++it == &a4);
    BOOST_CHECK(++it == l.end());
}

BOOST_AUTO_TEST_CASE(insert_center)
{
    List<A> l;
    A a1; l.push_back(a1);
    A a2; l.push_back(a2);
    A a3; l.push_back(a3);

    A a4;
    
    l.insert(++l.begin(), a4);
    auto it = l.begin();
    BOOST_CHECK(  it == &a1);
    BOOST_CHECK(++it == &a4);
    BOOST_CHECK(++it == &a2);
    BOOST_CHECK(++it == &a3);
    BOOST_CHECK(++it == l.end());
}

BOOST_AUTO_TEST_CASE(range_for)
{
    List<A> l;
    A a1 {10}; l.push_back(a1);
    A a2 {20}; l.push_back(a2);
    A a3 {30}; l.push_back(a3);

    std::stringstream ss;
    for (auto& a : l)
        ss << a.v << ' ';
    BOOST_CHECK_EQUAL (ss.str(),
        "10 20 30 "
    );
}

BOOST_AUTO_TEST_CASE(algorithm)
{
    List<A> l;
    A a1 {10}; l.push_back(a1);
    A a2 {20}; l.push_back(a2);
    A a3 {30}; l.push_back(a3);

    A a4 {25};
    
    l.insert (
        std::find_if (
              l.begin()
            , l.end()
            , [&](auto& v){ return v.v > a4.v; }
        )
        , a4
    );

    std::stringstream ss;
    for (auto& a : l)
        ss << a.v << ' ';
    BOOST_CHECK_EQUAL (ss.str(),
        "10 20 25 30 "
    );
}

BOOST_AUTO_TEST_SUITE_END()
