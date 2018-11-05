// вытянуть из типа количество полей структуры
// вытянуть тип любого поля структуры по его положению в структуре

#pragma once

#include <utility>
#include <type_traits>
#include <tuple>


namespace reflect {

   template<class T> constexpr size_t member_count();
   template<class T> constexpr size_t member_count(T&);
   // template<size_t i, class T> using get_t // определена далее





   namespace detail {
      // неявно приводится к любому типу (непосредственно вызов не предполагается, только decltype)
      template<size_t> struct any_ctor { template<class T> constexpr operator T() const; };
      // приводится только к одному типу (непосредственно вызов не предполагается, только decltype)
      template<class T> struct only_type { constexpr operator T() const; };

      // если decltype не пройдёт, то функция не подходит, хотя более специализированная
      template <class T, size_t I0, size_t ... I, class = decltype(T{any_ctor<I>{}...})>
      constexpr auto member_count(std::index_sequence<I0, I...>)
      {
         return sizeof...(I);
      }

      template <class T, size_t ... I>
      constexpr auto member_count(std::index_sequence<I...>)
      {
         return member_count<T> (std::make_index_sequence<sizeof...(I) - 1>{});
      }
   } // namespace detail {


   template<class T> constexpr size_t member_count()
   {
      // +64 - для битовых полей
      return detail::member_count<T> (std::make_index_sequence<sizeof(T) + 64>{});
   }
   template<class T> constexpr size_t member_count(T&)
   {
      return member_count<T>();
   }

  

   namespace detail {

      #define HELPER(n,...) \
         template <class T> \
         constexpr auto as_tuple ( \
              T v \
            , std::enable_if_t<reflect::member_count<T>() == n, size_t> = n \
         ) { \
            auto [__VA_ARGS__] = v; \
            return std::tuple {__VA_ARGS__}; \
         }

      HELPER( 1,n1)
      HELPER( 2,n1,n2)
      HELPER( 3,n1,n2,n3)
      HELPER( 4,n1,n2,n3,n4)
      HELPER( 5,n1,n2,n3,n4,n5)
      HELPER( 6,n1,n2,n3,n4,n5,n6)
      HELPER( 7,n1,n2,n3,n4,n5,n6,n7)
      HELPER( 8,n1,n2,n3,n4,n5,n6,n7,n8)
      HELPER( 9,n1,n2,n3,n4,n5,n6,n7,n8,n9)
      HELPER(10,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10)

      #undef HELPER

      template<size_t i, class T>
      struct get {
         using type = std::remove_reference_t <
            decltype (
               std::get<i>(
                  as_tuple(T{})
               )
            )
         >;
      };
   } // namespace detail {

   template<size_t i, class T>
   using get_t = typename detail::get<i,T>::type;




   namespace test {
      struct Data1 {
         int16_t  d1 :1;
         uint16_t d2 :1;
      };
      struct Data2 {
         uint16_t d1 :1;
         int16_t  d2 :1;
      };
      struct test {
         int16_t d1;
         uint16_t d2;
         Data1 d3;
         Data2 d4;
      };

      static_assert (member_count<Data1>() == 2);
      static_assert (member_count<test>()  == 4);

      static_assert (std::is_same_v<get_t<0,test>, int16_t>);
      static_assert (std::is_same_v<get_t<1,test>, uint16_t>);
      static_assert (std::is_same_v<get_t<2,test>, Data1>);
      static_assert (std::is_same_v<get_t<3,test>, Data2>);
   } // namespace test 
} // namespace reflect {
