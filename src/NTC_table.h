#pragma once

#include <stdint.h>
#include <cstddef>
#include <utility>

namespace NTC {
   /// таблица значений 10кОм характеристика R/T 2904 B25/100 4300K
   constexpr uint32_t resist_k2904[] {
      35563, 55, // от 0  -5.5 %/K
      27119, 53, // от 5  -5.3 %/K
      20860, 51, // от 10 -5.1 %/K
      16204, 50, // от 15 -5.0 %/K
      12683, 48, // от 20 -4.8 %/K
      10000, 47, // от 25 -4.7 %/K
       7942, 46, // от 30 -4.6 %/K
       6327, 45, // от 35 -4.5 %/K
       5074, 43, // от 40 -4.3 %/K
       4103, 42, // от 45 -4.2 %/K
       3336, 41, // от 50 -4.1 %/K
       2724, 40, // от 55 -4.0 %/K
       2237, 39, // от 60 -3.9 %/K
       1846, 38, // от 65 -3.8 %/K
       1531, 37, // от 70 -3.7 %/K
       1276, 36, // от 75 -3.6 %/K
       1068, 35, // от 80 -3.5 %/K
        899, 34, // от 85 -3.4 %/K
        761, 34, // от 90 -3.4 %/K
        645, 33  // от 95 -3.3 %/K
   };
   /// развёртывание таблицы до полного размера (градус на индекс от 0 до 99)
   template<size_t i, size_t j> struct Reisist2904 {
      static constexpr uint32_t resist = resist_k2904[2*i];
      static constexpr uint32_t k = resist_k2904[2*i+1];
      static constexpr uint32_t value = j == 0 ? resist : Reisist2904<i,j-1>::value*(1000-k)/1000;
   };
   template<size_t i> struct Reisist2904<i,0> {
      static constexpr uint32_t resist = resist_k2904[2*i];
      static constexpr uint32_t value = resist;
   };

   /// расчёт значений АЦП с дополнительныйм резистором 20кОм от 5 вольт
   /// 3.3V -> 4096, U в дециВольтах, R в омах
   template<size_t i, size_t j, size_t U = 50, size_t R = 20000>
   constexpr uint32_t t2904() 
   {
      return 4096 * U / 33 * Reisist2904<i,j>::value / (R + Reisist2904<i,j>::value);
   }

   /// таблица значений с АЦП, индекс - температура
   template<size_t U = 50, size_t R = 20000>
   constexpr uint32_t u2904[] = {
      t2904< 0,0,U,R>(), t2904< 0,1,U,R>(), t2904< 0,2,U,R>(), t2904< 0,3,U,R>(), t2904< 0,4,U,R>(),
      t2904< 1,0,U,R>(), t2904< 1,1,U,R>(), t2904< 1,2,U,R>(), t2904< 1,3,U,R>(), t2904< 1,4,U,R>(),
      t2904< 2,0,U,R>(), t2904< 2,1,U,R>(), t2904< 2,2,U,R>(), t2904< 2,3,U,R>(), t2904< 2,4,U,R>(),
      t2904< 3,0,U,R>(), t2904< 3,1,U,R>(), t2904< 3,2,U,R>(), t2904< 3,3,U,R>(), t2904< 3,4,U,R>(),
      t2904< 4,0,U,R>(), t2904< 4,1,U,R>(), t2904< 4,2,U,R>(), t2904< 4,3,U,R>(), t2904< 4,4,U,R>(),
      t2904< 5,0,U,R>(), t2904< 5,1,U,R>(), t2904< 5,2,U,R>(), t2904< 5,3,U,R>(), t2904< 5,4,U,R>(),
      t2904< 6,0,U,R>(), t2904< 6,1,U,R>(), t2904< 6,2,U,R>(), t2904< 6,3,U,R>(), t2904< 6,4,U,R>(),
      t2904< 7,0,U,R>(), t2904< 7,1,U,R>(), t2904< 7,2,U,R>(), t2904< 7,3,U,R>(), t2904< 7,4,U,R>(),
      t2904< 8,0,U,R>(), t2904< 8,1,U,R>(), t2904< 8,2,U,R>(), t2904< 8,3,U,R>(), t2904< 8,4,U,R>(),
      t2904< 9,0,U,R>(), t2904< 9,1,U,R>(), t2904< 9,2,U,R>(), t2904< 9,3,U,R>(), t2904< 9,4,U,R>(),
      t2904<10,0,U,R>(), t2904<10,1,U,R>(), t2904<10,2,U,R>(), t2904<10,3,U,R>(), t2904<10,4,U,R>(),
      t2904<11,0,U,R>(), t2904<11,1,U,R>(), t2904<11,2,U,R>(), t2904<11,3,U,R>(), t2904<11,4,U,R>(),
      t2904<12,0,U,R>(), t2904<12,1,U,R>(), t2904<12,2,U,R>(), t2904<12,3,U,R>(), t2904<12,4,U,R>(),
      t2904<13,0,U,R>(), t2904<13,1,U,R>(), t2904<13,2,U,R>(), t2904<13,3,U,R>(), t2904<13,4,U,R>(),
      t2904<14,0,U,R>(), t2904<14,1,U,R>(), t2904<14,2,U,R>(), t2904<14,3,U,R>(), t2904<14,4,U,R>(),
      t2904<15,0,U,R>(), t2904<15,1,U,R>(), t2904<15,2,U,R>(), t2904<15,3,U,R>(), t2904<15,4,U,R>(),
      t2904<16,0,U,R>(), t2904<16,1,U,R>(), t2904<16,2,U,R>(), t2904<16,3,U,R>(), t2904<16,4,U,R>(),
      t2904<17,0,U,R>(), t2904<17,1,U,R>(), t2904<17,2,U,R>(), t2904<17,3,U,R>(), t2904<17,4,U,R>(),
      t2904<18,0,U,R>(), t2904<18,1,U,R>(), t2904<18,2,U,R>(), t2904<18,3,U,R>(), t2904<18,4,U,R>(),
      t2904<19,0,U,R>(), t2904<19,1,U,R>(), t2904<19,2,U,R>(), t2904<19,3,U,R>(), t2904<19,4,U,R>()
   };

