#define BOOST_TEST_MODULE adc_f0
#include <boost/test/unit_test.hpp>

#define F_CPU 48'000'000UL
#define STM32F030x6

#include "mock_interrupt.h"
#include "mock_rcc.h"
#include "mock_gpio.h"
#include "mock_dma.h"
#include "mock_periph_adc.h"

#include "adc.h"
#include <thread>
#include "timeout.h"
#include "literals.h"

BOOST_AUTO_TEST_SUITE(test_suite_main)

auto &process = mock::Process::make();
constexpr auto conversion_on_channel{3};

BOOST_AUTO_TEST_CASE(make)
{

    auto &adc = ADC_average::make<mcu::Periph::ADC1>(conversion_on_channel);

    auto adr = std::to_string(size_t(&like_CMSIS(mock::adc1).DR));

    BOOST_CHECK_EQUAL (process.str(),
        "включение тактирования ADC1"                                      "\n"
        "ADC1: Установка разрешения: 12 бит"                               "\n"
        "ADC1: Автоматический старт следующего преобразования"             "\n"
        "ADC1: Разрешение DMA"                                             "\n"
        "ADC1: Установка режима DMA: по кругу"                             "\n"
        "ADC1: Установка частоты тактирования: PCLK/4"                     "\n"
        "ADC1: Установка времени семплирования 239,5 тактов"               "\n"
        "включение тактирования DMA1"                                      "\n"
        "DMA1_stream1: Установка адреса переферии: " + adr +               "\n"
        "DMA1_stream1: Установка направления из переферии в память"        "\n"
        "DMA1_stream1: Установка размера данных в памяти слово (16бит)"    "\n"
        "DMA1_stream1: Установка размера данных в переферии слово (16бит)" "\n"
        "DMA1_stream1: Установка инкремента адреса памяти"                 "\n"
        "DMA1_stream1: Разрешение прерывания по концу передачи данных"     "\n"
        "NVIC: включение прерывания DMA1_Channel1"                         "\n"

    );
}

BOOST_AUTO_TEST_CASE(add_channel)
{
    auto &adc = ADC_average::make<mcu::Periph::ADC1>(conversion_on_channel);
    process.clear();
    auto &value1 = adc.add_channel<mcu::PA0>();
    auto adr = std::to_string(adc.buffer_address());

    BOOST_CHECK_EQUAL (process.str(),
        "включение тактирования GPIOA" "\n"
        "инициализация вывода 0 порта GPIOA в режиме Analog"    "\n"
        "ADC1: Включение канала 0"                              "\n"
        "DMA1_stream1: Запрет работы"                           "\n"
        "DMA1_stream1: Установка адреса памяти: " + adr +       "\n"
        "DMA1_stream1: Установка количества передач данных: 3"  "\n"
        "DMA1_stream1: Разрешение работы"                       "\n"
    );

    process.clear();
    auto &value2 = adc.add_channel<mcu::PB1>();
    BOOST_CHECK_EQUAL (process.str(),
        "включение тактирования GPIOB"                         "\n"
        "инициализация вывода 1 порта GPIOB в режиме Analog"   "\n"
        "ADC1: Включение канала 9"                             "\n"
        "DMA1_stream1: Запрет работы"                          "\n"
        "DMA1_stream1: Установка адреса памяти: " + adr +      "\n"
        "DMA1_stream1: Установка количества передач данных: 6" "\n"
        "DMA1_stream1: Разрешение работы"                      "\n"
    );
    adc.reset_channels(); // потому что статический
}

BOOST_AUTO_TEST_CASE(start)
{
    auto &adc = ADC_average::make<mcu::Periph::ADC1>(conversion_on_channel);
    auto &value1 = adc.add_channel<mcu::PA0>();
    auto &value2 = adc.add_channel<mcu::PB1>();
    process.clear();

    like_CMSIS(mock::adc1).ISR = ADC_ISR_ADRDY_Msk;
    std::thread{[&] { adc.start(); }}.detach();

    Timeout timeout{10_ms};
    while (not timeout)
    {
    }
    like_CMSIS(mock::adc1).ISR = 0;
    timeout.restart();
    while (not timeout)
    {
    }

    BOOST_CHECK_EQUAL (process.str(),
        "DMA1_stream1: Разрешение работы" "\n"
        "ADC1: Пометить как работающий"   "\n"
        "ADC1: Разрешение работы"         "\n"
        "ADC1: Запуск преобразования"     "\n"
    );

    adc.reset_channels(); // потому что статический
}

