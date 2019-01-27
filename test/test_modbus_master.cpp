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
using RTS = mcu::PA15;
using LED = mcu::PC9;


BOOST_AUTO_TEST_SUITE (test_suite_main)

mock::UART::Settings set;
mcu::Register<1, 2> temp;
mcu::Register<3, 7> uf;

decltype(auto) master = mcu::make
    <mcu::Periph::USART1, TX, RX, RTS, LED>(50, set, temp, uf);

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

BOOST_AUTO_TEST_CASE(search)
{
    master();
    BOOST_CHECK_EQUAL(temp.request[0], buffer[0]);
    BOOST_CHECK_EQUAL(temp.request[1], buffer[1]);
    BOOST_CHECK_EQUAL(temp.request[2], buffer[2]);
    BOOST_CHECK_EQUAL(temp.request[3], buffer[3]);
    BOOST_CHECK_EQUAL(temp.request[4], buffer[4]);
    BOOST_CHECK_EQUAL(temp.request[5], buffer[5]);
    BOOST_CHECK_EQUAL(temp.request[6], buffer[6]);
    BOOST_CHECK_EQUAL(temp.request[7], buffer[7]);
    DMA1_Channel4_IRQHandler ();
    passed_ms(5);
    USART1_IRQHandler();
    passed_ms(14);

    master();
    BOOST_CHECK_EQUAL(uf.request[0], buffer[0]);
    BOOST_CHECK_EQUAL(uf.request[1], buffer[1]);
    BOOST_CHECK_EQUAL(uf.request[2], buffer[2]);
    BOOST_CHECK_EQUAL(uf.request[3], buffer[3]);
    BOOST_CHECK_EQUAL(uf.request[4], buffer[4]);
    BOOST_CHECK_EQUAL(uf.request[5], buffer[5]);
    BOOST_CHECK_EQUAL(uf.request[6], buffer[6]);
    BOOST_CHECK_EQUAL(uf.request[7], buffer[7]);

//     buffer.clear();

}

// BOOST_AUTO_TEST_CASE(answer)
// {
    
//     interrupt_usart1.clear_subscribe();
//     interrupt_DMA_channel4.clear_subscribe();

//     decltype(auto) master = mcu::make
//     <mcu::Periph::USART1, TX, RX, RTS, LED>(50, set, temp, uf);
//     decltype(auto) buffer = master.get_buffer();
    
    
//     master();
//     DMA1_Channel4_IRQHandler ();

//     uint8_t address = 1;
//     uint8_t function = 3;
//     uint8_t qty = 2;
//     uint16_t data = 40;
//     buffer << address;
//     buffer << function;
//     buffer << qty;
//     buffer << data;

//     uint16_t crc = 0x5AB8;
//     uint8_t crc_low = static_cast<uint8_t>(crc);
//     uint8_t crc_high = crc >> 8;

//     buffer << crc_low;
//     buffer << crc_high;

//     mock::CNDTR = 248;

//     passed_ms(5);
//     USART1_IRQHandler();
//     passed_ms(14);

//     BOOST_CHECK_EQUAL(temp.value, 40);

//     master();
//     DMA1_Channel4_IRQHandler ();

//     address = 3;
//     function = 3;
//     qty = 2;
//     data = 47194;

//     buffer << address;
//     buffer << function;
//     buffer << qty;
//     buffer << data;

//     crc = 0xBF33;
//     crc_low = static_cast<uint8_t>(crc);
//     crc_high = crc >> 8;

//     buffer << crc_low;
//     buffer << crc_high;

//     mock::CNDTR = 248;

//     passed_ms(5);
//     USART1_IRQHandler();
//     passed_ms(14);

//     BOOST_CHECK_EQUAL(uf.value, 0xB85A);
// }

// BOOST_AUTO_TEST_CASE(error_addr)
// {
//     decltype(auto) master = mcu::make
//     <mcu::Periph::USART1, TX, RX, RTS, LED>(50, set, temp, uf);
//     decltype(auto) buffer = master.get_buffer();
//     auto passed_ms = [&](int n){while (n--) {SysTick_Handler(); 
//                                              master();}};

//     master();
//     DMA1_Channel4_IRQHandler ();

// 	uint8_t address = 2;
//     uint8_t function = 3;
//     uint8_t qty = 2;
//     uint16_t data = 40;
//     buffer << address;
//     buffer << function;
//     buffer << qty;
//     buffer << data;

//     uint16_t crc = 0x5AB8;
//     uint8_t crc_low = static_cast<uint8_t>(crc);
//     uint8_t crc_high = crc >> 8;

//     buffer << crc_low;
//     buffer << crc_high;

//     mock::CNDTR = 248;

//     passed_ms(5);
//     USART1_IRQHandler();
//     passed_ms(14);

