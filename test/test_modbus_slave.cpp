#define BOOST_TEST_MODULE modbus_slave
#include <boost/test/unit_test.hpp>

#define F_CPU   72000000UL
#define STM32F103xB
#define TEST


#include <iostream>
#include <type_traits>
#include "mock_systick.h"
#include "mock_uart.h"
#include "mock_interrupt.h"
#include "modbus_slave.h"
#include "sstream"

auto& process = mock::Process::make();

int count {0};
void reaction(uint16_t reg_address){
    count++;
}

BOOST_AUTO_TEST_SUITE (test_suite_main)

BOOST_AUTO_TEST_CASE(make)
{
    process.clear();
    ::UART::Settings set;
    
    struct InReg{
        uint16_t a;
        uint16_t b;
        uint16_t c;
        uint16_t d;
        uint16_t f;
        uint16_t g;
    } in_reg;

    struct OutReg {
        int16_t  a = -25;
        uint16_t b = 0x12C;
        uint16_t c = 3;
        uint16_t d = 4;
        uint16_t f = 5;
        uint16_t g = 6;
    } out_reg;
    
    const uint8_t address = 1;
    auto& modbus = Modbus_slave<InReg, OutReg>
                 ::make<mcu::Periph::USART1, 
                   mcu::PA9, mcu::PA10, mcu::PA11, mcu::PA12>(address, set);
    
    BOOST_CHECK_EQUAL(process.str(),
        "Создаем объект UART"                    "\n"
        "Определение время задержки для модбаса" "\n"
        "Инициализация uart"                     "\n"
        // "Определение время задержки для модбаса" "\n"
    );
}
/*
BOOST_AUTO_TEST_CASE (read)
{
    UART::Settings set;
    
    struct InReg{
        uint16_t a;
        uint16_t b;
        uint16_t c;
        uint16_t d;
        uint16_t f;
        uint16_t g;
    } in_reg;

    struct OutReg {
        int16_t  a = -25;
        uint16_t b = 0x12C;
        uint16_t c = 3;
        uint16_t d = 4;
        uint16_t f = 5;
        uint16_t g = 6;
    } out_reg;

    interrupt_usart1.clear_subscribe();
    interrupt_DMA_channel4.clear_subscribe();

    result.str("");
    const uint8_t address = 1;
    auto& modbus = Modbus_slave<InReg, OutReg>
                 ::make<mcu::Periph::USART1, 
                  mcu::PA9, mcu::PA10, mcu::PA11, mcu::PA12>(address, set);

    buffer[0] = 1;
    buffer[1] = 3;
    buffer[2] = 0;
    buffer[3] = 5;
    buffer[4] = 0;
    buffer[5] = 1;

    uint16_t crc = 0x0B94;
    uint8_t crc_low = static_cast<uint8_t>(crc);
    uint8_t crc_high = crc >> 8;

    buffer[6] = crc_low;
    buffer[7] = crc_high;

    mcu::CNDTR = 247;

    USART1_IRQHandler();

    SysTick_Handler();
    SysTick_Handler();
    SysTick_Handler();
    SysTick_Handler();
    SysTick_Handler();

    modbus(reaction);

    crc = 0x4638;
    crc_low = static_cast<uint8_t>(crc);
    crc_high = crc >> 8;

    BOOST_CHECK_EQUAL(buffer[0], address);
    BOOST_CHECK_EQUAL(buffer[1], 3);
    BOOST_CHECK_EQUAL(buffer[2], sizeof(uint16_t));
    BOOST_CHECK_EQUAL(buffer[3], out_reg.g >> 8);
    BOOST_CHECK_EQUAL(buffer[4], out_reg.g);
    BOOST_CHECK_EQUAL(buffer[5], crc_low);
    BOOST_CHECK_EQUAL(buffer[6], crc_high);

    BOOST_CHECK_EQUAL(result.str(),
        "Создаем объект UART"                          "\n"
        "Определение время задержки для модбаса"       "\n"
        "Инициализация uart"                           "\n"
        "Прерывание uart"                              "\n"
        "Создали ссылку на переферию usart"            "\n"
        "Очищаем флаги прерываний uart"                "\n"
        "Забираем из буфера 8-битный элемент"          "\n"
        "Получаем значение CRC полученного буфера"     "\n"
        "Получаем значение CRC полученного буфера"     "\n"
        "Берем значение end"                           "\n"
        "Возвращаем указатель на буфер для расчета CRC""\n"
        "Забираем из буфера 8-битный элемент"          "\n"
        "Забираем из буфера 16-битный элемент"         "\n"
        "Забираем из буфера 8-битный элемент"          "\n"
        "Забираем из буфера 8-битный элемент"          "\n"
        "Забираем из буфера 16-битный элемент"         "\n"
        "Забираем из буфера 8-битный элемент"          "\n"
        "Забираем из буфера 8-битный элемент"          "\n"
        "Добавляем в буфер новый 8-битный элемент"     "\n"
        "Добавляем в буфер новый 8-битный элемент"     "\n"
        "Добавляем в буфер новый 8-битный элемент"     "\n"
        "Добавляем в буфер новый 16-битный элемент"    "\n"
        "Добавляем в буфер новый 8-битный элемент"     "\n"
        "Добавляем в буфер новый 8-битный элемент"     "\n"
        "Берем значение end"                           "\n"
        "Возвращаем указатель на буфер для расчета CRC""\n"
        "Добавляем в буфер новый 8-битный элемент"     "\n"
        "Добавляем в буфер новый 8-битный элемент"     "\n"
        "Старт передачи"                               "\n"
    );


    buffer[0] = 1;
    buffer[1] = 3;
    buffer[2] = 0;
    buffer[3] = 2;
    buffer[4] = 0;
    buffer[5] = 3;

    crc = 0x0BA4;
    crc_low = static_cast<uint8_t>(crc);
    crc_high = crc >> 8;

    buffer[6] = crc_low;
    buffer[7] = crc_high;

    USART1_IRQHandler();

    SysTick_Handler();
    SysTick_Handler();
    SysTick_Handler();
    SysTick_Handler();
    SysTick_Handler();

    modbus(reaction);

    crc = 0xB7E4;
    crc_low = static_cast<uint8_t>(crc);
    crc_high = crc >> 8;

    BOOST_CHECK_EQUAL(buffer[0], address);
    BOOST_CHECK_EQUAL(buffer[1], 3);
    BOOST_CHECK_EQUAL(buffer[2], sizeof(uint16_t) * 3);
    BOOST_CHECK_EQUAL(buffer[3], out_reg.c >> 8);
    BOOST_CHECK_EQUAL(buffer[4], out_reg.c);
    BOOST_CHECK_EQUAL(buffer[5], out_reg.d >> 8);
    BOOST_CHECK_EQUAL(buffer[6], out_reg.d);
    BOOST_CHECK_EQUAL(buffer[7], out_reg.f >> 8);
    BOOST_CHECK_EQUAL(buffer[8], out_reg.f);
    BOOST_CHECK_EQUAL(buffer[9], crc_low);
    BOOST_CHECK_EQUAL(buffer[10], crc_high);

    buffer[0] = 1;
    buffer[1] = 3;
    buffer[2] = 0;
    buffer[3] = 1;
    buffer[4] = 0;
    buffer[5] = 2;

    crc = 0xCB95;
    crc_low = static_cast<uint8_t>(crc);
    crc_high = crc >> 8;

    buffer[6] = crc_low;
    buffer[7] = crc_high;

    USART1_IRQHandler();

    SysTick_Handler();
    SysTick_Handler();
    SysTick_Handler();
    SysTick_Handler();
    SysTick_Handler();

    modbus(reaction);

    crc = 0x077A;
    crc_low = static_cast<uint8_t>(crc);
    crc_high = crc >> 8;

    BOOST_CHECK_EQUAL(buffer[0], address);
    BOOST_CHECK_EQUAL(buffer[1], 3);
    BOOST_CHECK_EQUAL(buffer[2], sizeof(uint16_t) * 2);
    BOOST_CHECK_EQUAL(buffer[3], out_reg.b >> 8);
    BOOST_CHECK_EQUAL(buffer[4], static_cast<uint8_t>(out_reg.b));
    BOOST_CHECK_EQUAL(buffer[5], out_reg.c >> 8);
    BOOST_CHECK_EQUAL(buffer[6], out_reg.c);
    BOOST_CHECK_EQUAL(buffer[7], crc_low);
    BOOST_CHECK_EQUAL(buffer[8], crc_high);

    buffer[0] = 1;
    buffer[1] = 3;
    buffer[2] = 0;
    buffer[3] = 0;
    buffer[4] = 0;
    buffer[5] = 1;

    crc = 0x0A84;
    crc_low = static_cast<uint8_t>(crc);
    crc_high = crc >> 8;

    buffer[6] = crc_low;
    buffer[7] = crc_high;

    
    USART1_IRQHandler();

    SysTick_Handler();
    SysTick_Handler();
    SysTick_Handler();
    SysTick_Handler();
    SysTick_Handler();

    modbus(reaction);

    crc =  0xFEB9;
    crc_low = static_cast<uint8_t>(crc);
    crc_high = crc >> 8;

    BOOST_CHECK_EQUAL(buffer[0], address);
    BOOST_CHECK_EQUAL(buffer[1], 3);
    BOOST_CHECK_EQUAL(buffer[2], sizeof(int16_t));
    BOOST_CHECK_EQUAL(buffer[3], 255);
    BOOST_CHECK_EQUAL(buffer[4], 231);
    BOOST_CHECK_EQUAL(buffer[5], crc_low);
    BOOST_CHECK_EQUAL(buffer[6], crc_high);

}

BOOST_AUTO_TEST_CASE (incomplete_message)
{
    UART::Settings set;
    
    struct InReg{
        uint16_t a;
        uint16_t b;
        uint16_t c;
        uint16_t d;
        uint16_t f;
        uint16_t g;
    };

    struct OutReg {
        uint16_t a = 1;
        uint16_t b = 2;
        uint16_t c = 3;
        uint16_t d = 4;
        uint16_t f = 5;
        uint16_t g = 6;
    } out_reg;

    interrupt_usart1.clear_subscribe();
    interrupt_DMA_channel4.clear_subscribe();

    
    const uint8_t address = 1;
    auto& modbus = Modbus_slave<InReg, OutReg>
                 ::make<mcu::Periph::USART1, 
                   mcu::PA9, mcu::PA10, mcu::PA11, mcu::PA12>(address, set);

    buffer[0] = 1;

    result.str("");
    USART1_IRQHandler();

    SysTick_Handler();
    SysTick_Handler();
    SysTick_Handler();
    SysTick_Handler();
    SysTick_Handler();
    
    mcu::CNDTR = 254;
    modbus(reaction);

    BOOST_CHECK_EQUAL(result.str(),
        "Прерывание uart"                   "\n"
        "Создали ссылку на переферию usart" "\n"
        "Очищаем флаги прерываний uart"     "\n"
        "Очищаем буфер"                     "\n"
        "Старт приема"                      "\n"
    );

}

BOOST_AUTO_TEST_CASE (wrong_CRC)
{
    UART::Settings set;
    
    struct InReg{
        uint16_t a;
        uint16_t b;
        uint16_t c;
        uint16_t d;
        uint16_t f;
        uint16_t g;
    };

    struct OutReg {
        uint16_t a = 1;
        uint16_t b = 2;
        uint16_t c = 3;
        uint16_t d = 4;
        uint16_t f = 5;
        uint16_t g = 6;
    } out_reg;

    interrupt_usart1.clear_subscribe();
    interrupt_DMA_channel4.clear_subscribe();

    const uint8_t address = 1;
    auto& modbus = Modbus_slave<InReg, OutReg>
                 ::make<mcu::Periph::USART1, 
                   mcu::PA9, mcu::PA10, mcu::PA11, mcu::PA12>(address, set);

    buffer[0] = 1;
    buffer[1] = 3;
    buffer[2] = 0;
    buffer[3] = 5;
    buffer[4] = 0;
    buffer[5] = 1;

    uint16_t crc = 0x0BA4; //wrong CRC
    uint8_t crc_low = static_cast<uint8_t>(crc);
    uint8_t crc_high = crc >> 8;

    buffer[6] = crc_low;
    buffer[7] = crc_high;

    mcu::CNDTR = 247;

    result.str("");
    USART1_IRQHandler();

    SysTick_Handler();
    SysTick_Handler();
    SysTick_Handler();
    SysTick_Handler();
    SysTick_Handler();
    
    
    modbus(reaction);

    BOOST_CHECK_EQUAL(result.str(),
        "Прерывание uart"                               "\n"
        "Создали ссылку на переферию usart"             "\n"
        "Очищаем флаги прерываний uart"                 "\n"
        "Забираем из буфера 8-битный элемент"           "\n"
        "Получаем значение CRC полученного буфера"      "\n"
        "Получаем значение CRC полученного буфера"      "\n"
        "Берем значение end"                            "\n"
        "Возвращаем указатель на буфер для расчета CRC" "\n"
        "Очищаем буфер"                                 "\n"
        "Старт приема"                                  "\n"
    );

}

BOOST_AUTO_TEST_CASE (wrong_adr)
{
    UART::Settings set;
    
    struct InReg{
        uint16_t a;
        uint16_t b;
        uint16_t c;
        uint16_t d;
        uint16_t f;
        uint16_t g;
    };

    struct OutReg {
        uint16_t a = 6;
        uint16_t b = 5;
        uint16_t c = 4;
        uint16_t d = 3;
        uint16_t f = 5;
        uint16_t g = 6;
    } out_reg;

    interrupt_usart1.clear_subscribe();
    interrupt_DMA_channel4.clear_subscribe();

    const uint8_t address = 1;
    auto& modbus = Modbus_slave<InReg, OutReg>
                 ::make<mcu::Periph::USART1, 
                   mcu::PA9, mcu::PA10, mcu::PA11, mcu::PA12>(address, set);

    buffer[0] = 9;
    buffer[1] = 3;
    buffer[2] = 0;
    buffer[3] = 5;
    buffer[4] = 0;
    buffer[5] = 1;

    uint16_t crc = 0x4395 ; 
    uint8_t crc_low = static_cast<uint8_t>(crc);
    uint8_t crc_high = crc >> 8;

    buffer[6] = crc_low;
    buffer[7] = crc_high;

    mcu::CNDTR = 247;

    result.str("");
    USART1_IRQHandler();

    SysTick_Handler();
    SysTick_Handler();
    SysTick_Handler();
    SysTick_Handler();
    SysTick_Handler();
    
    
    modbus(reaction);

    BOOST_CHECK_EQUAL(result.str(),
        "Прерывание uart"                               "\n"
        "Создали ссылку на переферию usart"             "\n"
        "Очищаем флаги прерываний uart"                 "\n"
        "Забираем из буфера 8-битный элемент"           "\n"
        "Очищаем буфер"                                 "\n"
        "Старт приема"                                  "\n"
    );

}

BOOST_AUTO_TEST_CASE (wrong_func)
{
    UART::Settings set;
    
    struct InReg{
        uint16_t a;
        uint16_t b;
        uint16_t c;
        uint16_t d;
        uint16_t f;
        uint16_t g;
    };

    struct OutReg {
        uint16_t a = 6;
        uint16_t b = 5;
        uint16_t c = 4;
        uint16_t d = 3;
        uint16_t f = 5;
        uint16_t g = 6;
    } out_reg;

    interrupt_usart1.clear_subscribe();
    interrupt_DMA_channel4.clear_subscribe();

    const uint8_t address = 1;
    uint8_t wrong_func = 1;
    auto& modbus = Modbus_slave<InReg, OutReg>
                 ::make<mcu::Periph::USART1, 
                   mcu::PA9, mcu::PA10, mcu::PA11, mcu::PA12>(address, set);

    buffer[0] = 1;
    buffer[1] = 9;
    buffer[2] = 0;
    buffer[3] = 5;
    buffer[4] = 0;
    buffer[5] = 1;

    uint16_t crc = 0x0A0C; 
    uint8_t  crc_low = static_cast<uint8_t>(crc);
    uint8_t  crc_high = crc >> 8;

    buffer[6] = crc_low;
    buffer[7] = crc_high;

    mcu::CNDTR = 247;

    result.str("");
    USART1_IRQHandler();

    SysTick_Handler();
    SysTick_Handler();
    SysTick_Handler();
    SysTick_Handler();
    SysTick_Handler();
    
    modbus(reaction);

    crc = 0x5086;
    crc_low = static_cast<uint8_t>(crc);
    crc_high = crc >> 8;

    BOOST_CHECK_EQUAL(buffer[0], address);
    BOOST_CHECK_EQUAL(buffer[1], 9 | 0b10000000);
    BOOST_CHECK_EQUAL(buffer[2], wrong_func);
    BOOST_CHECK_EQUAL(buffer[3], crc_low);
    BOOST_CHECK_EQUAL(buffer[4], crc_high);

    BOOST_CHECK_EQUAL(result.str(),
        "Прерывание uart"                               "\n"
        "Создали ссылку на переферию usart"             "\n"
        "Очищаем флаги прерываний uart"                 "\n"
        "Забираем из буфера 8-битный элемент"           "\n"
        "Получаем значение CRC полученного буфера"      "\n"
        "Получаем значение CRC полученного буфера"      "\n"
        "Берем значение end"                            "\n"
        "Возвращаем указатель на буфер для расчета CRC" "\n"
        "Забираем из буфера 8-битный элемент"           "\n"
        "Очищаем буфер"                                 "\n"
        "Добавляем в буфер новый 8-битный элемент"      "\n"
        "Добавляем в буфер новый 8-битный элемент"      "\n"
        "Добавляем в буфер новый 8-битный элемент"      "\n"
        "Берем значение end"                            "\n"
        "Возвращаем указатель на буфер для расчета CRC" "\n"
        "Добавляем в буфер новый 8-битный элемент"      "\n"
        "Добавляем в буфер новый 8-битный элемент"      "\n"
        "Старт передачи"                                "\n"
    );

}

BOOST_AUTO_TEST_CASE (wrong_reg)
{
    UART::Settings set;
    
    struct InReg{
        uint16_t a;
        uint16_t b;
        uint16_t c;
        uint16_t d;
        uint16_t f;
        uint16_t g;
    };

    struct OutReg {
        uint16_t a = 6;
        uint16_t b = 5;
        uint16_t c = 4;
        uint16_t d = 3;
    };

    interrupt_usart1.clear_subscribe();
    interrupt_DMA_channel4.clear_subscribe();

    const uint8_t address = 3;
    uint8_t wrong_reg = 2;
    auto& modbus = Modbus_slave<InReg, OutReg>
                 ::make<mcu::Periph::USART1, 
                   mcu::PA9, mcu::PA10, mcu::PA11, mcu::PA12>(address, set);

    buffer[0] = 3;
    buffer[1] = 3;
    buffer[2] = 0;
    buffer[3] = 9;
    buffer[4] = 0;
    buffer[5] = 1;

    uint16_t crc = 0xEA55; 
    uint8_t  crc_low = static_cast<uint8_t>(crc);
    uint8_t  crc_high = crc >> 8;

    buffer[6] = crc_low;
    buffer[7] = crc_high;

    mcu::CNDTR = 247;

    result.str("");
    USART1_IRQHandler();

    SysTick_Handler();
    SysTick_Handler();
    SysTick_Handler();
    SysTick_Handler();
    SysTick_Handler();
    
    modbus(reaction);

    crc = 0xF100;
    crc_low = static_cast<uint8_t>(crc);
    crc_high = crc >> 8;

    BOOST_CHECK_EQUAL(buffer[0], address);
    BOOST_CHECK_EQUAL(buffer[1], 3);
    BOOST_CHECK_EQUAL(buffer[2], wrong_reg);
    BOOST_CHECK_EQUAL(buffer[3], crc_low);
    BOOST_CHECK_EQUAL(buffer[4], crc_high);

    BOOST_CHECK_EQUAL(result.str(),
        "Прерывание uart"                               "\n"
        "Создали ссылку на переферию usart"             "\n"
        "Очищаем флаги прерываний uart"                 "\n"
        "Забираем из буфера 8-битный элемент"           "\n"
        "Получаем значение CRC полученного буфера"      "\n"
        "Получаем значение CRC полученного буфера"      "\n"
        "Берем значение end"                            "\n"
        "Возвращаем указатель на буфер для расчета CRC" "\n"
        "Забираем из буфера 8-битный элемент"           "\n"
        "Забираем из буфера 16-битный элемент"          "\n"
        "Забираем из буфера 8-битный элемент"           "\n"
        "Забираем из буфера 8-битный элемент"           "\n"
        "Забираем из буфера 16-битный элемент"          "\n"
        "Забираем из буфера 8-битный элемент"           "\n"
        "Забираем из буфера 8-битный элемент"           "\n"
        "Очищаем буфер"                                 "\n"
        "Добавляем в буфер новый 8-битный элемент"      "\n"
        "Добавляем в буфер новый 8-битный элемент"      "\n"
        "Добавляем в буфер новый 8-битный элемент"      "\n"
        "Берем значение end"                            "\n"
        "Возвращаем указатель на буфер для расчета CRC" "\n"
        "Добавляем в буфер новый 8-битный элемент"      "\n"
        "Добавляем в буфер новый 8-битный элемент"      "\n"
        "Старт передачи"                                "\n"
    );

}

BOOST_AUTO_TEST_CASE (write)
{
    UART::Settings set;
    
    struct InReg{
        uint16_t a;
        uint16_t b;
        uint16_t c;
        int16_t  d;
        uint16_t f;
        uint16_t g;
    } in_reg;

    struct OutReg {
        uint16_t a = 6;
        uint16_t b = 5;
        uint16_t c = 4;
        uint16_t d = 3;
    } out_reg;

    interrupt_usart1.clear_subscribe();
    interrupt_DMA_channel4.clear_subscribe();

    const uint8_t address = 7;
    auto& modbus = Modbus_slave<InReg, OutReg>
                 ::make<mcu::Periph::USART1, 
                   mcu::PA9, mcu::PA10, mcu::PA11, mcu::PA12>(address, set);

    buffer[0] = 7;
    buffer[1] = 16;
    buffer[2] = 0;
    buffer[3] = 2;
    buffer[4] = 0;
    buffer[5] = 1;
    buffer[6] = 2;
    buffer[7] = 0;
    buffer[8] = 8;

    uint16_t crc = 0xD48D; 
    uint8_t  crc_low = static_cast<uint8_t>(crc);
    uint8_t  crc_high = crc >> 8;

    buffer[9] = crc_low;
    buffer[10] = crc_high;

    mcu::CNDTR = 244;

    result.str("");
    USART1_IRQHandler();

    SysTick_Handler();
    SysTick_Handler();
    SysTick_Handler();
    SysTick_Handler();
    SysTick_Handler();
    
    modbus(reaction);

    crc = 0x6FA0;
    crc_low = static_cast<uint8_t>(crc);
    crc_high = crc >> 8;

    BOOST_CHECK_EQUAL(buffer[0], address);
    BOOST_CHECK_EQUAL(buffer[1], 0x10);
    BOOST_CHECK_EQUAL(buffer[2], 0);
    BOOST_CHECK_EQUAL(buffer[3], 2);
    BOOST_CHECK_EQUAL(buffer[4], 0);
    BOOST_CHECK_EQUAL(buffer[5], 1);
    BOOST_CHECK_EQUAL(buffer[6], crc_low);
    BOOST_CHECK_EQUAL(buffer[7], crc_high);
    BOOST_CHECK_EQUAL(modbus.inRegs.c, 8);
    BOOST_CHECK_EQUAL(count, 1);
    count = 0;

    BOOST_CHECK_EQUAL(result.str(),
        "Прерывание uart"                               "\n"
        "Создали ссылку на переферию usart"             "\n"
        "Очищаем флаги прерываний uart"                 "\n"
        "Забираем из буфера 8-битный элемент"           "\n"
        "Получаем значение CRC полученного буфера"      "\n"
        "Получаем значение CRC полученного буфера"      "\n"
        "Берем значение end"                            "\n"
        "Возвращаем указатель на буфер для расчета CRC" "\n"
        "Забираем из буфера 8-битный элемент"           "\n"
        "Забираем из буфера 16-битный элемент"          "\n"
        "Забираем из буфера 8-битный элемент"           "\n"
        "Забираем из буфера 8-битный элемент"           "\n"
        "Забираем из буфера 16-битный элемент"          "\n"
        "Забираем из буфера 8-битный элемент"           "\n"
        "Забираем из буфера 8-битный элемент"           "\n"
        "Забираем из буфера 8-битный элемент"           "\n"
        "Забираем из буфера 16-битный элемент"          "\n"
        "Забираем из буфера 8-битный элемент"           "\n"
        "Забираем из буфера 8-битный элемент"           "\n"
        "Добавляем в буфер новый 8-битный элемент"      "\n"
        "Добавляем в буфер новый 8-битный элемент"      "\n"
        "Добавляем в буфер новый 16-битный элемент"     "\n"
        "Добавляем в буфер новый 8-битный элемент"      "\n"
        "Добавляем в буфер новый 8-битный элемент"      "\n"
        "Добавляем в буфер новый 16-битный элемент"     "\n"
        "Добавляем в буфер новый 8-битный элемент"      "\n"
        "Добавляем в буфер новый 8-битный элемент"      "\n"
        "Берем значение end"                            "\n"
        "Возвращаем указатель на буфер для расчета CRC" "\n"
        "Добавляем в буфер новый 8-битный элемент"      "\n"
        "Добавляем в буфер новый 8-битный элемент"      "\n"
        "Старт передачи"                                "\n"
    );

    buffer[0] = 7;
    buffer[1] = 0x10;
    buffer[2] = 0;
    buffer[3] = 2;
    buffer[4] = 0;
    buffer[5] = 3;
    buffer[6] = 6;
    buffer[7] = 0x16;
    buffer[8] = 0x26; // [7] and [8] = 5670
    buffer[9] = 0xFF;
    buffer[10] = 0xDE; // [9] and [10] = -34
    buffer[11] = 0;
    buffer[12] = 7;

    crc = 0x5154; 
    crc_low = static_cast<uint8_t>(crc);
    crc_high = crc >> 8;

    buffer[13] = crc_low;
    buffer[14] = crc_high;

    mcu::CNDTR = 240;

    result.str("");
    USART1_IRQHandler();

    SysTick_Handler();
    SysTick_Handler();
    SysTick_Handler();
    SysTick_Handler();
    SysTick_Handler();
    
    modbus.signed_[3] =true;
    modbus(reaction);

    crc = 0xAE21;
    crc_low = static_cast<uint8_t>(crc);
    crc_high = crc >> 8;

    BOOST_CHECK_EQUAL(buffer[0], address);
    BOOST_CHECK_EQUAL(buffer[1], 0x10);
    BOOST_CHECK_EQUAL(buffer[2], 0);
    BOOST_CHECK_EQUAL(buffer[3], 2);
    BOOST_CHECK_EQUAL(buffer[4], 0);
    BOOST_CHECK_EQUAL(buffer[5], 3);
    BOOST_CHECK_EQUAL(buffer[6], crc_low);
    BOOST_CHECK_EQUAL(buffer[7], crc_high);
    BOOST_CHECK_EQUAL(modbus.inRegs.c, 5670);
    BOOST_CHECK_EQUAL(modbus.inRegs.d, -34);
    BOOST_CHECK_EQUAL(modbus.inRegs.f, 7);
    BOOST_CHECK_EQUAL(count, 3);
    count = 0;

}

BOOST_AUTO_TEST_CASE (check_value)
{
    UART::Settings set;
    
    struct InReg{
        int16_t  a;
        uint16_t b;
        uint16_t c;
        int16_t  d;
        uint16_t f;
        uint16_t g;
    } in_reg;

    struct OutReg {
        uint16_t a = 6;
        uint16_t b = 5;
        uint16_t c = 4;
        uint16_t d = 3;
    } out_reg;

    interrupt_usart1.clear_subscribe();
    interrupt_DMA_channel4.clear_subscribe();

    const uint8_t address = 7;
    auto& modbus = Modbus_slave<InReg, OutReg>
                 ::make<mcu::Periph::USART1, 
                   mcu::PA9, mcu::PA10, mcu::PA11, mcu::PA12>(address, set);

    buffer[0] = 7;
    buffer[1] = 16;
    buffer[2] = 0;
    buffer[3] = 0;
    buffer[4] = 0;
    buffer[5] = 1;
    buffer[6] = 2;
    buffer[7] = 3;
    buffer[8] = 0x15; // [7] and [8] = 789

    uint16_t crc = 0xCF4C; 
    uint8_t  crc_low = static_cast<uint8_t>(crc);
    uint8_t  crc_high = crc >> 8;

    buffer[9] = crc_low;
    buffer[10] = crc_high;

    mcu::CNDTR = 244;

    USART1_IRQHandler();

    SysTick_Handler();
    SysTick_Handler();
    SysTick_Handler();
    SysTick_Handler();
    SysTick_Handler();
    
    modbus.arInRegsMin[0] = -500;
    modbus.arInRegsMax[0] = 500;
    modbus(reaction);

    crc = 0xC08D;
    crc_low = static_cast<uint8_t>(crc);
    crc_high = crc >> 8;

    BOOST_CHECK_EQUAL(buffer[0], address);
    BOOST_CHECK_EQUAL(buffer[1], 0x10);
    BOOST_CHECK_EQUAL(buffer[2], 3);
    BOOST_CHECK_EQUAL(buffer[3], crc_low);
    BOOST_CHECK_EQUAL(buffer[4], crc_high);
    // BOOST_CHECK_EQUAL(modbus.inRegs.c, 8);
}

BOOST_AUTO_TEST_CASE (not_tick)
{
    UART::Settings set;
    
    struct InReg{
        uint16_t a;
        uint16_t b;
        uint16_t c;
        int16_t  d;
        uint16_t f;
        uint16_t g;
    } in_reg;

    struct OutReg {
        uint16_t a = 6;
        uint16_t b = 5;
        uint16_t c = 4;
        uint16_t d = 3;
    } out_reg;

    interrupt_usart1.clear_subscribe();
    interrupt_DMA_channel4.clear_subscribe();

    const uint8_t address = 7;
    auto& modbus = Modbus_slave<InReg, OutReg>
                 ::make<mcu::Periph::USART1, 
                   mcu::PA9, mcu::PA10, mcu::PA11, mcu::PA12>(address, set);

    buffer[0] = 7;
    buffer[1] = 16;
    buffer[2] = 0;
    buffer[3] = 2;
    buffer[4] = 0;
    buffer[5] = 1;
    buffer[6] = 2;
    buffer[7] = 0;
    buffer[8] = 8;

    uint16_t crc = 0xD48D; 
    uint8_t  crc_low = static_cast<uint8_t>(crc);
    uint8_t  crc_high = crc >> 8;

    buffer[9] = crc_low;
    buffer[10] = crc_high;

    mcu::CNDTR = 244;

    USART1_IRQHandler();

    SysTick_Handler();
    SysTick_Handler();
    SysTick_Handler(); //время выдержки для modbus time = 3 < 4;
    // буфер остается непрочитанным
    BOOST_CHECK_EQUAL(buffer[0], address);
    BOOST_CHECK_EQUAL(buffer[1], 0x10);
    BOOST_CHECK_EQUAL(buffer[2], 0);
    BOOST_CHECK_EQUAL(buffer[3], 2);
    BOOST_CHECK_EQUAL(buffer[4], 0);
    BOOST_CHECK_EQUAL(buffer[5], 1);
    BOOST_CHECK_EQUAL(buffer[6], 2);
    BOOST_CHECK_EQUAL(buffer[7], 0);
    BOOST_CHECK_EQUAL(buffer[8], 8);
    BOOST_CHECK_EQUAL(buffer[9], crc_low);
    BOOST_CHECK_EQUAL(buffer[10], crc_high);

    tickUpdater.clear(); // отписываемся от прерываний tick
}

BOOST_AUTO_TEST_CASE (not_uart_interrupt)
{
    UART::Settings set;
    
    struct InReg{
        uint16_t a;
        uint16_t b;
        uint16_t c;
        int16_t  d;
        uint16_t f;
        uint16_t g;
    } in_reg;

    struct OutReg {
        uint16_t a = 6;
        uint16_t b = 5;
        uint16_t c = 4;
        uint16_t d = 3;
    } out_reg;

    interrupt_usart1.clear_subscribe();
    interrupt_DMA_channel4.clear_subscribe();

    const uint8_t address = 7;
    auto& modbus = Modbus_slave<InReg, OutReg>
                 ::make<mcu::Periph::USART1, 
                   mcu::PA9, mcu::PA10, mcu::PA11, mcu::PA12>(address, set);

    buffer[0] = 7;
    buffer[1] = 16;
    buffer[2] = 0;
    buffer[3] = 2;
    buffer[4] = 0;
    buffer[5] = 1;
    buffer[6] = 2;
    buffer[7] = 0;
    buffer[8] = 8;

    uint16_t crc = 0xD48D; 
    uint8_t  crc_low = static_cast<uint8_t>(crc);
    uint8_t  crc_high = crc >> 8;

    buffer[9] = crc_low;
    buffer[10] = crc_high;

    mcu::CNDTR = 244;

    // USART1_IRQHandler(); // прерывания нет, подписки нет, notify нет

    SysTick_Handler();
    SysTick_Handler();
    SysTick_Handler();
    SysTick_Handler();
    SysTick_Handler(); //таймер не считает
    // буфер остается непрочитанным
    BOOST_CHECK_EQUAL(buffer[0], address);
    BOOST_CHECK_EQUAL(buffer[1], 0x10);
    BOOST_CHECK_EQUAL(buffer[2], 0);
    BOOST_CHECK_EQUAL(buffer[3], 2);
    BOOST_CHECK_EQUAL(buffer[4], 0);
    BOOST_CHECK_EQUAL(buffer[5], 1);
    BOOST_CHECK_EQUAL(buffer[6], 2);
    BOOST_CHECK_EQUAL(buffer[7], 0);
    BOOST_CHECK_EQUAL(buffer[8], 8);
    BOOST_CHECK_EQUAL(buffer[9], crc_low);
    BOOST_CHECK_EQUAL(buffer[10], crc_high);

    tickUpdater.clear();
}

BOOST_AUTO_TEST_CASE (dma_interrupt)
{
    UART::Settings set;
    
    struct InReg{
        uint16_t a;
        uint16_t b;
        uint16_t c;
        int16_t  d;
        uint16_t f;
        uint16_t g;
    } in_reg;

    struct OutReg {
        uint16_t a = 6;
        uint16_t b = 5;
        uint16_t c = 4;
        uint16_t d = 3;
    } out_reg;

    interrupt_usart1.clear_subscribe();
    interrupt_DMA_channel4.clear_subscribe();

    const uint8_t address = 7;
    auto& modbus = Modbus_slave<InReg, OutReg>
                 ::make<mcu::Periph::USART1, 
                   mcu::PA9, mcu::PA10, mcu::PA11, mcu::PA12>(address, set);

    buffer[0] = 7;
    buffer[1] = 16;
    buffer[2] = 0;
    buffer[3] = 2;
    buffer[4] = 0;
    buffer[5] = 1;
    buffer[6] = 2;
    buffer[7] = 0;
    buffer[8] = 8;

    uint16_t crc = 0xD48D; 
    uint8_t  crc_low = static_cast<uint8_t>(crc);
    uint8_t  crc_high = crc >> 8;

    buffer[9] = crc_low;
    buffer[10] = crc_high;

    mcu::CNDTR = 244;

    result.str("");
    USART1_IRQHandler();

    SysTick_Handler();
    SysTick_Handler();
    SysTick_Handler();
    SysTick_Handler();
    SysTick_Handler();
    
    modbus(reaction);
    DMA1_Channel4_IRQHandler(); // в прерывании переходим на прием

    crc = 0x6FA0;
    crc_low = static_cast<uint8_t>(crc);
    crc_high = crc >> 8;

    BOOST_CHECK_EQUAL(buffer[0], address);
    BOOST_CHECK_EQUAL(buffer[1], 0x10);
    BOOST_CHECK_EQUAL(buffer[2], 0);
    BOOST_CHECK_EQUAL(buffer[3], 2);
    BOOST_CHECK_EQUAL(buffer[4], 0);
    BOOST_CHECK_EQUAL(buffer[5], 1);
    BOOST_CHECK_EQUAL(buffer[6], crc_low);
    BOOST_CHECK_EQUAL(buffer[7], crc_high);
    BOOST_CHECK_EQUAL(modbus.inRegs.c, 8);

    BOOST_CHECK_EQUAL(result.str(),
        "Прерывание uart"                               "\n"
        "Создали ссылку на переферию usart"             "\n"
        "Очищаем флаги прерываний uart"                 "\n"
        "Забираем из буфера 8-битный элемент"           "\n"
        "Получаем значение CRC полученного буфера"      "\n"
        "Получаем значение CRC полученного буфера"      "\n"
        "Берем значение end"                            "\n"
        "Возвращаем указатель на буфер для расчета CRC" "\n"
        "Забираем из буфера 8-битный элемент"           "\n"
        "Забираем из буфера 16-битный элемент"          "\n"
        "Забираем из буфера 8-битный элемент"           "\n"
        "Забираем из буфера 8-битный элемент"           "\n"
        "Забираем из буфера 16-битный элемент"          "\n"
        "Забираем из буфера 8-битный элемент"           "\n"
        "Забираем из буфера 8-битный элемент"           "\n"
        "Забираем из буфера 8-битный элемент"           "\n"
        "Забираем из буфера 16-битный элемент"          "\n"
        "Забираем из буфера 8-битный элемент"           "\n"
        "Забираем из буфера 8-битный элемент"           "\n"
        "Добавляем в буфер новый 8-битный элемент"      "\n"
        "Добавляем в буфер новый 8-битный элемент"      "\n"
        "Добавляем в буфер новый 16-битный элемент"     "\n"
        "Добавляем в буфер новый 8-битный элемент"      "\n"
        "Добавляем в буфер новый 8-битный элемент"      "\n"
        "Добавляем в буфер новый 16-битный элемент"     "\n"
        "Добавляем в буфер новый 8-битный элемент"      "\n"
        "Добавляем в буфер новый 8-битный элемент"      "\n"
        "Берем значение end"                            "\n"
        "Возвращаем указатель на буфер для расчета CRC" "\n"
        "Добавляем в буфер новый 8-битный элемент"      "\n"
        "Добавляем в буфер новый 8-битный элемент"      "\n"
        "Старт передачи"                                "\n"
        "Прерывание dma"                                "\n"
        "Очищаем буфер"                                 "\n"
        "Старт приема"                                  "\n"
        "Создали ссылку на переферию tx stream"         "\n"
        "Очищаем флаги прерываний dma"                  "\n"
    );

    mcu::CNDTR = 255;
}

BOOST_AUTO_TEST_CASE(new_message)
{
	 UART::Settings set;
    
    struct InReg{
        uint16_t a;
        uint16_t b;
        uint16_t c;
        int16_t  d;
        uint16_t f;
        uint16_t g;
    } in_reg;

    struct OutReg {
        uint16_t a = 6;
        uint16_t b = 5;
        uint16_t c = 4;
        uint16_t d = 3;
    } out_reg;

    interrupt_usart1.clear_subscribe();
    interrupt_DMA_channel4.clear_subscribe();

    const uint8_t address = 3;
    uint8_t wrong_reg = 2;
    auto& modbus = Modbus_slave<InReg, OutReg>
                 ::make<mcu::Periph::USART1, 
                   mcu::PA9, mcu::PA10, mcu::PA11, mcu::PA12>(address, set);

    auto passed_ms = [&](int n)
    {
        while (n--) {
            SysTick_Handler();
            modbus(reaction);
        }
    };


	buffer[0] = 3;
    buffer[1] = 3;
    buffer[2] = 0;
    buffer[3] = 9;

    mcu::CNDTR = 251;

    USART1_IRQHandler();
    passed_ms(2);

    buffer[4] = 0;
    buffer[5] = 1;

    uint16_t crc = 0xEA55; 
    uint8_t  crc_low = static_cast<uint8_t>(crc);
    uint8_t  crc_high = crc >> 8;

    buffer[6] = crc_low;
    buffer[7] = crc_high;

    mcu::CNDTR = 247;
    passed_ms(2);

    USART1_IRQHandler();
    passed_ms(5);
    DMA1_Channel4_IRQHandler ();

    crc = 0xF100;
    crc_low = static_cast<uint8_t>(crc);
    crc_high = crc >> 8;

    BOOST_CHECK_EQUAL(buffer[0], address);
    BOOST_CHECK_EQUAL(buffer[1], 3);
    BOOST_CHECK_EQUAL(buffer[2], wrong_reg);
    BOOST_CHECK_EQUAL(buffer[3], crc_low);
    BOOST_CHECK_EQUAL(buffer[4], crc_high);
}
*/

BOOST_AUTO_TEST_SUITE_END()