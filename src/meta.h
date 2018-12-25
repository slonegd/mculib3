#pragma once

#include <type_traits>
#include <utility>
#include <stdint.h>
#include <cstddef>
#include <iterator>
#include <array>


// определяет количество типов вариадика, пример:
// const uint8_t qty = QtyTypes<Types...>::value;
template <class T, class ... Types> struct QtyTypes {
   static const uint8_t value = 1 + QtyTypes<Types...>::value;
};
template<class T> struct QtyTypes<T> {
   static const uint8_t value = 1;
};



// определяет конкретный тип аргумента вариадика, пример:
// using Type = At<2, Types...>::Result;
template<size_t n, class T, class ... Types> struct At {
   using Result = typename At<n-1, Types...>::Result;
};
template<class T, class ... Types> struct At<1,T,Types...> {
   using Result = T;
};

template <size_t n, class ... Ts>
using at_t = typename At<n,Ts...>::Result;

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





/// генерирация массива данных (хелперы)
/// пример генерирующей функции
constexpr size_t fibo (size_t i) { return (i > 1) ? fibo(i-2) + fibo(i-1) : 1; }

template<class T, T(*f)(size_t), class index_sequence>
struct generate_impl;

template<class T, T(*f)(size_t), size_t ... i>
struct generate_impl<T, f, std::index_sequence<i...>>
{
   static constexpr std::array<T,sizeof...(i)> table {f(i)...};
};

/// генерировать массив функцией f
template<class T, T(*f)(size_t), size_t size>
using Generate = generate_impl<T, f, std::make_index_sequence<size>>;

/// пример использования + тест
using Fibo_example = Generate<size_t, &fibo, 6>;
static_assert (Fibo_example::table[0] == 1);
static_assert (Fibo_example::table[1] == 1);
static_assert (Fibo_example::table[2] == 2);
static_assert (Fibo_example::table[3] == 3);
static_assert (Fibo_example::table[4] == 5);
static_assert (Fibo_example::table[5] == 8);
static_assert (std::size(Fibo_example::table) == 6);