   static_assert (u2904<33,    0>[0] == 4096);
   static_assert (u2904<50,    0>[0] == 6206);
   static_assert (u2904<33,10000>[0] == 3197);
   static_assert (u2904<50,10000>[0] == 4843);

   static_assert (u2904<33,    0>[5] == 4096);
   static_assert (u2904<50,    0>[5] == 6206);
   static_assert (u2904<33,10000>[5] == 2992);
   static_assert (u2904<50,10000>[5] == 4534);

   static_assert (u2904<33,10000>[6] == 2948);
   static_assert (u2904<50,10000>[6] == 4466);

   static_assert (u2904<33,10000>[25]== 2048);

  //  static_assert (u2904<33, 5100>[0] == 4096);

} // namespace NTC {

namespace NTC_test {

// t - температура в градусах Цельсия
// тернарная в инстанцировании шаблона, чтобы не зациклить
template<size_t t>
constexpr size_t R2904_at =
   (t % 5) ?
        R2904_at<(t ? t-1 : t)> * (1000 - NTC::resist_k2904[t/5*2+1]) / 1000
      : NTC::resist_k2904[t/5*2];

template<size_t i>
struct R2904_at_f
{
   constexpr size_t operator()()
   {
      if constexpr (i % 5)
         return R2904_at_f<i-1>{}() * (1000 - NTC::resist_k2904[i/5*2+1]) / 1000;
      else
         return NTC::resist_k2904[i/5*2];
   }
};

static_assert (R2904_at<0> == 35563);
static_assert (R2904_at<1> == 33607);
static_assert (R2904_at<2> == 31758);
static_assert (R2904_at<3> == 30011);
static_assert (R2904_at<4> == 28360);
static_assert (R2904_at<5> == 27119);

template<template<size_t>class F, class index_sequence>
struct R2904_table_impl;

// template<size_t ... i>
// struct R2904_table_impl<std::index_sequence<i...>>
// {
//    static constexpr size_t table[] {R2904_at<i>...};
// };

template<template<size_t> class F, size_t ... i>
struct R2904_table_impl<F,std::index_sequence<i...>>
{
   static constexpr size_t table[] {F<i>{}()...};
};

using R2904 = R2904_table_impl<R2904_at_f, std::make_index_sequence<100>>;

static_assert (R2904::table[0] == 35563);
static_assert (R2904::table[1] == 33607);
static_assert (R2904::table[4] == 28360);
static_assert (R2904::table[5] == 27119);



} // namespace NTC {
