#pragma once

#include <array>
#include "bit.h"
#include "timers.h"
#include "table_crc.h"
#include "ring_buffer.h"
#include "uart.h"
#include "interrupt.h"
#include "static_vector.h"
#include "modbus_common.h"
#include "reflect.h"
#include <cstring>

#if defined(USE_MOCK_UART)
using UART_ = mock::UART;
#else
using UART_ = UART;
#endif

struct Register_base {
    using byte = uint8_t;
    using word = uint16_t;
    const byte address;
    const word register_n;
    const Modbus_function function;
    word& data;
    bool disable {false};
    RingBuffer<10, Modbus_error_code> errors;

    Register_base (byte address, word register_n, Modbus_function function, word& data)
        : address{address}, register_n{register_n}, function{function}, data{data}
    {}

    virtual const std::tuple<byte,byte> get_crc() = 0;
};


template <
      uint8_t address_
    , Modbus_function f
    , uint16_t register_n_
    , class T = typename std::conditional_t<f == Modbus_function::force_coil_05, bool, uint16_t>
>
struct Register : Register_base
{
    Register() : Register_base{address_, register_n_, f, data} {}

    operator T&() {return value;}
    T& operator= (T v) {value = v; return value;}
    const std::tuple<byte,byte> get_crc() override;

private:
    union {
        T value;
        uint16_t data{};
    };

    template<bool on = false>
    static constexpr const std::tuple<byte,byte> crc();
};




template <size_t max_regs_qty>
class Modbus_master : TickSubscriber
{
public:
    using byte = uint8_t;
    using word = uint16_t;
    
    template<class Regs>
    Modbus_master (
          UART_& uart
        , Interrupt& interrupt_usart
        , Interrupt& interrupt_DMA_channel
        , int time_out
        , UART_::Settings set
        , Regs& regs
    ) : uart {uart}
      , interrupt_usart {interrupt_usart}
      , interrupt_DMA_channel {interrupt_DMA_channel}
      , modbus_time {set_modbus_time(set.baudrate)}
      , time_out {time_out}
    {
        auto tuple = reflect::make_tie(regs);
        auto i {0};
        meta::tuple_foreach<Register_base> (tuple, [this,&i] (auto p) {
            arr_register[i++] = p;
        });
        uart.init (set);
    }


    void operator() ();
    auto& get_buffer() const {return uart.buffer;}
    auto qty_slave () {return arr_register.size();}
    void get_answer ();


private:
    using Baudrate = UART_::Baudrate;
    
    enum State {
        wait_answer, request, pause
    } state {State::request};
    
    std::array<Register_base*, max_regs_qty> arr_register;
    UART_& uart;
    Interrupt& interrupt_usart;
    Interrupt& interrupt_DMA_channel;
    decltype(arr_register.begin()) current {arr_register.begin()};
    int time{0};
    int time_end_message;
    int modbus_time;
    int time_out; // in ms
    int time_pause{0};
    size_t message_size;
    bool search {false};
    volatile size_t errors_count    {0};
    volatile size_t transfers_count {0};

    void uartInterrupt();
    void dmaInterrupt();
    void notify() override { time++; }
    bool check_CRC();
    int  set_modbus_time (Baudrate);
    void unsubscribe_modbus()
    {
        time = 0;
        tick_unsubscribe();
    }

    using Parent = Modbus_master;

    struct uart_interrupt : Interrupting
    {
        Parent& parent;
        uart_interrupt (Parent& parent) : parent(parent) {
            parent.interrupt_usart.subscribe (this);
        }
        void interrupt() override {parent.uartInterrupt();} 
    } uart_ {*this};

    struct dma_interrupt : Interrupting
    {
        Parent& parent;
        dma_interrupt (Parent& parent) : parent(parent) {
            parent.interrupt_DMA_channel.subscribe (this);
        }
        void interrupt() override {parent.dmaInterrupt();} 
    } dma_ {*this};
};

template<class Regs>
Modbus_master (UART_&, Interrupt&, Interrupt&, size_t, UART_::Settings, Regs&)
    -> Modbus_master<reflect::member_count<Regs>()>;


template <mcu::Periph usart, class TXpin,  class RXpin, class RTSpin, class Regs>
auto& make_modbus_master (int time_out, UART_::Settings set, Regs& regs)
{
    static Modbus_master modbus {
          UART_::make<usart, TXpin, RXpin, RTSpin>()
        , get_interrupt<usart>()
        , get_interrupt<USART::default_stream<TXpin>()>()
        , time_out
        , set
        , regs
    };

    return modbus;
}










/*/////////////////////////////////////////////////////////////////////////////
 *
 *  Register
 *
 */////////////////////////////////////////////////////////////////////////////
template<uint8_t address_, Modbus_function f, uint16_t register_n_, class T>
const std::tuple<uint8_t,uint8_t> Register<address_,f,register_n_,T>::get_crc()
{
    if constexpr (f == Modbus_function::read_03) {
        return crc();
    } else if constexpr (f == Modbus_function::force_coil_05) {
        return bool(data) ? crc<true>() : crc<false>();
    } else {
        return std::tuple<byte,byte>{};
    }
}


