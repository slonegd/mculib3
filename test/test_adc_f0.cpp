#define BOOST_TEST_MODULE adc_f0
#include <boost/test/unit_test.hpp>

#define F_CPU   48'000'000UL
#define STM32F030x6

#include "mock_rcc.h"
#include "mock_periph_adc.h"
#include "mock_dma.h"
#include "mock_interrupt.h"
#include "adc.h"


BOOST_AUTO_TEST_SUITE (test_suite_main)

auto& process = mock::Process::make();

BOOST_AUTO_TEST_CASE(make)
{
    process.clear();

    constexpr auto conversion_on_channel {16};
    auto& adc = ADC_average::make<mcu::Periph::ADC1>(conversion_on_channel);

    auto adr = std::to_string(size_t(&like_CMSIS(mock::adc1).DR));
    
    BOOST_CHECK_EQUAL(process.str(),
        "включение тактирования ADC1"                                     "\n"
        "ADC1: Установка частоты тактирования: PCLK/4"                    "\n"
        "ADC1: Установка разрешения: 12 бит"                              "\n"
        "ADC1: Установка времени семплирования 239,5 тактов"              "\n"
        "ADC1: Автоматический старт следующего преобразования"            "\n"
        "ADC1: Разрешение DMA"                                            "\n"
        "ADC1: Установка режима DMA: по кругу"                            "\n"
        "включение тактирования DMA1"                                     "\n"
        "DMA1_stream1: Установка адреса переферии: " + adr              + "\n"
        "DMA1_stream1: Установка направления из переферии в память"       "\n"
        "DMA1_stream1: Установка размера данных в памяти слово (16бит)"   "\n"
        "DMA1_stream1: Установка размера данных в переферии слово (16бит)""\n"
        "DMA1_stream1: Установка инкремента адреса памяти"                "\n"
        "DMA1_stream1: Разрешение прерывания по концу передачи данных"    "\n"
        "NVIC: включение прерывания DMA1_Channel1"                        "\n"

    );
}

BOOST_AUTO_TEST_CASE(add_channel)
{
    constexpr auto conversion_on_channel {16};
    auto& adc = ADC_average::make<mcu::Periph::ADC1>(conversion_on_channel);

    process.clear();

    auto& value = adc.add_channel<mcu::PA0>();

    
    // BOOST_CHECK_EQUAL(process.str(),
    //     "включение тактирования допиши вывод в поток переферии""\n"
    // );
}



BOOST_AUTO_TEST_SUITE_END()