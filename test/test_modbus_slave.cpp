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

auto passed_ms = [&](int n)
{
    while (n--) {
        SysTick_Handler();
    }
};

// auto passed_ms_ = [&](int n, std::function<void(std::function<void(uint16_t reg)> reaction)> f)
// {
//     while (n--) {
//         SysTick_Handler();
//         f(reaction);
//     }
// };

::UART::Settings set;

BOOST_AUTO_TEST_SUITE (test_suite_main)

BOOST_AUTO_TEST_CASE(make)
{
    process.clear();

    set.parity_enable = false;
    set.parity    = ::UART::Parity::even;
    set.data_bits = ::UART::DataBits::_8;
    set.stop_bits = ::UART::StopBits::_1;
    set.baudrate  = ::UART::Baudrate::BR9600;

    struct InReg {};
    struct OutReg{};
    
    const uint8_t address = 1;
    decltype(auto) modbus = Modbus_slave<InReg, OutReg>
                 ::make<mcu::Periph::USART1, 
                   mcu::PA9, mcu::PA10, mcu::PA11>(address, set);
    
    BOOST_CHECK_EQUAL(process.str(),
        "Инициализация PA9(TX), PA10(RX), PA11(RTS), USART1"  "\n"
        "настройка параметров USART1"                         "\n"
        "USART1: Установлена скорость 9600 бит/с"             "\n"
        "USART1: Задана проверка на четность"                 "\n"
        "USART1: Установлен размер пакета 8 бит"              "\n"
        "USART1: Установлено количество стоп битов: 1"        "\n"
        "USART1: Отключение проверки на чётность"             "\n"
    );
}

BOOST_AUTO_TEST_CASE (read)
{
    struct InReg {};

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

    
    const uint8_t  address   = 1;
          uint8_t  function  = 3;
          uint16_t first_reg = 5;
          uint16_t qty_reg   = 1;

    decltype(auto) modbus = Modbus_slave<InReg, OutReg>
                 ::make<mcu::Periph::USART1, 
                  mcu::PA9, mcu::PA10, mcu::PA11>(address, set);

    decltype(auto) buffer = modbus.buffer();
                  
    buffer << address;
    buffer << function; 
    buffer << first_reg;
    buffer << qty_reg; 

    uint16_t crc = 0x0B94;
    uint8_t crc_low = static_cast<uint8_t>(crc);
    uint8_t crc_high = crc >> 8;

    buffer << crc_low;
    buffer << crc_high;

    mock::CNDTR = 247;

    USART1_IRQHandler();

    passed_ms(5);

    process.clear();
    // passed_ms_(5, modbus);
    modbus(reaction);

    crc = 0x4638;

    BOOST_CHECK_EQUAL(process.str(),
        "Передача данных: 01 03 02 00 06 38 46" "\n"
    );

    DMA1_Channel4_IRQHandler(); 

    first_reg = 2;
    qty_reg   = 3;
    
    buffer << address;
    buffer << function; 
    buffer << first_reg;
    buffer << qty_reg; 

    crc = 0x0BA4;
    crc_low = static_cast<uint8_t>(crc);
    crc_high = crc >> 8;

    buffer << crc_low;
    buffer << crc_high;


    USART1_IRQHandler();

    passed_ms(5);
    process.clear();
    modbus(reaction);

    crc = 0xB7E4;

    BOOST_CHECK_EQUAL(process.str(),
        "Передача данных: 01 03 06 00 03 00 04 00 05 E4 B7" "\n"
    );

    DMA1_Channel4_IRQHandler(); 

    first_reg = 1;
    qty_reg   = 2;
    
    buffer << address;
    buffer << function; 
    buffer << first_reg;
    buffer << qty_reg; 

    crc = 0xCB95;
    crc_low = static_cast<uint8_t>(crc);
    crc_high = crc >> 8;

    buffer << crc_low;
    buffer << crc_high;

    USART1_IRQHandler();

    passed_ms(5);
    process.clear();
    modbus(reaction);

    crc = 0x077A;

    BOOST_CHECK_EQUAL(process.str(),
        "Передача данных: 01 03 04 01 2C 00 03 7A 07" "\n"
    );

    DMA1_Channel4_IRQHandler(); 

    first_reg = 0;
    qty_reg   = 1;
    
    buffer << address;
    buffer << function; 
    buffer << first_reg;
    buffer << qty_reg; 

    crc = 0x0A84;
    crc_low = static_cast<uint8_t>(crc);
    crc_high = crc >> 8;

    buffer << crc_low;
    buffer << crc_high;

    USART1_IRQHandler();

    passed_ms(5);
    process.clear();
    modbus(reaction);

    crc = 0xFEB9;

    BOOST_CHECK_EQUAL(process.str(),
        "Передача данных: 01 03 02 FF E7 B9 FE" "\n"
    );

    process.clear();
    DMA1_Channel4_IRQHandler(); 

    BOOST_CHECK_EQUAL(process.str(),
        "Очистка буфера, прием данных" "\n"
    );
}

