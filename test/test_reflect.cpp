#define BOOST_TEST_MODULE test_reflect
#include <boost/test/unit_test.hpp>

#include "reflect.h"
#include <array>

BOOST_AUTO_TEST_SUITE (test_suite_main)

struct data0 {};
struct data1 {
   int d1;
};
struct data2 {
   int d1;
   bool d2;
};
struct data3 {
   data2 d1;
   int d2;
   bool d3;
};
struct data4 {
   data2 d1;
   int d2[2];
   bool d3;
};
struct data5 {
   data2 d1;
   int d2;
   int d3;
   bool d4;
   std::array<int,5> d5;
};
enum Enum {_1,_2,_3,_4};
struct data6 {
   int  d1 :2;
   bool d2 :1;
   Enum d3 :2;
   int  d4 :3;
   bool d5 :1;
   bool d6 :1;
};
struct data7 {
   data6 d1;
   int d2;
   bool d3;
   Enum d4;
   bool d5;
   bool d6;
   bool d7;
};
struct data8 {
   int16_t  d1;
   int16_t  d2;
   uint16_t d3;
   int8_t   d4;
   uint8_t  d5;
   uint32_t d6;
   int32_t  d7;
   uint64_t d8;
};
struct data9 {
   union {
      int16_t  d1;
      uint16_t d2;
   };
   int16_t  d3;
   uint16_t d4;
   int8_t   d5;
   data8    d6;
   data7    d7;
   data6    d8;
   data5    d9;
   data0    d10; // пустая структура
};

struct data10 {
   union Union {
      int16_t  du1;
      uint16_t du2;
   } du;
   data0 d0;
   data1 d1;
   data2 d2;
   data3 d3;
   data4 d4;
   data5 d5;
   data6 d6;
   data7 d7;
   data8 d8;
};


BOOST_AUTO_TEST_CASE (member_count)
{
   BOOST_CHECK_EQUAL (reflect::member_count<data0>(), 0);
   BOOST_CHECK_EQUAL (reflect::member_count<data1>(), 1);
   BOOST_CHECK_EQUAL (reflect::member_count<data2>(), 2);
   BOOST_CHECK_EQUAL (reflect::member_count<data3>(), 3);
   BOOST_CHECK_EQUAL (reflect::member_count<data4>(), 4);
   BOOST_CHECK_EQUAL (reflect::member_count<data5>(), 5);
   BOOST_CHECK_EQUAL (reflect::member_count<data6>(), 6);
   BOOST_CHECK_EQUAL (reflect::member_count<data7>(), 7);
   BOOST_CHECK_EQUAL (reflect::member_count<data8>(), 8);
   BOOST_CHECK_EQUAL (reflect::member_count<data9>(), 9);
   BOOST_CHECK_EQUAL (reflect::member_count<data10>(), 10);
}

