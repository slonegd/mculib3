#define BOOST_TEST_MODULE f1_test_uart
#include <boost/test/unit_test.hpp>

#define F_CPU   72'000'000UL
#define STM32F103xB

#include "mock_interrupt.h"
#include "mock_rcc.h"
#include "mock_afio.h"
#include "mock_gpio.h"
#include "mock_dma.h"
#include "mock_periph_usart.h"
#include "uart.h"
#include <type_traits>
#include <string>


auto& process = mock::Process::make();


BOOST_AUTO_TEST_SUITE (test_suite_main)

BOOST_AUTO_TEST_CASE (make)
{
   process.clear();

   auto& uart = UART::make <
        mcu::Periph::USART1
      , mcu::PA9
      , mcu::PA10
      , mcu::PA11
      , mcu::PA12
   >();

   BOOST_CHECK_EQUAL ( process.str(),
      "включение тактирования GPIOA\n"
      "инициализация вывода 9 порта GPIOA в режиме USART1_TX\n"
      "включение тактирования AFIO\n"
      "включение тактирования GPIOA\n"
      "инициализация вывода 10 порта GPIOA в режиме USART1_RX\n"
      "включение тактирования AFIO\n"
      "включение тактирования GPIOA\n"
      "инициализация вывода 11 порта GPIOA в режиме Output\n"
      "включение тактирования GPIOA\n"
      "инициализация вывода 12 порта GPIOA в режиме Output\n"
      "включение тактирования USART1\n"
      "USART1: Разрешение отправки\n"
      "USART1: Разрешение приёма\n"
      "USART1: Разрешение DMA на отправку\n"
      "USART1: Разрешение DMA на приём\n"
      "USART1: Разрешение прерывания по простою\n"
      "USART1: Разрешение работы\n"
      "NVIC: включение прерывания USART1\n"
      "включение тактирования DMA1\n"
      "DMA1_stream4: Установка направления из памяти в переферию\n"
      "DMA1_stream4: Установка адреса памяти: " + std::to_string(size_t(uart.buffer.begin())) + "\n"
      "DMA1_stream4: Установка адреса переферии: " + std::to_string(size_t(&usart1.like_CMSIS().DR)) + "\n"
      "DMA1_stream4: Установка инкремента адреса памяти\n"
      "DMA1_stream4: Установка размера данных в памяти байт (8бит)\n"
      "DMA1_stream4: Установка размера данных в переферии байт (8бит)\n"
      "DMA1_stream4: Разрешение прерывания по концу передачи данных\n"
      "NVIC: включение прерывания DMA1_Channel4\n"
      "DMA1_stream5: Установка направления из переферии в память\n"
      "DMA1_stream5: Установка адреса памяти: " + std::to_string(size_t(uart.buffer.begin())) + "\n"
      "DMA1_stream5: Установка адреса переферии: " + std::to_string(size_t(&usart1.like_CMSIS().DR)) + "\n"
      "DMA1_stream5: Установка количества передач данных: 255\n"
      "DMA1_stream5: Установка инкремента адреса памяти\n"
      "DMA1_stream5: Установка размера данных в памяти байт (8бит)\n"
      "DMA1_stream5: Установка размера данных в переферии байт (8бит)\n"
   );
}

BOOST_AUTO_TEST_CASE(init)
{
   auto& uart = UART::make <
        mcu::Periph::USART1
      , mcu::PA9
      , mcu::PA10
      , mcu::PA11
      , mcu::PA12
   >();

   process.clear();

   UART::Settings set {
      .parity_enable = false,
      .parity        = UART::Parity::even,
      .data_bits     = UART::DataBits::_8,
      .stop_bits     = UART::StopBits::_1,
      .baudrate      = UART::Baudrate::BR9600,
      .res           = 0
   };
   uart.init(set);

   BOOST_CHECK_EQUAL (process.str(),
      "USART1: Установлена скорость 9600 бит/с\n"
      "USART1: Задана проверка на четность\n"
      "USART1: Установлен размер пакета 8 бит\n"
      "USART1: Установлено количество стоп битов: 1\n"
   );

   process.clear();
   set.baudrate = UART::Baudrate::BR14400;
   uart.init(set);
   BOOST_CHECK_EQUAL (process.str(),
      "USART1: Установлена скорость 14400 бит/с\n"
      "USART1: Задана проверка на четность\n"
      "USART1: Установлен размер пакета 8 бит\n"
      "USART1: Установлено количество стоп битов: 1\n"
   );

   process.clear();
   set.parity = UART::Parity::odd;
   uart.init(set);
   BOOST_CHECK_EQUAL (process.str(),
      "USART1: Установлена скорость 14400 бит/с\n"
      "USART1: Задана проверка на нечетность\n"
      "USART1: Установлен размер пакета 8 бит\n"
      "USART1: Установлено количество стоп битов: 1\n"
   );
}

BOOST_AUTO_TEST_CASE(transmit)
{
   auto& uart = UART::make <
        mcu::Periph::USART1
      , mcu::PA9
      , mcu::PA10
      , mcu::PA11
      , mcu::PA12
   >();

   process.clear();
   
   uart.buffer << uint8_t(1) << uint8_t(2) << uint8_t(3);
   uart.transmit();
   BOOST_CHECK_EQUAL (process.str(),
      "установка вывода 12 порта GPIOA\n"
      "установка вывода 11 порта GPIOA\n"
      "DMA1_stream5: Запрет работы\n"
      "DMA1_stream4: Запрет работы\n"
      "DMA1_stream4: Установка количества передач данных: 3\n"
      "DMA1_stream4: Разрешение работы\n"
   );

}

BOOST_AUTO_TEST_CASE(start_receive)
{
   auto& uart = UART::make <
        mcu::Periph::USART1
      , mcu::PA9
      , mcu::PA10
      , mcu::PA11
      , mcu::PA12
   >();

   process.clear();

   uart.receive();
   BOOST_CHECK_EQUAL (process.str(),
      "сброс вывода 12 порта GPIOA\n"
      "сброс вывода 11 порта GPIOA\n"
      "DMA1_stream4: Запрет работы\n"
      "DMA1_stream5: Запрет работы\n"
      "DMA1_stream5: Разрешение работы\n"
   );
}

BOOST_AUTO_TEST_SUITE_END()

