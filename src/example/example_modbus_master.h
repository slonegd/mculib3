#pragma once
#include "modbus_master.h"
#include "literals.h"

namespace mcu::example {

void modbus_master() {
    constexpr bool parity_enable {true};
    constexpr int timeout {50_ms};
    UART::Settings set {
          not parity_enable
        , UART::Parity::even
        , UART::DataBits::_8
        , UART::StopBits::_1
        , UART::Baudrate::BR9600
    };

    struct Flags {
        bool f1  :1;
        bool f2  :1;
        uint16_t :14;
    };

    struct Regs {
        Register<1, Modbus_function::read_03      , 2> temp;
        Register<3, Modbus_function::force_coil_05, 7> uf;
        Register<2, Modbus_function::write_16     , 4> time;
        Register<2, Modbus_function::read_03      , 3, Flags> flags;
    } regs;

    decltype(auto) master = make_modbus_master <
          mcu::Periph::USART1
        , mcu::PA9
        , mcu::PA10
        , mcu::PB1
    > (timeout, set, regs);

    while (1) {
        master();

        // либо делать ссылку
        Flags& flags = regs.flags;
        regs.time.disable = flags.f1;

        // либо приводить явно
        regs.uf.disable = Flags(regs.flags).f2;

        if (regs.temp > 50)
            regs.time = 10;
    }
}

} // namespace example {