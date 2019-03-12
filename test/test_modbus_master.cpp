#define BOOST_TEST_MODULE test_modbus_master
#include <boost/test/unit_test.hpp>

#define F_CPU   72000000UL
#define STM32F103xB
#define TEST


#include <iostream>
#include <type_traits>
#include "mock_systick.h"
#include "mock_interrupt.h"
#include "mock_uart.h"
#include "modbus_master.h"

using TX  = mcu::PA9;
using RX  = mcu::PA10;
using RTS = mcu::PA11;

auto& process = mock::Process::make();


BOOST_AUTO_TEST_SUITE (test_suite_main)

::UART::Settings set {false, ::UART::Parity::even,
                             ::UART::DataBits::_8,
                             ::UART::StopBits::_1,
                             ::UART::Baudrate::BR9600
};

Register<1, 2> temp;
Register<3, 7> uf;
Register<2, 4> time;

decltype(auto) master = make_modbus_master
    <mcu::Periph::USART1, TX, RX, RTS>(50, set, temp, time, uf);

decltype(auto) buffer = master.get_buffer();

auto passed_ms = [&](int n)
{
    while (n--) {
        SysTick_Handler(); 
        master();
    }
};

BOOST_AUTO_TEST_CASE(Register)
{
    BOOST_CHECK_EQUAL(temp.request[0], 1);
    BOOST_CHECK_EQUAL(temp.request[1], 3);
    BOOST_CHECK_EQUAL(temp.request[2], 0);
    BOOST_CHECK_EQUAL(temp.request[3], 2);
    BOOST_CHECK_EQUAL(temp.request[4], 0);
    BOOST_CHECK_EQUAL(temp.request[5], 1);
    BOOST_CHECK_EQUAL(temp.request[6], 0x25);
    BOOST_CHECK_EQUAL(temp.request[7], 0xCA);

    BOOST_CHECK_EQUAL(uf.request[0], 3);
    BOOST_CHECK_EQUAL(uf.request[1], 3);
    BOOST_CHECK_EQUAL(uf.request[2], 0);
    BOOST_CHECK_EQUAL(uf.request[3], 7);
    BOOST_CHECK_EQUAL(uf.request[4], 0);
    BOOST_CHECK_EQUAL(uf.request[5], 1);
    BOOST_CHECK_EQUAL(uf.request[6], 0x34);
    BOOST_CHECK_EQUAL(uf.request[7], 0x29);
}

BOOST_AUTO_TEST_CASE(make)
{
    BOOST_CHECK_EQUAL(process.str(),
        "Инициализация PA9(TX), PA10(RX), PA11(RTS), USART1"      "\n"
    );
}

BOOST_AUTO_TEST_CASE(search)
{
    process.clear();
    master();
    DMA1_Channel4_IRQHandler();
    BOOST_CHECK_EQUAL(process.str(),
        "Передача данных: 01 03 00 02 00 01 25 CA" "\n"
        "Очистка буфера, прием данных"             "\n"
    );

    passed_ms(5);
    USART1_IRQHandler();
    passed_ms(14);

    process.clear();
    master();
    DMA1_Channel4_IRQHandler ();
    BOOST_CHECK_EQUAL(process.str(),
        "Передача данных: 02 03 00 04 00 01 C5 F8" "\n"
        "Очистка буфера, прием данных"             "\n"
    );
    passed_ms(50);//second didn't answer
    passed_ms(10);//pause

    process.clear();
    master();
    DMA1_Channel4_IRQHandler ();
    BOOST_CHECK_EQUAL(process.str(),
        "Передача данных: 03 03 00 07 00 01 34 29" "\n"
        "Очистка буфера, прием данных"             "\n"
    );
    passed_ms(5);
    USART1_IRQHandler();
    passed_ms(14);

    BOOST_CHECK_EQUAL(master.qty_slave(), 2);
}

BOOST_AUTO_TEST_CASE(answer)
{   
    process.clear();
    master();
    DMA1_Channel4_IRQHandler ();
    BOOST_CHECK_EQUAL(process.str(),
        "Передача данных: 01 03 00 02 00 01 25 CA" "\n"
        "Очистка буфера, прием данных"             "\n"
    );

    uint8_t  address   = 1;
    uint8_t  function  = 3;
    uint8_t  qty_byte  = 2;
    uint16_t data      = 40;
    buffer << address;
    buffer << function;
    buffer << qty_byte;
    buffer << data;

    uint16_t crc = 0x5AB8;
    uint8_t crc_low = static_cast<uint8_t>(crc);
    uint8_t crc_high = crc >> 8;

    buffer << crc_low;
    buffer << crc_high;

    mock::CNDTR = 248;

    passed_ms(5);
    USART1_IRQHandler();
    passed_ms(14);

    BOOST_CHECK_EQUAL(temp.value, 40);

    process.clear();
    master();
    DMA1_Channel4_IRQHandler ();
    BOOST_CHECK_EQUAL(process.str(),
        "Передача данных: 03 03 00 07 00 01 34 29" "\n"
        "Очистка буфера, прием данных"             "\n"
    );

    address  = 3;
    function = 3;
    qty_byte = 2;
    data = 47194;

    buffer << address;
    buffer << function;
    buffer << qty_byte;
    buffer << data;

    crc = 0xBF33;
    crc_low = static_cast<uint8_t>(crc);
    crc_high = crc >> 8;

    buffer << crc_low;
    buffer << crc_high;

    mock::CNDTR = 248;

    passed_ms(5);
    USART1_IRQHandler();
    passed_ms(14);

    BOOST_CHECK_EQUAL(uf.value, 0xB85A);
}

