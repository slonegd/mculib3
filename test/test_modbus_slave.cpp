#define BOOST_TEST_MODULE test_uart
#include <boost/test/unit_test.hpp>

#define F_CPU   72000000UL
#define STM32F103xB
#define TEST


#include <iostream>
#include <type_traits>
#include "mock_uart.h"
#include "mock_interrupt.h"
#include "modbus_slave.h"

struct InReg{};
struct OutReg{};

auto modbus = mcu::Modbus_slave<InReg, OutReg>
                 ::make<mcu::Periph::TEST_USART, 
                   mcu::TX, mcu::RX, mcu::RTS, mcu::LED>();

BOOST_AUTO_TEST_SUITE (test_suite_main)

BOOST_AUTO_TEST_CASE (make)
{
    
}

BOOST_AUTO_TEST_SUITE_END()