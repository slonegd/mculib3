#pragma once

#include "timers.h"
#include "table_crc.h"
#if not defined (TEST)
   #include "uart.h"
   #include "interrupt.h"
   #include <cstring>
#endif

namespace mcu {

class Modbus_master : TickSubscriber
{
    size_t time {0}; //время для паузы между сообщениями

    enum class Function  : uint8_t {read_03 = 0x03, write_16 = 0x10};
    enum class Error_code : uint8_t {wrong_func = 0x01, wrong_reg = 0x02, wrong_value = 0x03};

    UART uart;
    Interrupt& interrupt_usart;
    Interrupt& interrupt_DMA_channel;

    uint8_t address;
    uint8_t func;
    uint16_t first_reg{0};
    uint16_t last_reg {0};
    uint16_t qty_reg  {0};
    uint8_t  qty_byte {0};
    uint16_t  data;
    uint16_t crc{0};

    template <Periph usart, class TXpin,  class RXpin, class RTSpin, class LEDpin> 
    static auto make (uint8_t address, UART::Settings set)
    {
       auto interrupt_usart = usart == Periph::USART1 ? &interrupt_usart1 :
                              usart == Periph::USART2 ? &interrupt_usart2 :
                              usart == Periph::USART3 ? &interrupt_usart3 :
                              nullptr;
       auto interrupt_dma   = usart == Periph::USART1 ? &interrupt_DMA_channel4 :
                              usart == Periph::USART2 ? &interrupt_DMA_channel7 : 
                              usart == Periph::USART3 ? &interrupt_DMA_channel2 :
                              nullptr;

       Modbus_slave<InRegs_t, OutRegs_t> modbus {address, UART::make<usart, TXpin, RXpin, RTSpin, LEDpin>(), *interrupt_usart, *interrupt_dma};

       modbus.uart.init(set);
       // modbus.uart.modbus_time(set.baudrate);
       return modbus;
    }

    void request(); // должна откуда-то принимать данные для формирования сообщения
    void operator() (); // должен как-то обрабатывать ответ и куда-то записывать полученное или повторный запрос

    void uartInterrupt()
    {
       if (uart.is_IDLE())
          subscribe();
    }
    void dmaInterrupt()
    {
       uart.start_receive();
    }

    void notify() override 
    {
       time++;
    }

    using Parent = Modbus_slave;

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
    } dma_ {*this};;
};

} //namespace mcu 