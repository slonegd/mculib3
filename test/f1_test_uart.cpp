#define BOOST_TEST_MODULE f1_test_uart
#include <boost/test/unit_test.hpp>

#define F_CPU   72'000'000UL
#define STM32F103xB


#include "mock_rcc.h"
#include "mock_afio.h"
#include "mock_gpio.h"
#include "mock_dma.h"
#include "mock_usart.h"
#include "uart.h"
#include <type_traits>

std::stringstream process{};
auto& rcc   = mcu::make_reference<mcu::Periph::RCC>();
auto& gpioa = mcu::make_reference<mcu::Periph::GPIOA>();
auto& gpiob = mcu::make_reference<mcu::Periph::GPIOB>();
auto& gpioc = mcu::make_reference<mcu::Periph::GPIOC>();
#if defined(STM32F1)
auto& afio = mcu::make_reference<mcu::Periph::AFIO>();
#endif

auto set_stream = [&](){
     rcc.set_stream (process);
   gpioa.set_stream (process);
   gpiob.set_stream (process);
   gpioc.set_stream (process);
#if defined(STM32F1)
    afio.set_stream (process);
#endif
};
auto clear_stream = [&](){ process.str(std::string{}); };








BOOST_AUTO_TEST_SUITE (test_suite_main)

BOOST_AUTO_TEST_CASE (make)
{
   set_stream();
   clear_stream();

   auto uart = UART::make <
        mcu::Periph::USART1
      , mcu::PA9
      , mcu::PA10
      , mcu::PA11
      , mcu::PA12
   >();

   BOOST_CHECK_EQUAL ( process.str(),
      // методы объекта usart
      "Проверяем назначение пинов"                          "\n"
      "Создали ссылку на переферию usart"                   "\n"
      "Создали ссылку на переферию tx stream"               "\n"
      "Создали ссылку на переферию rx stream"               "\n"
      "Передаем значение clock"                             "\n"
      "создали пин TX в альтернативном режиме"              "\n"
      "создали пин RX в альтернативном режиме"              "\n"
      "создали пин RTS в режиме выхода"                     "\n"
      "создали пин LED в режиме выхода"                     "\n"
      "Включаем тактирование переферии usart"               "\n"
      "USART Transmitter enable"                            "\n"
      "USART Receiver enable"                               "\n"
      "DMA enable transmitter"                              "\n"
      "DMA enable receiver"                                 "\n"
      "USART IDLE interrupt enable"                         "\n"
      "USART enable"                                        "\n"
      "Определили номер прерывания USART"                   "\n"
      "Включили прерывание переферии USART"                 "\n"
      // методы объекта TXstrteam
      "Включаем тактирование переферии DMA"                 "\n" 
      "Запрещаем работу TX_stream"                          "\n"
      "Задаем направление TX_stream в переферию"            "\n"
      "Задаем адрес памяти TX_stream"                       "\n"
      "Передаем адрес регистра данных"                      "\n"
      "Задаем адрес переферии TX_stream"                    "\n"
      "Инкременнтируем адрес памяти TX_stream"              "\n"
      "Задаем размер для памяти TX_stream 8 бит"            "\n"
      "Задаем размер для переферии TX_stream 8 бит"         "\n"
      "Разрешаем прерывание TX_stream по окончании отправки""\n"
      "Определяем номер прерывания TX_stream"                "\n"
      "Включили прерывание переферии DMA TX_stream"         "\n"
      // методы объекта RXstrteam
      "Задаем направление RX_stream в память"               "\n"
      "Задаем адрес памяти RX_stream"                       "\n"
      "Передаем адрес регистра данных"                      "\n"
      "Задаем адрес переферии RX_stream"                    "\n"
      "Устанавливаем колв-во транзакций для RX_stream"      "\n"
      "Инкременнтируем адрес памяти RX_stream"              "\n"
      "Задаем размер для памяти RX_stream 8 бит"            "\n"
      "Задаем размер для переферии RX_stream 8 бит"         "\n"
   );
}
/*
BOOST_AUTO_TEST_CASE(init)
{
   mcu::UART::Settings set {
      .parity_enable = false,
      .parity        = mcu::UART::Parity::even,
      .data_bits     = mcu::UART::DataBits::_8,
      .stop_bits     = mcu::UART::StopBits::_1,
      .baudrate      = mcu::UART::Baudrate::BR9600,
      .res           = 0
   };
   result.str("");
   uart.init(set);
   BOOST_CHECK_EQUAL (result.str(),
      "Установлена скорость 9600 бит/с""\n"
      "Задана проверка на четность"    "\n"
      "Установлен размер пакета 8 бит" "\n"
      "УСтановлен один стоповый бит"   "\n"
   );

   result.str("");
   set.baudrate = mcu::UART::Baudrate::BR14400;
   uart.init(set);
   BOOST_CHECK_EQUAL (result.str(),
      "Установлена скорость 14400 бит/с""\n"
      "Задана проверка на четность"     "\n"
      "Установлен размер пакета 8 бит"  "\n"
      "УСтановлен один стоповый бит"    "\n"
   );

   result.str("");
   set.parity = mcu::UART::Parity::odd;
   uart.init(set);
   BOOST_CHECK_EQUAL (result.str(),
      "Установлена скорость 14400 бит/с""\n"
      "Задана проверка на нечетность"   "\n"
      "Установлен размер пакета 8 бит"  "\n"
      "УСтановлен один стоповый бит"    "\n"
   );
}

BOOST_AUTO_TEST_CASE(transmit)
{
   result.str("");
   uart.transmit(qty);
   BOOST_CHECK_EQUAL (result.str(),
      "устанавливаем значение пина LED true"          "\n"
      "устанавливаем значение пина RTS true"          "\n"
      "Запрещаем работу RX_stream"                    "\n"
      "Запрещаем работу TX_stream"                    "\n"
      "Устанавливаем колв-во транзакций для TX_stream""\n"
      "Разрешаем работу TX_stream"                    "\n"
   );

}

BOOST_AUTO_TEST_CASE(start_receive)
{
   result.str("");
   uart.start_receive();
   BOOST_CHECK_EQUAL (result.str(),
      "устанавливаем значение пина LED false" "\n"
      "устанавливаем значение пина RTS false" "\n"
      "Запрещаем работу TX_stream"            "\n"
      "Запрещаем работу RX_stream"            "\n"
      "Разрешаем работу RX_stream"            "\n"
   ); 
}

BOOST_AUTO_TEST_CASE(interrupt)
{
   uint message_size = uart.message_size();
   USART1_IRQHandler();
}
*/
BOOST_AUTO_TEST_SUITE_END()