BOOST_AUTO_TEST_CASE (incomplete_message)
{
    struct InReg {};
    struct OutReg{};

    interrupt_usart1.clear_subscribe();
    interrupt_DMA_channel4.clear_subscribe();

    
    const uint8_t address = 1;
    decltype(auto) modbus = Modbus_slave<InReg, OutReg>
                 ::make<mcu::Periph::USART1, 
                   mcu::PA9, mcu::PA10, mcu::PA11>(address, set);

    decltype(auto) buffer = modbus.buffer();

    buffer << address;

    USART1_IRQHandler();

    passed_ms(5);
    
    mock::CNDTR = 254;
    process.clear();
    modbus(reaction);

    BOOST_CHECK_EQUAL(process.str(),
        "Очистка буфера, прием данных" "\n"
    );

}

BOOST_AUTO_TEST_CASE (wrong_CRC)
{
    struct InReg {};
    struct OutReg{};

    interrupt_usart1.clear_subscribe();
    interrupt_DMA_channel4.clear_subscribe();

    const uint8_t  address   = 1;
          uint8_t  function  = 3;
          uint16_t first_reg = 5;
          uint16_t qty_reg   = 1;

    decltype(auto) modbus = Modbus_slave<InReg, OutReg>
                 ::make<mcu::Periph::USART1, 
                  mcu::PA9, mcu::PA10, mcu::PA11>(address, set);

    decltype(auto) buffer = modbus.buffer();
                  
    buffer << address;
    buffer << function; 
    buffer << first_reg;
    buffer << qty_reg; 

    uint16_t crc = 0x0B95; //wrong crc
    uint8_t crc_low = static_cast<uint8_t>(crc);
    uint8_t crc_high = crc >> 8;

    buffer << crc_low;
    buffer << crc_high;

    mock::CNDTR = 247;

    USART1_IRQHandler();

    passed_ms(5);
    process.clear();
    modbus(reaction);

    BOOST_CHECK_EQUAL(process.str(),
        "Очистка буфера, прием данных" "\n"
    );

}

BOOST_AUTO_TEST_CASE (wrong_adr)
{
    struct InReg {};
    struct OutReg{};

    interrupt_usart1.clear_subscribe();
    interrupt_DMA_channel4.clear_subscribe();

    const uint8_t  address   = 1;
          uint8_t  function  = 3;
          uint16_t first_reg = 5;
          uint16_t qty_reg   = 1;

    decltype(auto) modbus = Modbus_slave<InReg, OutReg>
                 ::make<mcu::Periph::USART1, 
                  mcu::PA9, mcu::PA10, mcu::PA11>(address, set);

    decltype(auto) buffer = modbus.buffer();
                  
    buffer << uint8_t(9);
    buffer << function; 
    buffer << first_reg;
    buffer << qty_reg; 

    uint16_t crc = 0x4395; 
    uint8_t crc_low = static_cast<uint8_t>(crc);
    uint8_t crc_high = crc >> 8;

    buffer << crc_low;
    buffer << crc_high;

    mock::CNDTR = 247;

    USART1_IRQHandler();

    process.clear();
    passed_ms(5);
    modbus(reaction);

    BOOST_CHECK_EQUAL(process.str(),
        "Очистка буфера, прием данных" "\n"
    );

}