//     BOOST_CHECK_EQUAL(static_cast<uint8_t>(temp.errors[0]), 9);
// }

// BOOST_AUTO_TEST_CASE(error_crc)
// {
// 	mock::UART::Settings set; 
    
//     mcu::Register<1, 2> temp;
//     mcu::Register<3, 7> uf;

//     interrupt_usart1.clear_subscribe();
//     interrupt_DMA_channel4.clear_subscribe();

//     auto master = mcu::make
//     <mcu::Periph::USART1, TX, RX, RTS, LED>(50, set, temp, uf);
//     auto buffer = master.get_buffer();
//     auto passed_ms = [&](int n){while (n--) {SysTick_Handler(); master();}};

//     master();
//     DMA1_Channel4_IRQHandler ();

// 	buffer[0] = 1;
//     buffer[1] = 5;
//     buffer[2] = 2;
//     buffer[3] = 0;
//     buffer[4] = 40;

//     uint16_t crc = 0x5AB8;
//     uint8_t crc_low = static_cast<uint8_t>(crc);
//     uint8_t crc_high = crc >> 8;

//     buffer[5] = crc_low;
//     buffer[6] = crc_high;

//     mock::CNDTR = 248;

//     passed_ms(5);
//     USART1_IRQHandler();
//     passed_ms(14);

//     BOOST_CHECK_EQUAL(static_cast<uint8_t>(temp.errors[0]), 0x10);
// }

// BOOST_AUTO_TEST_CASE(error_func)
// {
// 	mock::UART::Settings set; 
    
//     mcu::Register<1, 2> temp;
//     mcu::Register<3, 7> uf;

//     interrupt_usart1.clear_subscribe();
//     interrupt_DMA_channel4.clear_subscribe();

//     auto master = mcu::make
//     <mcu::Periph::USART1, TX, RX, RTS, LED>(50, set, temp, uf);
//     auto buffer = master.get_buffer();
//     auto passed_ms = [&](int n){while (n--) {SysTick_Handler(); master();}};

//     master();
//     DMA1_Channel4_IRQHandler ();

// 	buffer[0] = 1;
//     buffer[1] = 5;
//     buffer[2] = 2;
//     buffer[3] = 0;
//     buffer[4] = 40;

//     uint16_t crc = 0xD2B8;
//     uint8_t crc_low = static_cast<uint8_t>(crc);
//     uint8_t crc_high = crc >> 8;

//     buffer[5] = crc_low;
//     buffer[6] = crc_high;

//     mock::CNDTR = 248;

//     passed_ms(5);
//     USART1_IRQHandler();
//     passed_ms(14);

//     BOOST_CHECK_EQUAL(static_cast<uint8_t>(temp.errors[0]), 0x01);
// }

// BOOST_AUTO_TEST_CASE(error_qty_byte)
// {
// 	mock::UART::Settings set; 
    
//     mcu::Register<1, 2> temp;

//     mcu::Register<3, 7> uf;

//     interrupt_usart1.clear_subscribe();
//     interrupt_DMA_channel4.clear_subscribe();

//     auto master = mcu::make
//     <mcu::Periph::USART1, TX, RX, RTS, LED>(50, set, temp, uf);
//     auto buffer = master.get_buffer();
//     auto passed_ms = [&](int n){while (n--) {SysTick_Handler(); master();}};

//     master();
//     DMA1_Channel4_IRQHandler ();

// 	buffer[0] = 1;
//     buffer[1] = 3;
//     buffer[2] = 4;
//     buffer[3] = 0;
//     buffer[4] = 40;

//     uint16_t crc = 0x5B58;
//     uint8_t crc_low = static_cast<uint8_t>(crc);
//     uint8_t crc_high = crc >> 8;

//     buffer[5] = crc_low;
//     buffer[6] = crc_high;

//     mock::CNDTR = 248;

//     passed_ms(5);
//     USART1_IRQHandler();
//     passed_ms(14);

//     BOOST_CHECK_EQUAL(static_cast<uint8_t>(temp.errors[0]), 0x11);
// }

// BOOST_AUTO_TEST_CASE(error_func_answer)
// {
// 	mock::UART::Settings set; 
    
//     mcu::Register<1, 2> temp;
//     mcu::Register<3, 7> uf;

//     interrupt_usart1.clear_subscribe();
//     interrupt_DMA_channel4.clear_subscribe();

//     auto master = mcu::make
//     <mcu::Periph::USART1, TX, RX, RTS, LED>(50, set, temp, uf);
//     auto buffer = master.get_buffer();
//     auto passed_ms = [&](int n){while (n--) {SysTick_Handler(); master();}};

//     master();
//     DMA1_Channel4_IRQHandler ();

