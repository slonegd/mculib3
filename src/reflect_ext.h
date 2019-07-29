#pragma once

#include "../magic_get/include/boost/pfr/precise.hpp"

namespace reflect {

// смотри пример в /example_magic_get.cpp
// сравнивает структуры
// возможно понадобиться скачать submodule
// работало на коммите 72df78ad288f00b0ea82521f2c5e27ae8755b216
template<typename T>
bool is_equal(const T& a, const T& b)
{
    return boost::pfr::equal_to<T>{}(a, b);
}

} // namespace reflect {

