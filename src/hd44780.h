#pragma once

#include "pin.h"
#include "meta.h"
#include "delay.h"
#include "timers.h"
#include "bit_set.h"
#include "meta.h"
#include <initializer_list>
		
#if defined (USE_MOCK_DELAY)
#define NS mock
using namespace mock;
#else
#define NS
using namespace mcu;
#endif


class HD44780 : TickSubscriber
{
public:
    template <class RS, class RW, class E, class DB4, class DB5, class DB6, class DB7>
    static HD44780& make(const std::array<char, 80>& buffer);



private:
    using BSRR = std::pair<uint32_t, uint32_t>;

    enum Step {_1, _2, _3} step {Step::_1};

    Pin& rs;
    Pin& rw;
    Pin& e;
    GPIO& port;
    const std::array<char, 80 >& buffer;
    const std::array<BSRR, 256>& chars;
    const std::array<BSRR, 256>& command;
    uint32_t second;

    class Symbol_n {
    public:
        size_t operator++(int) { return table[index++ % 80]; }
    private:
        static constexpr auto symbol_n_function = [](size_t i) {
            return i < 20 ? i      :
                   i < 40 ? i + 20 :
                   i < 60 ? i - 20 : i;
        };
        static constexpr auto table = meta::generate<symbol_n_function,80>;
        size_t index {0};
    } index{};

    HD44780 (
        Pin& rs, Pin& rw, Pin& e, GPIO& port
        , const std::array<char, 80 >& buffer
        , const std::array<BSRR, 256>& chars
        , const std::array<BSRR, 256>& command
    ) : rs      {rs}
      , rw      {rw}
      , e       {e}
      , port    {port}
      , buffer  {buffer}
      , chars   {chars}
      , command {command}
    {}

    void init();
    void notify() override;
};











constexpr unsigned char convert_HD44780[64] =
{
    0x41,0xA0,0x42,0xA1,0xE0,0x45,0xA3,0xA4,
    0xA5,0xA6,0x4B,0xA7,0x4D,0x48,0x4F,0xA8,
    0x50,0x43,0x54,0xA9,0xAA,0x58,0xE1,0xAB,
    0xAC,0xE2,0xAD,0xAE,0xAD,0xAF,0xB0,0xB1,
    0x61,0xB2,0xB3,0xB4,0xE3,0x65,0xB6,0xB7,
    0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0x6F,0xBE,
    0x70,0x63,0xBF,0x79,0xE4,0x78,0xE5,0xC0,
    0xC1,0xE6,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7
};

template<class DB4, class DB5, class DB6, class DB7>
constexpr auto BSRR_command (char data)
{
    bit_set d {data};
    bit_set<uint32_t> bsrr_high {0};
    bit_set<uint32_t> bsrr_low  {0};

    bsrr_low. set (d[0] ? DB4::n : DB4::n+16);
    bsrr_low. set (d[1] ? DB5::n : DB5::n+16);
    bsrr_low. set (d[2] ? DB6::n : DB6::n+16);
    bsrr_low. set (d[3] ? DB7::n : DB7::n+16);
    bsrr_high.set (d[4] ? DB4::n : DB4::n+16);
    bsrr_high.set (d[5] ? DB5::n : DB5::n+16);
    bsrr_high.set (d[6] ? DB6::n : DB6::n+16);
    bsrr_high.set (d[7] ? DB7::n : DB7::n+16);

    return std::pair{bsrr_high.value, bsrr_low.value};
}

template<class DB4, class DB5, class DB6, class DB7>
constexpr auto BSRR_value (char data)
{
    data = data > 191 ? convert_HD44780[data - 192] : data; // TODO 191 192 magic num
    return BSRR_command<DB4, DB5, DB6, DB7>(data);
}







template <class RS, class RW, class E, class DB4, class DB5, class DB6, class DB7>
HD44780& HD44780::make(const std::array<char, 80>& buffer)
{
    static_assert (
        meta::all_is_same(DB4::periph, DB5::periph, DB6::periph, DB7::periph)
        , "Пины для шины экрана должны быть на одном порту"
    );

    static auto screen = HD44780 {
        Pin::template make<RS, mcu::PinMode::Output>(),
        Pin::template make<RW, mcu::PinMode::Output>(),
        Pin::template make<E,  mcu::PinMode::Output>(),
        mcu::make_reference<DB4::periph>(),
        buffer,
        meta::generate<BSRR_value<DB4, DB5, DB6, DB7>, 256>,
        meta::generate<BSRR_command<DB4, DB5, DB6, DB7>, 256>
    };

    make_pins<mcu::PinMode::Output, DB4, DB5, DB6, DB7>();

    screen.init();
    screen.tick_subscribe();

    return screen;
}


void HD44780::init()
{
    enum Set {
          _4_bit_mode     = 0x28
        , display_on      = 0x0C
        , dir_shift_right = 0x06
        , display_clear   = 0x01
        , set_to_zero     = 0x80
    };

    NS::Delay delay;
    
    auto strob_e = [&](){
        e = false;
        while(delay.us(100)) {}
        e = true;
        while(delay.us(100)) {}
    };

    auto instruction = [&](uint32_t action) {
        rs = false;
        port.atomic_write(command[action].first);
        strob_e();
        port.atomic_write(command[action].second);
        strob_e();
        while(delay.us(50)) {}
    };
    
    while(delay.ms(20)) {}
    rs = false;
    rw = false;
    e = true;
    port.atomic_write(command[0x03].second);
    strob_e();
    while(delay.ms(5)) {}
    port.atomic_write(command[0x03].second);
    strob_e();
    while(delay.us(100)) {}
    port.atomic_write(command[0x03].second);
    strob_e();
    while(delay.us(50)) {}
    port.atomic_write(command[0x02].second);
    strob_e();
    while(delay.us(50)) {}
    instruction (_4_bit_mode);
    instruction (display_on);
    instruction (dir_shift_right);
    instruction (display_clear);
    instruction (set_to_zero);
}

void HD44780::notify()
{
    switch (step) {
        case _1: {
            rw = false;
            rs = true;
            auto symbol = buffer[index++];
            auto [first, second] = chars[symbol];
            port.atomic_write(first);
            this->second = second;
            step = Step::_2;
        break;
        }
        case _2:
            e = true;
            e = false;
            port.atomic_write(second);
            step = Step::_3;
        break;
        case _3:
            e = true;
            e = false;
            step = Step::_1;
        break;
    }
}

#undef NS