// 	buffer[0] = 1;
//     buffer[1] = 131;
//     buffer[2] = 1;
    

//     uint16_t crc = 0xF080;
//     uint8_t crc_low = static_cast<uint8_t>(crc);
//     uint8_t crc_high = crc >> 8;

//     buffer[3] = crc_low;
//     buffer[4] = crc_high;

//     mock::CNDTR = 250;

//     passed_ms(5);
//     USART1_IRQHandler();
//     passed_ms(14);

//     BOOST_CHECK_EQUAL(static_cast<uint8_t>(temp.errors[0]), 0x01);
// }

// BOOST_AUTO_TEST_CASE(error_reg_answer)
// {
// 	mock::UART::Settings set; 
    
//     mcu::Register<1, 2> temp;
//     mcu::Register<3, 7> uf;

//     interrupt_usart1.clear_subscribe();
//     interrupt_DMA_channel4.clear_subscribe();

//     auto master = mcu::make
//     <mcu::Periph::USART1, TX, RX, RTS, LED>(50, set, temp, uf);
//     auto buffer = master.get_buffer();
//     auto passed_ms = [&](int n){while (n--) {SysTick_Handler(); master();}};

//     master();
//     DMA1_Channel4_IRQHandler ();

// 	buffer[0] = 1;
//     buffer[1] = 131;
//     buffer[2] = 3;
    
//     uint16_t crc = 0x3101 ;
//     uint8_t crc_low = static_cast<uint8_t>(crc);
//     uint8_t crc_high = crc >> 8;

//     buffer[3] = crc_low;
//     buffer[4] = crc_high;

//     mock::CNDTR = 250;

//     passed_ms(5);
//     USART1_IRQHandler();
//     passed_ms(14);

//     BOOST_CHECK_EQUAL(static_cast<uint8_t>(temp.errors[0]), 0x03);
// }

// BOOST_AUTO_TEST_CASE(error_reg_answer_both)
// {
// 	mock::UART::Settings set; 
    
//     mcu::Register<1, 2> temp;
//     mcu::Register<3, 7> uf;

//     interrupt_usart1.clear_subscribe();
//     interrupt_DMA_channel4.clear_subscribe();

//     auto master = mcu::make
//     <mcu::Periph::USART1, TX, RX, RTS, LED>(50, set, temp, uf);
//     auto buffer = master.get_buffer();
//     auto passed_ms = [&](int n){while (n--) {SysTick_Handler(); master();}};

//     master();
//     DMA1_Channel4_IRQHandler ();

// 	buffer[0] = 1;
//     buffer[1] = 131;
//     buffer[2] = 3;

//     uint16_t crc = 0x3101 ;
//     uint8_t crc_low = static_cast<uint8_t>(crc);
//     uint8_t crc_high = crc >> 8;

//     buffer[3] = crc_low;
//     buffer[4] = crc_high;

//     mock::CNDTR = 250;

//     passed_ms(5);
//     USART1_IRQHandler();
//     passed_ms(14);

//     BOOST_CHECK_EQUAL(static_cast<uint8_t>(temp.errors[0]), 0x03);

//     master();
//     DMA1_Channel4_IRQHandler ();

// 	buffer[0] = 3;
//     buffer[1] = 131;
//     buffer[2] = 3;
    

//     crc = 0xF1A0;
//     crc_low = static_cast<uint8_t>(crc);
//     crc_high = crc >> 8;

//     buffer[3] = crc_low;
//     buffer[4] = crc_high;

//     mock::CNDTR = 250;

//     passed_ms(5);
//     USART1_IRQHandler();
//     passed_ms(14);

//     BOOST_CHECK_EQUAL(static_cast<uint8_t>(uf.errors[0]), 0x03);
// }

// BOOST_AUTO_TEST_CASE(error_reg_func_answer_twice)
// {
// 	mock::UART::Settings set; 
    
//     mcu::Register<1, 2> temp;
//     mcu::Register<3, 7> uf;

//     interrupt_usart1.clear_subscribe();
//     interrupt_DMA_channel4.clear_subscribe();

//     auto master = mcu::make
//     <mcu::Periph::USART1, TX, RX, RTS, LED>(50, set, temp, uf);
//     auto buffer = master.get_buffer();
//     auto passed_ms = [&](int n){while (n--) {SysTick_Handler(); master();}};

//     master();
//     DMA1_Channel4_IRQHandler ();

// 	buffer[0] = 1;
//     buffer[1] = 131;
//     buffer[2] = 3;
    
//     uint16_t crc = 0x3101 ;
//     uint8_t crc_low = static_cast<uint8_t>(crc);
//     uint8_t crc_high = crc >> 8;

//     buffer[3] = crc_low;
//     buffer[4] = crc_high;

