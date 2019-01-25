#define BOOST_TEST_MODULE test_net_buffer
#include <boost/test/unit_test.hpp>

#include "vector.h"
#include "ring_buffer.h"
#include "table_crc.h"

using Array = std::array<uint8_t, 8>;

struct Register_base {

	enum class Error_code : uint8_t {
		wrong_func = 0x01,  wrong_reg = 0x02, 
		wrong_value = 0x03, wrong_addr = 0x09, 
		wrong_crc = 0x10,   wrong_qty_byte = 0x11, 
		time_out = 0x12
	};

   virtual       uint8_t  get_adr    () = 0;
   virtual       uint16_t get_adr_reg() = 0;
   virtual       void     set        (uint16_t data) = 0;
	// virtual       uint16_t operator=  (uint16_t v) = 0;
   virtual const Array get_request   () = 0;

	RingBuffer<10, Error_code> errors;
};

template<uint8_t address_, uint16_t address_register_, class T = uint16_t>
struct Register : Register_base
{
   static constexpr uint8_t address = address_;
   uint8_t get_adr() override {return address;}
   static constexpr uint16_t address_register = address_register_;
   uint16_t get_adr_reg() override {return address_register_;}
   T value;
   operator T() {return value;}
	// uint16_t operator= (uint16_t v) override {this -> value = v; return value;}
   void set(uint16_t data) override {(*reinterpret_cast<uint16_t*>(&value)) = data;}
   static constexpr  uint8_t request_base[]
		{ address_
		, 3
		, static_cast<uint8_t>(address_register_ << 8)
		, static_cast<uint8_t>(address_register_)
		, 0
		, 1
		};

	static constexpr Array request
		{ address_
		, 3
		, static_cast<uint8_t>(address_register_ << 8)
		, static_cast<uint8_t>(address_register_)
		, 0
		, 1
		, std::get<0>(CRC16(std::cbegin(request_base), std::cend(request_base)))
		, std::get<1>(CRC16(std::cbegin(request_base), std::cend(request_base)))
		};

   const Array get_request() override {return request;}

};

Register<1, 2> temp;
Register<3, 7> uf;

BOOST_AUTO_TEST_SUITE (test_suite_main)

BOOST_AUTO_TEST_CASE (ctor)
{
   auto vector = Vector<Register_base*, 2>{};
   BOOST_CHECK_EQUAL (vector.size(), 0);
   BOOST_CHECK_EQUAL (vector.begin(), vector.end());
}

BOOST_AUTO_TEST_CASE (push_back)
{
   auto vector = Vector<Register_base*, 2>{};
   vector.push_back(&temp);
   BOOST_CHECK_EQUAL (vector.size(), 1);
}

BOOST_AUTO_TEST_CASE (erase)
{
   auto vector = Vector<Register_base*, 2>{};
   vector.push_back(&temp);
   vector.erase(&temp);
   BOOST_CHECK_EQUAL (vector.size(), 0);
}

BOOST_AUTO_TEST_SUITE_END()