BOOST_AUTO_TEST_CASE (wrong_func)
{
    struct InReg {};
    struct OutReg{};

    interrupt_usart1.clear_subscribe();
    interrupt_DMA_channel4.clear_subscribe();

    const uint8_t  address   = 1;
          uint8_t  function  = 9;
          uint16_t first_reg = 5;
          uint16_t qty_reg   = 1;

    decltype(auto) modbus = Modbus_slave<InReg, OutReg>
                 ::make<mcu::Periph::USART1, 
                  mcu::PA9, mcu::PA10, mcu::PA11>(address, set);

    decltype(auto) buffer = modbus.buffer();
                  
    buffer << address;
    buffer << function; 
    buffer << first_reg;
    buffer << qty_reg; 

    uint16_t crc = 0x0A0C; 
    uint8_t crc_low = static_cast<uint8_t>(crc);
    uint8_t crc_high = crc >> 8;

    buffer << crc_low;
    buffer << crc_high;

    mock::CNDTR = 247;

    USART1_IRQHandler();
    
    process.clear();
    passed_ms(5);
    modbus(reaction);

    crc = 0x5086;

    BOOST_CHECK_EQUAL(process.str(),
        "Передача данных: 01 89 01 86 50" "\n"
    );

    DMA1_Channel4_IRQHandler(); 
}

BOOST_AUTO_TEST_CASE (wrong_reg)
{
    struct InReg {};
    struct OutReg{
        uint16_t a = 6;
        uint16_t b = 5;
        uint16_t c = 4;
        uint16_t d = 3;
    };

    interrupt_usart1.clear_subscribe();
    interrupt_DMA_channel4.clear_subscribe();

    const uint8_t  address   = 3;
          uint8_t  function  = 3;
          uint16_t first_reg = 9;
          uint16_t qty_reg   = 1;

    decltype(auto) modbus = Modbus_slave<InReg, OutReg>
                 ::make<mcu::Periph::USART1, 
                  mcu::PA9, mcu::PA10, mcu::PA11>(address, set);

    decltype(auto) buffer = modbus.buffer();
                  
    buffer << address;
    buffer << function; 
    buffer << first_reg;
    buffer << qty_reg; 

    uint16_t crc = 0xEA55; 
    uint8_t crc_low = static_cast<uint8_t>(crc);
    uint8_t crc_high = crc >> 8;

    buffer << crc_low;
    buffer << crc_high;

    mock::CNDTR = 247;

    USART1_IRQHandler();

    process.clear();
    passed_ms(5);
    modbus(reaction);

    crc = 0xF100;

    BOOST_CHECK_EQUAL(process.str(),
        "Передача данных: 03 03 02 00 F1" "\n"
    );

    DMA1_Channel4_IRQHandler(); 
}

BOOST_AUTO_TEST_CASE (write)
{
    struct InReg{
        uint16_t a;
        uint16_t b;
        uint16_t c;
        int16_t  d;
        uint16_t f;
        uint16_t g;
    } in_reg;

    struct OutReg {};

    interrupt_usart1.clear_subscribe();
    interrupt_DMA_channel4.clear_subscribe();

    const uint8_t  address   = 7;
          uint8_t  function  = 16;
          uint8_t  qty_byte  = 2;
          uint16_t first_reg = 2;
          uint16_t qty_reg   = 1;
          uint16_t data      = 8;

    decltype(auto) modbus = Modbus_slave<InReg, OutReg>
                 ::make<mcu::Periph::USART1, 
                  mcu::PA9, mcu::PA10, mcu::PA11>(address, set);

    decltype(auto) buffer = modbus.buffer();
                  
    buffer << address;
    buffer << function; 
    buffer << first_reg;
    buffer << qty_reg;
    buffer << qty_byte;
    buffer << data;

    uint16_t crc = 0xD48D; 
    uint8_t crc_low = static_cast<uint8_t>(crc);
    uint8_t crc_high = crc >> 8;

    buffer << crc_low;
    buffer << crc_high;

    mock::CNDTR = 244;

    USART1_IRQHandler();

    process.clear();
    passed_ms(5);
    modbus(reaction);

    crc = 0x6FA0;

    BOOST_CHECK_EQUAL(process.str(),
        "Передача данных: 07 10 00 02 00 01 A0 6F" "\n"
    );

    DMA1_Channel4_IRQHandler(); 

    BOOST_CHECK_EQUAL(modbus.inRegs.c, 8);
    BOOST_CHECK_EQUAL(count, 1);
    count = 0;

    
    qty_byte  = 6;
    first_reg = 2;
    qty_reg   = 3;
    uint16_t data_1 = 5670;
    uint16_t data_2 = -34;
    uint16_t data_3 = 7;

    buffer << address;
    buffer << function; 
    buffer << first_reg;
    buffer << qty_reg;
    buffer << qty_byte;
    buffer << data_1;
    buffer << data_2;
    buffer << data_3;

    crc = 0x5154; 
    crc_low = static_cast<uint8_t>(crc);
    crc_high = crc >> 8;

    buffer << crc_low;
    buffer << crc_high;

    mock::CNDTR = 240;

    USART1_IRQHandler();

    process.clear();
    passed_ms(5);
    modbus.signed_[3] = true;
    modbus(reaction);

    crc = 0xAE21;

    BOOST_CHECK_EQUAL(process.str(),
        "Передача данных: 07 10 00 02 00 03 21 AE" "\n"
    );

    DMA1_Channel4_IRQHandler(); 

    BOOST_CHECK_EQUAL(modbus.inRegs.c, 5670);
    BOOST_CHECK_EQUAL(modbus.inRegs.d, -34);
    BOOST_CHECK_EQUAL(modbus.inRegs.f, 7);
    BOOST_CHECK_EQUAL(count, 3);
    count = 0;

}