//     mock::CNDTR = 250;

//     passed_ms(5);
//     USART1_IRQHandler();
//     passed_ms(14);

//     BOOST_CHECK_EQUAL(static_cast<uint8_t>(temp.errors[0]), 0x03);

//     master();
//     DMA1_Channel4_IRQHandler ();

// 	buffer[0] = 3;
//     buffer[1] = 131;
//     buffer[2] = 3;
    

//     crc = 0xF1A0;
//     crc_low = static_cast<uint8_t>(crc);
//     crc_high = crc >> 8;

//     buffer[3] = crc_low;
//     buffer[4] = crc_high;

//     mock::CNDTR = 250;

//     passed_ms(5);
//     USART1_IRQHandler();
//     passed_ms(14);

//     BOOST_CHECK_EQUAL(static_cast<uint8_t>(uf.errors[0]), 0x03);

//     master();
//     DMA1_Channel4_IRQHandler ();

// 	buffer[0] = 1;
//     buffer[1] = 131;
//     buffer[2] = 1;
    
//     crc = 0xF080;
//     crc_low = static_cast<uint8_t>(crc);
//     crc_high = crc >> 8;

//     buffer[3] = crc_low;
//     buffer[4] = crc_high;

//     mock::CNDTR = 250;

//     passed_ms(5);
//     USART1_IRQHandler();
//     passed_ms(14);

//     BOOST_CHECK_EQUAL(static_cast<uint8_t>(temp.errors[1]), 0x01);
// }

// BOOST_AUTO_TEST_CASE(error_time_out)
// {
// 	mock::UART::Settings set; 
    
//     mcu::Register<1, 2> temp;
//     mcu::Register<3, 7> uf;

//     interrupt_usart1.clear_subscribe();
//     interrupt_DMA_channel4.clear_subscribe();

//     auto master = mcu::make
//     <mcu::Periph::USART1, TX, RX, RTS, LED>(50, set, temp, uf);
//     auto buffer = master.get_buffer();
//     auto passed_ms = [&](int n){while (n--) {SysTick_Handler(); master();}};

//     master();
//     DMA1_Channel4_IRQHandler ();
//     passed_ms(60);

//     BOOST_CHECK_EQUAL(static_cast<uint8_t>(temp.errors[0]), 0x12);
// }

// BOOST_AUTO_TEST_CASE(constantly_new_message)
// {
// 	mock::UART::Settings set; 
    
//     mcu::Register<1, 2> temp;

//     mcu::Register<3, 7> uf;

//     interrupt_usart1.clear_subscribe();
//     interrupt_DMA_channel4.clear_subscribe();

//     auto master = mcu::make
//     <mcu::Periph::USART1, TX, RX, RTS, LED>(50, set, temp, uf);
//     auto buffer = master.get_buffer();
//     auto passed_ms = [&](int n)
//     {
//         while (n--) { 
//         SysTick_Handler(); 
//         master();
//         }
//     };

//     master();
//     DMA1_Channel4_IRQHandler ();
//     passed_ms(2);
//     mock::CNDTR = 240;
//     passed_ms(58);

//     BOOST_CHECK_EQUAL(static_cast<uint8_t>(temp.errors[0]), 0x12);

//     mock::CNDTR = 255;
// }

// BOOST_AUTO_TEST_CASE(new_message)
// {
// 	mock::UART::Settings set; 
    
//     mcu::Register<1, 2> temp;
//     mcu::Register<3, 7> uf;

//     interrupt_usart1.clear_subscribe();
//     interrupt_DMA_channel4.clear_subscribe();

//     auto master = mcu::make
//     <mcu::Periph::USART1, TX, RX, RTS, LED>(50, set, temp, uf);
//     auto buffer = master.get_buffer();
//     auto passed_ms = [&](int n)
//     {
//         while (n--) {
//             SysTick_Handler(); 
//             master();
//         }
//     };

//     master();
//     DMA1_Channel4_IRQHandler ();

// 	buffer[0] = 1;
//     buffer[1] = 131;
//     buffer[2] = 3;

//     mock::CNDTR = 252;

//     passed_ms(5);
//     USART1_IRQHandler();
//     passed_ms(1);

//     uint16_t crc = 0x3101 ;
//     uint8_t crc_low = static_cast<uint8_t>(crc);
//     uint8_t crc_high = crc >> 8;
    
//     buffer[3] = crc_low;
//     buffer[4] = crc_high;

//     mock::CNDTR = 250;

//     USART1_IRQHandler();
//     passed_ms(14);

//     BOOST_CHECK_EQUAL(static_cast<uint8_t>(temp.errors[0]), 0x03);
// }

BOOST_AUTO_TEST_SUITE_END()