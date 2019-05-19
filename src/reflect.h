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
        // пока костыль, для больштх sizeof fatal error: template instantiation depth exceeds maximum of 900
        // return detail::member_count<T> (std::make_index_sequence<sizeof(T) + 64>{});
        return detail::member_count<T> (std::make_index_sequence<64>{});
    }
    template<class T> constexpr size_t member_count(T&)
    {
        return member_count<T>();
    }

  


    #define HELPER(n,...) \
        template <class T> \
        constexpr auto as_tuple (const T& t, std::enable_if_t<reflect::member_count<T>() == n, size_t> = n) \
        { \
            auto [__VA_ARGS__] = t; \
            return std::tuple {__VA_ARGS__}; \
        } \
        template <class T> \
        constexpr auto make_tie (T& t, std::enable_if_t<reflect::member_count<T>() == n, size_t> = n) \
        { \
            auto& [__VA_ARGS__] = t; \
            return std::tie (__VA_ARGS__); \
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
    HELPER(11,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11)
    HELPER(12,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12)
    HELPER(13,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13)
    HELPER(14,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14)
    HELPER(15,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15)
    HELPER(16,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16)
    HELPER(17,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16,n17)
    HELPER(18,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16,n17,n18)
    HELPER(19,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16,n17,n18,n19)
    HELPER(20,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16,n17,n18,n19,n20)
    HELPER(21,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16,n17,n18,n19,n20,n21)
    HELPER(22,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16,n17,n18,n19,n20,n21,n22)
    HELPER(23,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16,n17,n18,n19,n20,n21,n22,n23)
    HELPER(24,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16,n17,n18,n19,n20,n21,n22,n23,n24)
    HELPER(25,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16,n17,n18,n19,n20,n21,n22,n23,n24,n25)
    HELPER(26,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16,n17,n18,n19,n20,n21,n22,n23,n24,n25,n26)
    HELPER(27,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16,n17,n18,n19,n20,n21,n22,n23,n24,n25,n26,n27)
    HELPER(28,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16,n17,n18,n19,n20,n21,n22,n23,n24,n25,n26,n27,n28)
    HELPER(29,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16,n17,n18,n19,n20,n21,n22,n23,n24,n25,n26,n27,n28,n29)
    HELPER(30,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16,n17,n18,n19,n20,n21,n22,n23,n24,n25,n26,n27,n28,n29,n30)
    HELPER(31,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16,n17,n18,n19,n20,n21,n22,n23,n24,n25,n26,n27,n28,n29,n30,n31)
    HELPER(32,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16,n17,n18,n19,n20,n21,n22,n23,n24,n25,n26,n27,n28,n29,n30,n31,n32)
    HELPER(33,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16,n17,n18,n19,n20,n21,n22,n23,n24,n25,n26,n27,n28,n29,n30,n31,n32,n33)
    HELPER(34,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16,n17,n18,n19,n20,n21,n22,n23,n24,n25,n26,n27,n28,n29,n30,n31,n32,n33,n34)



    #undef HELPER

    template<size_t i, class T>
    struct get { using type = std::tuple_element_t<i, decltype(as_tuple(T{}))>; };


    template<size_t i, class T>
    using get_t = typename get<i,T>::type;






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