BOOST_AUTO_TEST_CASE(error_addr)
{
    master();
    DMA1_Channel4_IRQHandler ();

	uint8_t address = 2;
    uint8_t function = 3;
    uint8_t qty = 2;
    uint16_t data = 40;
    buffer << address;
    buffer << function;
    buffer << qty;
    buffer << data;

    uint16_t crc = 0x5AB8;
    uint8_t crc_low = static_cast<uint8_t>(crc);
    uint8_t crc_high = crc >> 8;

    buffer << crc_low;
    buffer << crc_high;

    mock::CNDTR = 248;

    passed_ms(5);
    USART1_IRQHandler();
    passed_ms(14);

    BOOST_CHECK_EQUAL(static_cast<uint8_t>(temp.errors[0]), 9);
}

BOOST_AUTO_TEST_CASE(error_crc)
{
    master();
    DMA1_Channel4_IRQHandler ();

    uint8_t address  = 3;
    uint8_t function = 5;
    uint8_t qty_byte = 2;
    uint16_t data = 47194;

    buffer << address;
    buffer << function;
    buffer << qty_byte;
    buffer << data;

    uint16_t crc = 0xBF33;
    uint8_t crc_low = static_cast<uint8_t>(crc);
    uint8_t crc_high = crc >> 8;

    buffer << crc_low;
    buffer << crc_high;

    mock::CNDTR = 248;

    passed_ms(5);
    USART1_IRQHandler();
    passed_ms(14);

    BOOST_CHECK_EQUAL(static_cast<uint8_t>(uf.errors[0]), 0x10);
}

BOOST_AUTO_TEST_CASE(error_func)
{
    master();
    DMA1_Channel4_IRQHandler ();

    uint8_t address  = 1;
    uint8_t function = 5;
    uint8_t qty_byte = 2;
    uint16_t data = 40;

    buffer << address;
    buffer << function;
    buffer << qty_byte;
    buffer << data;

    uint16_t crc = 0xD2B8;
    uint8_t crc_low = static_cast<uint8_t>(crc);
    uint8_t crc_high = crc >> 8;

    buffer << crc_low;
    buffer << crc_high;

    mock::CNDTR = 248;

    passed_ms(5);
    USART1_IRQHandler();
    passed_ms(14);

    BOOST_CHECK_EQUAL(static_cast<uint8_t>(temp.errors[1]), 0x01);
}

BOOST_AUTO_TEST_CASE(error_qty_byte)
{
    master();
    DMA1_Channel4_IRQHandler ();

    uint8_t address  = 3;
    uint8_t function = 3;
    uint8_t qty_byte = 3;
    uint16_t data = 47194;

    buffer << address;
    buffer << function;
    buffer << qty_byte;
    buffer << data;

    uint16_t crc = 0x7F62;
    uint8_t crc_low = static_cast<uint8_t>(crc);
    uint8_t crc_high = crc >> 8;

    buffer << crc_low;
    buffer << crc_high;

    mock::CNDTR = 248;

    passed_ms(5);
    USART1_IRQHandler();
    passed_ms(14);

    BOOST_CHECK_EQUAL(static_cast<uint8_t>(uf.errors[1]), 0x11);
}

BOOST_AUTO_TEST_CASE(error_func_answer)
{
    master();
    DMA1_Channel4_IRQHandler ();

    uint8_t address  = 1;
    uint8_t function = 131;
    uint8_t error_code = 1;

    buffer << address;
    buffer << function;
    buffer << error_code;

    uint16_t crc = 0xF080;
    uint8_t crc_low = static_cast<uint8_t>(crc);
    uint8_t crc_high = crc >> 8;

    buffer << crc_low;
    buffer << crc_high;

    mock::CNDTR = 250;

    passed_ms(5);
    USART1_IRQHandler();
    passed_ms(14);

    BOOST_CHECK_EQUAL(static_cast<uint8_t>(temp.errors[2]), 0x01);
}

BOOST_AUTO_TEST_CASE(error_reg_answer)
{
    master();
    DMA1_Channel4_IRQHandler ();

    uint8_t address  = 3;
    uint8_t function = 131;
    uint8_t error_code = 3;

    buffer << address;
    buffer << function;
    buffer << error_code;

    uint16_t crc = 0xF1A0;
    uint8_t crc_low = static_cast<uint8_t>(crc);
    uint8_t crc_high = crc >> 8;

    buffer << crc_low;
    buffer << crc_high;

    mock::CNDTR = 250;

    passed_ms(5);
    USART1_IRQHandler();
    passed_ms(14);

    BOOST_CHECK_EQUAL(static_cast<uint8_t>(uf.errors[2]), 0x03);
}

BOOST_AUTO_TEST_CASE(error_time_out)
{
    master();
    DMA1_Channel4_IRQHandler ();
    passed_ms(60);

    BOOST_CHECK_EQUAL(static_cast<uint8_t>(temp.errors[3]), 0x12);
}

BOOST_AUTO_TEST_CASE(new_message)
{
    master();
    DMA1_Channel4_IRQHandler ();

	uint8_t address  = 3;
    uint8_t function = 131;
    uint8_t error_code = 3;

    buffer << address;
    buffer << function;
    buffer << error_code;

    mock::CNDTR = 252;

    passed_ms(5);
    USART1_IRQHandler();
    passed_ms(1);

    uint16_t crc = 0xF1A0;
    uint8_t crc_low = static_cast<uint8_t>(crc);
    uint8_t crc_high = crc >> 8;

    buffer << crc_low;
    buffer << crc_high;

    mock::CNDTR = 250;

    USART1_IRQHandler();
    passed_ms(14);

    BOOST_CHECK_EQUAL(static_cast<uint8_t>(uf.errors[3]), 0x03);
}

BOOST_AUTO_TEST_SUITE_END()