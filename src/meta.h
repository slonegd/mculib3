#pragma once

#include <type_traits>
#include <utility>
#include <stdint.h>
#include <cstddef>
#include <iterator>
#include <array>
#include <tuple>
#include "../magic_get/include/boost/pfr/precise.hpp"


namespace meta {

// определяет первое положение конкретного типа из вариадика, пример:
// начиная с 1
// const int at = Position<T, Types...>::value;
// возвращает -1 если такого нет
template<int n, class T, class U, class ... Types>
struct PositionIterator {
    static const int value =
        std::is_same<T,U>::value ? n : PositionIterator<n + 1, T, Types...>::value;
};
template<int n, class T, class U>
struct PositionIterator<n, T, U> {
    static const int value =
        std::is_same<T,U>::value ? n : -1;
};
template<class T, class U, class ... Types> struct Position {
    static const int value =
        PositionIterator<1, T, U, Types...>::value;
};

template<class T, class ... Ts>
constexpr int position_v = Position<T,Ts...>::value;





/// генерирация массива данных
template<auto& f, class index_sequence>
struct generate_impl;

template<auto& f, size_t ... i>
struct generate_impl<f, std::index_sequence<i...>>
{
    using type = decltype(f(0));
    static constexpr auto size = sizeof...(i);
    static constexpr std::array<type,size> table {f(i)...};
};

/// генерировать массив функцией f
template<auto& f, size_t size>
constexpr auto generate = generate_impl<f, std::make_index_sequence<size>>::table;
} // namespace meta {

namespace { // test

/// пример генерирующей функции
constexpr size_t fibo (size_t i) { return (i > 1) ? fibo(i-2) + fibo(i-1) : 1; }

/// пример использования + тест
constexpr auto fibo_example = meta::generate<fibo, 6>;

static_assert (fibo_example[0] == 1);
static_assert (fibo_example[1] == 1);
static_assert (fibo_example[2] == 2);
static_assert (fibo_example[3] == 3);
static_assert (fibo_example[4] == 5);
static_assert (fibo_example[5] == 8);
static_assert (fibo_example.size() == 6);

} // namespace {


namespace meta {

/// генерирация tuple типа, состоящего из заданного количество типов T
template<class T, class index_sequence>
struct tuple_generate_impl;

template<class T, size_t i>
struct placeholder { using type = T; };

template<class T, size_t ... i>
struct tuple_generate_impl<T, std::index_sequence<i...>>
{
    using type = std::tuple<typename placeholder<T,i>::type...>;
};

template<class T, size_t n>
using tuple_generate_t = typename tuple_generate_impl<T, std::make_index_sequence<n>>::type;



/// метаструктура, проходящая в операторе() по элементам tuple
template<size_t index, class Base, class function, class ... T>
struct tuple_foreach_impl
{
    void operator () (std::tuple<T...>& t, function f)
    {
        const size_t i = sizeof...(T) - index;
        f(static_cast<Base*>(&std::get<i>(t)));
        tuple_foreach_impl<index - 1, Base, function, T...>() (t,f);
    }
};
/// для конца рекурсии
template<class function, class Base, class ... T>
struct tuple_foreach_impl<0, Base, function, T...>
{
    void operator () (std::tuple<T...>& t, function f)
    {
    }
};

template<class Base, class function, class ... T>
void tuple_foreach (std::tuple<T...>& t, function f)
{
     tuple_foreach_impl<sizeof...(T), Base, function, T...>() (t,f);
}



constexpr auto all_is_same = [](auto...vs) {
    auto v = *std::initializer_list{vs...}.begin();
    return ((v == vs) and ...);
};


// смотри пример в /example
// сравнивает структуры
// возможно понадобиться скачать submodule
// работало на коммите 72df78ad288f00b0ea82521f2c5e27ae8755b216
template<typename T>
bool is_equal(const T& a, const T& b)
{
    return boost::pfr::greater_equal<T>{}(a, b);
}

} // namespace meta {