BOOST_AUTO_TEST_CASE (check_value)
{
    struct InReg{
        int16_t  a;
        uint16_t b;
        uint16_t c;
        int16_t  d;
        uint16_t f;
        uint16_t g;
    } in_reg;

    struct OutReg {};

    interrupt_usart1.clear_subscribe();
    interrupt_DMA_channel4.clear_subscribe();

    const uint8_t  address   = 7;
          uint8_t  function  = 16;
          uint8_t  qty_byte  = 2;
          uint16_t first_reg = 0;
          uint16_t qty_reg   = 1;
          uint16_t data      = 789;

    decltype(auto) modbus = Modbus_slave<InReg, OutReg>
                 ::make<mcu::Periph::USART1, 
                  mcu::PA9, mcu::PA10, mcu::PA11>(address, set);

    decltype(auto) buffer = modbus.buffer();
                  
    buffer << address;
    buffer << function; 
    buffer << first_reg;
    buffer << qty_reg;
    buffer << qty_byte;
    buffer << data;

    uint16_t crc = 0xCF4C; 
    uint8_t crc_low = static_cast<uint8_t>(crc);
    uint8_t crc_high = crc >> 8;

    buffer << crc_low;
    buffer << crc_high;

    mock::CNDTR = 244;

    USART1_IRQHandler();

    process.clear();
    passed_ms(5);
    modbus.arInRegsMin[0] = -500;
    modbus.arInRegsMax[0] = 500;
    modbus(reaction);

    crc = 0xC08D;

    BOOST_CHECK_EQUAL(process.str(),
        "Передача данных: 07 10 03 8D C0" "\n"
    );

    DMA1_Channel4_IRQHandler(); 
}

BOOST_AUTO_TEST_CASE (not_tick)
{
    struct InReg {};
    struct OutReg{};

    interrupt_usart1.clear_subscribe();
    interrupt_DMA_channel4.clear_subscribe();

    const uint8_t  address   = 7;
          uint8_t  function  = 16;
          uint8_t  qty_byte  = 2;
          uint16_t first_reg = 2;
          uint16_t qty_reg   = 1;
          uint16_t data      = 8;

    decltype(auto) modbus = Modbus_slave<InReg, OutReg>
                 ::make<mcu::Periph::USART1, 
                  mcu::PA9, mcu::PA10, mcu::PA11>(address, set);

    decltype(auto) buffer = modbus.buffer();
                  
    buffer << address;
    buffer << function; 
    buffer << first_reg;
    buffer << qty_reg;
    buffer << qty_byte;
    buffer << data;

    uint16_t crc = 0xD48D; 
    uint8_t crc_low = static_cast<uint8_t>(crc);
    uint8_t crc_high = crc >> 8;

    buffer << crc_low;
    buffer << crc_high;

    mock::CNDTR = 244;

    USART1_IRQHandler();

    passed_ms(3); //время выдержки для modbus time = 3 < 4;
    modbus(reaction);// буфер остается непрочитанным

    BOOST_CHECK_EQUAL(*(buffer.end() - 2), crc_low);
    BOOST_CHECK_EQUAL(*(buffer.end() - 1), crc_high);

    tickUpdater.clear(); // отписываемся от прерываний tick
    buffer.clear();
}