BOOST_AUTO_TEST_CASE (get_t)
{
   bool res;
   res = std::is_same_v<reflect::get_t<0,data1>,int>  ; BOOST_CHECK (res);
 
   res = std::is_same_v<reflect::get_t<0,data2>,int>  ; BOOST_CHECK (res);
   res = std::is_same_v<reflect::get_t<1,data2>,bool> ; BOOST_CHECK (res);

   res = std::is_same_v<reflect::get_t<0,data3>,data2>; BOOST_CHECK (res);
   res = std::is_same_v<reflect::get_t<1,data3>,int>  ; BOOST_CHECK (res);
   res = std::is_same_v<reflect::get_t<2,data3>,bool> ; BOOST_CHECK (res);

   // ! dont work with arrays yet. use std::array
   // res = std::is_same_v<reflect::get_t<0,data4>,data2>; BOOST_CHECK (res);
   // res = std::is_same_v<reflect::get_t<1,data4>,int>  ; BOOST_CHECK (res);
   // res = std::is_same_v<reflect::get_t<2,data4>,int>  ; BOOST_CHECK (res);
   // res = std::is_same_v<reflect::get_t<3,data4>,bool> ; BOOST_CHECK (res);

   res = std::is_same_v<reflect::get_t<0,data5>,data2>; BOOST_CHECK (res);
   res = std::is_same_v<reflect::get_t<1,data5>,int>  ; BOOST_CHECK (res);
   res = std::is_same_v<reflect::get_t<2,data5>,int>  ; BOOST_CHECK (res);
   res = std::is_same_v<reflect::get_t<3,data5>,bool> ; BOOST_CHECK (res);
   res = std::is_same_v<reflect::get_t<4,data5>,std::array<int,5>> ; BOOST_CHECK (res);

   // ! dont work with bitsfield yet. 
   // res = std::is_same_v<reflect::get_t<0,data6>,int>  ; BOOST_CHECK (res);
   // res = std::is_same_v<reflect::get_t<1,data6>,bool> ; BOOST_CHECK (res);
   // res = std::is_same_v<reflect::get_t<2,data6>,Enum> ; BOOST_CHECK (res);
   // res = std::is_same_v<reflect::get_t<3,data6>,int>  ; BOOST_CHECK (res);
   // res = std::is_same_v<reflect::get_t<4,data6>,bool> ; BOOST_CHECK (res);
   // res = std::is_same_v<reflect::get_t<5,data6>,bool> ; BOOST_CHECK (res);

   res = std::is_same_v<reflect::get_t<0,data7>,data6> ; BOOST_CHECK (res);
   res = std::is_same_v<reflect::get_t<1,data7>,int>   ; BOOST_CHECK (res);
   res = std::is_same_v<reflect::get_t<2,data7>,bool>  ; BOOST_CHECK (res);
   res = std::is_same_v<reflect::get_t<3,data7>,Enum>  ; BOOST_CHECK (res);
   res = std::is_same_v<reflect::get_t<4,data7>,bool>  ; BOOST_CHECK (res);
   res = std::is_same_v<reflect::get_t<5,data7>,bool>  ; BOOST_CHECK (res);
   res = std::is_same_v<reflect::get_t<6,data7>,bool>  ; BOOST_CHECK (res);

   res = std::is_same_v<reflect::get_t<0,data8>,int16_t> ; BOOST_CHECK (res);
   res = std::is_same_v<reflect::get_t<1,data8>,int16_t> ; BOOST_CHECK (res);
   res = std::is_same_v<reflect::get_t<2,data8>,uint16_t>; BOOST_CHECK (res);
   res = std::is_same_v<reflect::get_t<3,data8>,int8_t>  ; BOOST_CHECK (res);
   res = std::is_same_v<reflect::get_t<4,data8>,uint8_t> ; BOOST_CHECK (res);
   res = std::is_same_v<reflect::get_t<5,data8>,uint32_t>; BOOST_CHECK (res);
   res = std::is_same_v<reflect::get_t<6,data8>,int32_t> ; BOOST_CHECK (res);
   res = std::is_same_v<reflect::get_t<7,data8>,uint64_t>; BOOST_CHECK (res);

   // ! dont work with anonimus union. 
   // res = std::is_same_v<reflect::get_t<0,data9>,int16_t> ; BOOST_CHECK (res);

   res = std::is_same_v<reflect::get_t<0,data10>,data10::Union> ; BOOST_CHECK (res);
   res = std::is_same_v<reflect::get_t<1,data10>,data0> ; BOOST_CHECK (res);
   res = std::is_same_v<reflect::get_t<2,data10>,data1>; BOOST_CHECK (res);
   res = std::is_same_v<reflect::get_t<3,data10>,data2>  ; BOOST_CHECK (res);
   res = std::is_same_v<reflect::get_t<4,data10>,data3> ; BOOST_CHECK (res);
   res = std::is_same_v<reflect::get_t<5,data10>,data4>; BOOST_CHECK (res);
   res = std::is_same_v<reflect::get_t<6,data10>,data5> ; BOOST_CHECK (res);
   res = std::is_same_v<reflect::get_t<7,data10>,data6>; BOOST_CHECK (res);
   res = std::is_same_v<reflect::get_t<8,data10>,data7>; BOOST_CHECK (res);
   res = std::is_same_v<reflect::get_t<9,data10>,data8>; BOOST_CHECK (res);

}

BOOST_AUTO_TEST_SUITE_END()
