#define BOOST_TEST_MODULE test_uart
#include <boost/test/unit_test.hpp>

#define F_CPU   72000000UL
#define STM32F103xB
#define TEST


#include <iostream>
#include <type_traits>
#include "mock_for_test.h"
#include "uart.h"

auto uart = mcu::UART::make <
        mcu::Periph::TEST_USART
      , mcu::TX
      , mcu::RX
      , mcu::RTS
      , mcu::LED
   >();

mcu::UART::Settings set {
      .parity_enable = false,
      .parity        = mcu::UART::Parity::even,
      .data_bits     = mcu::UART::DataBits::_8,
      .stop_bits     = mcu::UART::StopBits::_1,
      .baudrate      = mcu::UART::Baudrate::BR9600,
      .res           = 0
   };

BOOST_AUTO_TEST_SUITE (test_suite_main)



BOOST_AUTO_TEST_CASE (make)
{
   BOOST_CHECK_EQUAL ( result.str(),
   // make()
      // методы объекта usart
      "Проверяем назначение пинов"            "\n"
      "Создали ссылку на переферию usart"     "\n"
      "Создали ссылку на переферию tx stream" "\n"
      "Создали ссылку на переферию rx stream" "\n"
      "Передаем значение clock"               "\n"
      "создали пин TX"                        "\n"
      "создали пин RX"                        "\n"
      "создали пин RTS"                       "\n"
      "создали пин LED"                       "\n"
      "Включаем тактирование переферии usart" "\n"
      "USART Transmitter enable"              "\n"
      "USART Receiver enable"                 "\n"
      "DMA enable transmitter"                "\n"
      "DMA enable receiver"                   "\n"
      "USART IDLE interrupt enable"           "\n"
      "USART enable"                          "\n"
      "Определили номер прерывания USART"     "\n"
      "Включили прерывание переферии"         "\n"
      // методы объекта TXstrteam
      "Включаем тактирование переферии DMA"   "\n"
      "Запрещаем работу DMA потока"           "\n"
      "Задаем направление DMA в переферию"    "\n"
      "Задаем адрес памяти"                   "\n"
      "Передаем адрес регистра данных"        "\n"
      "Задаем адрес переферии"                "\n"
      "Инкременнтируем адрес памяти"          "\n"
      "Задаем размер для памяти 8 бит"        "\n"
      "Задаем размер для переферии 8 бит"     "\n"
      "Разрешаем прерывание по отправке"      "\n"
      "Определяем номер прерывания DMA потока""\n"
      "Включили прерывание переферии"         "\n"
      // методы объекта RXstrteam
      "Включаем тактирование переферии DMA"   "\n"
      "Задаем направление DMA в память"       "\n"
      "Задаем адрес памяти"                   "\n"
      "Передаем адрес регистра данных"        "\n"
      "Задаем адрес переферии"                "\n"
      "Устанавливаем колв-во транзакций"      "\n"
      "Инкременнтируем адрес памяти"          "\n"
      "Задаем размер для памяти 8 бит"        "\n"
      "Задаем размер для переферии 8 бит"     "\n"
   );
}

BOOST_AUTO_TEST_CASE(init)
{
   result.str("");
   uart.init(set);
   BOOST_CHECK_EQUAL (result.str(),
      "Установлена скорость 9600 бит/с""\n"
      "Задана проверка на четность"    "\n"
      "Установлен размер пакета 8 бит" "\n"
      "УСтановлен один стоповый бит"   "\n"
   );
}

BOOST_AUTO_TEST_CASE(transmit)
{
   result.str("");
   uart.transmit(2);
   BOOST_CHECK_EQUAL (result.str(),
      "устанавливаем значение пина true" "\n"
      "устанавливаем значение пина true" "\n"
      "Запрещаем работу DMA потока"      "\n"
      "Запрещаем работу DMA потока"      "\n"
      "Устанавливаем колв-во транзакций" "\n"
      "Разрешаем работу DMA потока"      "\n"
   );

}

BOOST_AUTO_TEST_CASE(start_receive)
{
   result.str("");
   uart.start_receive(buffer);
   BOOST_CHECK_EQUAL (result.str(),
      "устанавливаем значение пина fasle" "\n"
      "устанавливаем значение пина fasle" "\n"
      "Запрещаем работу DMA потока"       "\n"
      "Запрещаем работу DMA потока"       "\n"
      "Разрешаем работу DMA потока"       "\n"
   ); 
}

BOOST_AUTO_TEST_SUITE_END()

