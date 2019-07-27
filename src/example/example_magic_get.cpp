// g++ -std=c++17 example_magic_get.cpp  -I../../magic_get/include
#include "../../magic_get/include/boost/pfr/precise.hpp"
#include <iostream>

template<typename T>
bool is_equal(const T& a, const T& b)
{
    return boost::pfr::equal_to<T>{}(a, b);
}

int main() {
    struct A{
        int a;
        int b;
    };
    auto a1 = A{1,1};
    auto a2 = A{1,1};
    std::cout << is_equal(a1,a2) << "\n";
}