BOOST_AUTO_TEST_CASE(start2)
{
    auto &adc = ADC_average::make<mcu::Periph::ADC1>(conversion_on_channel);
    auto &value1 = adc.add_channel<mcu::PA0>();
    auto &value2 = adc.add_channel<mcu::PB1>();
    process.clear();

    like_CMSIS(mock::adc1).ISR = 0;
    std::thread{[&] { adc.start(); }}.detach();

    Timeout timeout{10_ms};
    while (not timeout)
    {
    }

    BOOST_CHECK_EQUAL (process.str(),
        "DMA1_stream1: Разрешение работы" "\n"
        "ADC1: Разрешение работы"         "\n"
    );

    adc.reset_channels(); // потому что статический
}

BOOST_AUTO_TEST_CASE(values)
{
    mock::dma1_stream1.mock.reset();
    like_CMSIS(mock::adc1).ISR = ADC_ISR_ADRDY_Msk;
    auto &adc = ADC_average::make<mcu::Periph::ADC1>(conversion_on_channel);
    auto &value1 = adc.add_channel<mcu::PA0>();
    auto &value2 = adc.add_channel<mcu::PB1>();
    adc.start();

    mock::dma1_stream1.mock.transfer(1, 2, 1, 2, 1, 2);

    BOOST_CHECK_EQUAL(uint32_t(value1), 3);
    BOOST_CHECK_EQUAL(uint32_t(value2), 6);

    mock::dma1_stream1.mock.transfer(4, 3, 4, 3, 4);

    BOOST_CHECK_EQUAL(uint32_t(value1), 3);
    BOOST_CHECK_EQUAL(uint32_t(value2), 6);

    mock::dma1_stream1.mock.transfer(3);

    BOOST_CHECK_EQUAL(uint32_t(value1), 12);
    BOOST_CHECK_EQUAL(uint32_t(value2), 9);

    adc.reset_channels(); // потому что статический
}

BOOST_AUTO_TEST_CASE(values_reverse)
{
    mock::dma1_stream1.mock.reset();
    auto &adc = ADC_average::make<mcu::Periph::ADC1>(conversion_on_channel);
    like_CMSIS(mock::adc1).ISR = ADC_ISR_ADRDY_Msk;
    auto &value1 = adc.add_channel<mcu::PB1>();
    auto &value2 = adc.add_channel<mcu::PA0>();
    adc.start();

    mock::dma1_stream1.mock.transfer(1, 2, 1, 2, 1, 2);

    BOOST_CHECK_EQUAL(uint32_t(value1), 6);
    BOOST_CHECK_EQUAL(uint32_t(value2), 3);

    mock::dma1_stream1.mock.transfer(4, 3, 4, 3, 4);

    BOOST_CHECK_EQUAL(uint32_t(value1), 6);
    BOOST_CHECK_EQUAL(uint32_t(value2), 3);

    mock::dma1_stream1.mock.transfer(3);

    BOOST_CHECK_EQUAL(uint32_t(value1), 9);
    BOOST_CHECK_EQUAL(uint32_t(value2), 12);

    adc.reset_channels(); // потому что статический
}

BOOST_AUTO_TEST_CASE(lambda)
{
    mock::dma1_stream1.mock.reset();
    like_CMSIS(mock::adc1).ISR = ADC_ISR_ADRDY_Msk;
    auto &adc = ADC_average::make<mcu::Periph::ADC1>(conversion_on_channel);
    auto &value1 = adc.add_channel<mcu::PB1>();
    auto &value2 = adc.add_channel<mcu::PA0>();
    auto counter {0};
    adc.set_callback([&](){counter++;});
    adc.start();

    BOOST_CHECK_EQUAL(counter, 0);
    mock::dma1_stream1.mock.transfer(1, 2, 1, 2, 1, 2);
    BOOST_CHECK_EQUAL(counter, 1);
    mock::dma1_stream1.mock.transfer(4, 3, 4, 3, 4);
    BOOST_CHECK_EQUAL(counter, 1);
    mock::dma1_stream1.mock.transfer(3);
    BOOST_CHECK_EQUAL(counter, 2);

    mock::dma1_stream1.mock.interrupt();
    BOOST_CHECK_EQUAL(counter, 3);

    adc.reset_channels(); // потому что статический
}

BOOST_AUTO_TEST_SUITE_END()