BOOST_AUTO_TEST_CASE (not_uart_interrupt)
{
    struct InReg {};
    struct OutReg{};

    interrupt_usart1.clear_subscribe();
    interrupt_DMA_channel4.clear_subscribe();

    const uint8_t  address   = 7;
          uint8_t  function  = 16;
          uint8_t  qty_byte  = 2;
          uint16_t first_reg = 2;
          uint16_t qty_reg   = 1;
          uint16_t data      = 8;

    decltype(auto) modbus = Modbus_slave<InReg, OutReg>
                 ::make<mcu::Periph::USART1, 
                  mcu::PA9, mcu::PA10, mcu::PA11>(address, set);

    decltype(auto) buffer = modbus.buffer();
                  
    buffer << address;
    buffer << function; 
    buffer << first_reg;
    buffer << qty_reg;
    buffer << qty_byte;
    buffer << data;

    uint16_t crc = 0xD48D; 
    uint8_t crc_low = static_cast<uint8_t>(crc);
    uint8_t crc_high = crc >> 8;

    buffer << crc_low;
    buffer << crc_high;

    mock::CNDTR = 244;

    // USART1_IRQHandler(); // прерывания нет, подписки нет, notify нет

    passed_ms(5);//таймер не считает
    // буфер остается непрочитанным
    BOOST_CHECK_EQUAL(*(buffer.end() - 2), crc_low);
    BOOST_CHECK_EQUAL(*(buffer.end() - 1), crc_high);

    tickUpdater.clear();
    buffer.clear();
}

BOOST_AUTO_TEST_CASE (dma_interrupt)
{
    struct InReg{
        uint16_t a;
        uint16_t b;
        uint16_t c;
        int16_t  d;
        uint16_t f;
        uint16_t g;
    } in_reg;

    struct OutReg {};

    interrupt_usart1.clear_subscribe();
    interrupt_DMA_channel4.clear_subscribe();

    const uint8_t  address   = 7;
          uint8_t  function  = 16;
          uint8_t  qty_byte  = 2;
          uint16_t first_reg = 2;
          uint16_t qty_reg   = 1;
          uint16_t data      = 8;

    decltype(auto) modbus = Modbus_slave<InReg, OutReg>
                 ::make<mcu::Periph::USART1, 
                  mcu::PA9, mcu::PA10, mcu::PA11>(address, set);

    decltype(auto) buffer = modbus.buffer();
                  
    buffer << address;
    buffer << function; 
    buffer << first_reg;
    buffer << qty_reg;
    buffer << qty_byte;
    buffer << data;

    uint16_t crc = 0xD48D; 
    uint8_t crc_low = static_cast<uint8_t>(crc);
    uint8_t crc_high = crc >> 8;

    buffer << crc_low;
    buffer << crc_high;

    mock::CNDTR = 244;

    USART1_IRQHandler();

    process.clear();
    passed_ms(5);
    modbus(reaction);
    DMA1_Channel4_IRQHandler(); // в прерывании переходим на прием

    crc = 0x6FA0;

    BOOST_CHECK_EQUAL(process.str(),
        "Передача данных: 07 10 00 02 00 01 A0 6F" "\n"
        "Очистка буфера, прием данных"             "\n"
    );

    mock::CNDTR = 255;
}

BOOST_AUTO_TEST_CASE(new_message)
{
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

    const uint8_t  address   = 3;
          uint8_t  function  = 3;
          uint16_t first_reg = 9;
          uint16_t qty_reg   = 1;

    decltype(auto) modbus = Modbus_slave<InReg, OutReg>
                 ::make<mcu::Periph::USART1, 
                  mcu::PA9, mcu::PA10, mcu::PA11>(address, set);

    decltype(auto) buffer = modbus.buffer();

    auto passed_ms = [&](int n)
    {
        while (n--) {
            SysTick_Handler();
            modbus(reaction);
        }
    };
                  
    buffer << address;
    buffer << function; 
    buffer << first_reg;

    mock::CNDTR = 251;


    USART1_IRQHandler();
    process.clear();
    passed_ms(2);

    buffer << qty_reg;

    uint16_t crc = 0xEA55; 
    uint8_t crc_low = static_cast<uint8_t>(crc);
    uint8_t crc_high = crc >> 8;

    buffer << crc_low;
    buffer << crc_high;

    mock::CNDTR = 247;
    passed_ms(2);

    crc = 0xF100;

    BOOST_CHECK_EQUAL(process.str(),
        "Передача данных: 03 03 02 00 F1" "\n"
    );

    DMA1_Channel4_IRQHandler ();
}


BOOST_AUTO_TEST_SUITE_END()