template<uint8_t address_, Modbus_function f, uint16_t register_n_, class T>
template<bool on>
constexpr const std::tuple<uint8_t,uint8_t> Register<address_,f,register_n_,T>::crc()
{
    if constexpr (f == Modbus_function::read_03 or f == Modbus_function::force_coil_05) {
        byte by = f == Modbus_function::force_coil_05 and on ? 0x55 :
                  f == Modbus_function::read_03              ? 1 : 0;
        auto res = Static_vector<byte, 8> {
                address_
            , byte(f)
            , static_cast<byte>(register_n_ << 8)
            , static_cast<byte>(register_n_)
            , byte(0)
            , by
        };
        return CRC16(res.cbegin(), res.cend());
    } else {
        static_assert(always_false_v<decltype(function)>, "Допиши функцию модбас");
    }
}





/*/////////////////////////////////////////////////////////////////////////////
 *
 *  Modbus_master
 *
 */////////////////////////////////////////////////////////////////////////////
template <size_t max_regs_qty>
void Modbus_master<max_regs_qty>::operator() ()
{
    switch (state) {
        case request: {
                current = std::find_if (current, arr_register.end(), [](auto reg){
                    return not reg->disable;
                });
                if (current == arr_register.end()) {
                    current = arr_register.begin();
                    break;
                }

                auto reg = *current;

                uart.buffer.clear();
                uart.buffer << reg->address;
                uart.buffer << static_cast<byte>(reg->function);
                uart.buffer << static_cast<byte>(reg->register_n << 8);
                uart.buffer << static_cast<byte>(reg->register_n);
                uart.buffer << byte(0);
                if (reg->function == Modbus_function::read_03 or reg->function == Modbus_function::write_16) {
                    uart.buffer << byte(1);
                } else if (reg->function == Modbus_function::force_coil_05) {
                    uart.buffer << (bool(reg->data) ? byte(0x55) : byte(0));
                }
                if (reg->function == Modbus_function::write_16) {
                    uart.buffer << static_cast<byte>(reg->data << 8) << static_cast<byte>(reg->data);
                }
                if (reg->function == Modbus_function::read_03 or reg->function == Modbus_function::force_coil_05) {
                    auto [crc_lo, crc_hi] = reg->get_crc();
                    uart.buffer << crc_lo << crc_hi;
                } else if (reg->function == Modbus_function::write_16) {
                    auto [crc_lo, crc_hi] = CRC16(uart.buffer.begin(), uart.buffer.end());
                    uart.buffer << crc_lo << crc_hi;
                }
                time_end_message = time_out;
                message_size = 0;
                transfers_count++;
                uart.transmit();
                state = State::wait_answer;
            break;
        }
        case wait_answer:
            if (message_size < uart.buffer.size()) {
                time_end_message = time_out;
            }
            if (time >= time_out) {
                (*current)->errors.push(Modbus_error_code::time_out);
                errors_count++;
                time_pause = time;
                state = State::pause;
            }
            if (time - time_end_message >= modbus_time) {
                get_answer();
                time_pause = time;
                state = State::pause;
            }
        break;
        case pause:
            if (time - 10 >= time_pause) {
                unsubscribe_modbus();
                current++;
                state = State::request;
            }
        break;
    }
}

template <size_t max_regs_qty>
void Modbus_master<max_regs_qty>::get_answer()
{
    auto reg = *current;
    if (uart.buffer.front() != reg->address) {
        reg->errors.push (Modbus_error_code::wrong_addr);
        errors_count++;
        return;
    }

    if (not check_CRC()) {
        reg->errors.push (Modbus_error_code::wrong_crc);
        errors_count++;
        return;
    }

    uart.buffer.pop_front(); // adr
    uint8_t func;
    uart.buffer >> func;
    if (is_high_bit(func)) {
        uint8_t error_code;
        uart.buffer >> error_code;
        reg->errors.push (static_cast<Modbus_error_code>(error_code));
        errors_count++;
        return;
    }

    if (reg->function == Modbus_function::read_03) {
        uint8_t  byte_qty;
        uart.buffer >> byte_qty;
        uart.buffer >> reg->data;
    }
}

template <size_t max_regs_qty>
bool Modbus_master<max_regs_qty>::check_CRC()
{
    if (uart.buffer.size() > 2) {
        uint8_t high = uart.buffer.pop_back();
        uint8_t low  = uart.buffer.pop_back();
        auto [low_, high_] = CRC16(uart.buffer.begin(), uart.buffer.end());
        return (high == high_) and (low == low_);
    } else {
        return false;
    }
    
}

template <size_t max_regs_qty>
void Modbus_master<max_regs_qty>::uartInterrupt()
{
    if (uart.is_rx_IDLE()) {
        time_end_message = time;
        message_size = uart.buffer.size();
    }
}

template <size_t max_regs_qty>
void Modbus_master<max_regs_qty>::dmaInterrupt()
{
    if (uart.is_tx_complete()) {
        uart.receive();
        tick_subscribe();
    }
}

template<size_t max_regs_qty>
int Modbus_master<max_regs_qty>::set_modbus_time (Baudrate baudrate)
{
    return baudrate == Baudrate::BR9600  ? 4 :
           baudrate == Baudrate::BR14400 ? 3 :
           baudrate == Baudrate::BR19200 ? 2 :
           baudrate == Baudrate::BR28800 ? 2 : 1;
}




