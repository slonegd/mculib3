#pragma once

#include <type_traits>
#include <utility>
#include <stdint.h>
#include <cstddef>
#include <iterator>

template <class T, class ... Types> 
constexpr T get1stArg (T arg1, Types ... otherArgs)
{
   return arg1;
}



template <class T, class ... Types> struct get1stType {
   using Result = T;
};

template <class ... Ts>
using get1st_t = typename get1stType<Ts...>::Result;


#define IS_SAME(T1,T2) (std::is_same<T1,T2>::value)




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

template<class T, class function, function* f, class index_sequence>
struct generate_impl;

template<class T, class function, function* f, size_t ... i>
struct generate_impl<T, function, f, std::index_sequence<i...>>
{
   static constexpr std::array<T, sizeof...(i)> table {f(i)...};
};

/// генерировать массив функцией f
template<class function, function* f, size_t size, class T = size_t>
using Generate = generate_impl<T, function, f, std::make_index_sequence<size>>;

/// пример использования + тест
using Fibo_example = Generate<decltype(fibo), &fibo, 6>;
static_assert (Fibo_example::table[0] == 1);
static_assert (Fibo_example::table[1] == 1);
static_assert (Fibo_example::table[2] == 2);
static_assert (Fibo_example::table[3] == 3);
static_assert (Fibo_example::table[4] == 5);
static_assert (Fibo_example::table[5] == 8);
static_assert (std::size(Fibo_example::table) == 6);




/// изменения массива данных (хелперы)
/// пример изменяющей функции
constexpr size_t x2 (size_t i, size_t v) { return v * 2; }

template<class T, class Source, decltype(&x2) f, class index_sequence>
struct transform_impl;

template<class T, class Source, decltype(&x2) f, size_t ... i>
struct transform_impl<T, Source, f, std::index_sequence<i...>>
{
   static constexpr T table[] {f(i,Source::table[i])...};
};

/// изменить массив Source::table функцией f
template<class Source, decltype(&x2) f, class T = size_t>
using Transform = transform_impl<T, Source, f, std::make_index_sequence<std::size(Source::table)>>;

/// пример использования и тесты
using Fibox2_example = Transform<Fibo_example, &x2>;
static_assert (Fibox2_example::table[0] == 2);
static_assert (Fibox2_example::table[1] == 2);
static_assert (Fibox2_example::table[2] == 4);
static_assert (Fibox2_example::table[3] == 6);
static_assert (Fibox2_example::table[4] == 10);
static_assert (Fibox2_example::table[5] == 16);
static_assert (std::size(Fibox2_example::table) == 6);
