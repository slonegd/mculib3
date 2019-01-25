#pragma once

#include <array>
#include "bit.h"
#include "timers.h"
#include "table_crc.h"
#include "ring_buffer.h"
#include "uart.h"
#include "interrupt.h"
#include "vector.h"
#include <cstring>



#if defined(USE_MOCK_UART)
using UART_ = mock::UART;
#else
using UART_ = ::UART;
#endif

namespace mcu {

template<size_t n, class Int = size_t>
class Auto_reset {
   Int value {0};
public:
   inline Int operator++(int)   { return value = (++value < n) ? value : 0; }
   inline operator Int() const  { return value; }
   inline Int operator= (Int v) { return value = v; }
};

using Array = std::array<uint8_t, 8>;


struct Register_base {

	enum class Error_code : uint8_t {
		wrong_func = 0x01,  wrong_reg = 0x02, 
		wrong_value = 0x03, wrong_addr = 0x09, 
		wrong_crc = 0x10,   wrong_qty_byte = 0x11, 
		time_out = 0x12
	};

   virtual       uint8_t  get_adr    () = 0;
   virtual       uint16_t get_adr_reg() = 0;
   virtual       void     set        (uint16_t data) = 0;
	// virtual       uint16_t operator=  (uint16_t v) = 0;
   virtual const Array get_request   () = 0;

	RingBuffer<10, Error_code> errors;
};

template<uint8_t address_, uint16_t address_register_, class T = uint16_t>
struct Register : Register_base
{
   static constexpr uint8_t address = address_;
   uint8_t get_adr() override {return address;}
   static constexpr uint16_t address_register = address_register_;
   uint16_t get_adr_reg() override {return address_register_;}
   T value;
   operator T() {return value;}
	// uint16_t operator= (uint16_t v) override {this -> value = v; return value;}
   void set(uint16_t data) override {(*reinterpret_cast<uint16_t*>(&value)) = data;}
   static constexpr  uint8_t request_base[]
		{ address_
		, 3
		, static_cast<uint8_t>(address_register_ << 8)
		, static_cast<uint8_t>(address_register_)
		, 0
		, 1
		};

	static constexpr Array request
		{ address_
		, 3
		, static_cast<uint8_t>(address_register_ << 8)
		, static_cast<uint8_t>(address_register_)
		, 0
		, 1
		, std::get<0>(CRC16(std::cbegin(request_base), std::cend(request_base)))
		, std::get<1>(CRC16(std::cbegin(request_base), std::cend(request_base)))
		};

   const Array get_request() override {return request;}

};

template <size_t qty>
class Modbus_master : TickSubscriber
{
	using Baudrate = UART_::Baudrate;
	
	enum State {
      wait_answer, request, pause
   } state {State::request};
	
	UART_& uart;
	Interrupt& interrupt_usart;
   Interrupt& interrupt_DMA_channel;
	Vector<Register_base*, qty> arr_register;
	Register_base* arr_register[qty];

	Auto_reset<qty> current {};

	uint16_t qty_reg{0};

	int time{0};
	int time_end_message;
	int modbus_time;
	int time_pause{0};
	size_t message_size;

	
	enum class Function   : uint8_t {read_03 = 0x03, write_16 = 0x10};

	void uartInterrupt()
   {
      if (uart.is_IDLE()) {
	  		time_end_message = time;
			message_size = uart.buffer.size();
		}
   }
   void dmaInterrupt()
   {
	   if (uart.is_tx_complete()) {
			uart.receive();
			tick_subscribe();
		}
   }

   void notify() override 
   {
      time++;
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

	void unsubscribe_modbus() {
		time = 0;
		tick_unsubscribe();
	}

	bool check_CRC();
	int  set_modbus_time (Baudrate);
	

public:

	size_t time_out; // time-out for master

   template<class ... Args>
   Modbus_master (UART_& uart, Interrupt& interrupt_usart, Interrupt& interrupt_DMA_channel, size_t time_out, UART_::Settings set, Args& ... args) 
		: uart    {uart}
		, interrupt_usart {interrupt_usart}
		, interrupt_DMA_channel {interrupt_DMA_channel}
		, arr_register {&args...}
		, modbus_time {set_modbus_time(set.baudrate)}
		, time_out {time_out}
		{}

	void operator() ();
	void get_answer ();
	auto& get_buffer() {return uart.buffer;}
};

template<class ... Args>
Modbus_master (UART_& uart, Interrupt& interrupt_usart, Interrupt& interrupt_DMA_channel, size_t time_out, UART_::Settings set, Args& ... args) 
				-> Modbus_master<sizeof... (args)>;

template <Periph usart, class TXpin,  class RXpin, class RTSpin, class LEDpin, class... Args> 
auto& make (size_t time_out, UART_::Settings set, Args&... args)
{
	auto interrupt_usart = usart == Periph::USART1 ? &interrupt_usart1 :
                          usart == Periph::USART2 ? &interrupt_usart2 :
                          usart == Periph::USART3 ? &interrupt_usart3 :
                          nullptr;
  	auto interrupt_dma   = usart == Periph::USART1 ? &interrupt_DMA_channel4 :
                          usart == Periph::USART2 ? &interrupt_DMA_channel7 : 
                          usart == Periph::USART3 ? &interrupt_DMA_channel2 :
                          nullptr;
	// auto u = UART::make<usart, TXpin, RXpin, RTSpin, LEDpin>();
	static Modbus_master modbus
	{UART_::make<usart, TXpin, RXpin, RTSpin, LEDpin>(), *interrupt_usart, *interrupt_dma, time_out, set, args...};

	return modbus;

}

template <size_t qty>
void Modbus_master<qty>::operator() ()
{
	switch (state) {
		case request:
			uart.buffer << arr_register[current]->get_request();
			uart.transmit();
			time_end_message = time_out;
			message_size = 0;
			state = State::wait_answer;
		break;
		case wait_answer:
			if (message_size < uart.buffer.size()) {
				time_end_message = time_out;
			}
			if (time >= time_out) {
				arr_register[current]->errors.push(Register_base::Error_code::time_out);
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

template <size_t qty>
void Modbus_master<qty>::get_answer()
{
	uint8_t  func, byte_qty, error_code;
	uint16_t data, crc;

	if (uart.buffer.front() != arr_register[current]->get_adr()) {
		arr_register[current]->errors.push(Register_base::Error_code::wrong_addr);
		return;
	}
	if (not check_CRC()) {
      arr_register[current]->errors.push(Register_base::Error_code::wrong_crc);
		return;
   }
	uart.buffer.pop_front(); // adr
	uart.buffer >> func;
	if(func != static_cast<uint8_t>(Function::read_03)) {
		if (is_high_bit(func)) {
			uart.buffer >> error_code;
			arr_register[current]->errors.push(static_cast<Register_base::Error_code>(error_code));
		} else
			arr_register[current]->errors.push(Register_base::Error_code::wrong_func);
		return;
	}
	uart.buffer >> byte_qty;
	uart.buffer >> data;
	if (sizeof(data) != byte_qty) {
		arr_register[current]->errors.push(Register_base::Error_code::wrong_qty_byte);
		return;
	}

	arr_register[current]->set(data);
}

template <size_t qty>
bool Modbus_master<qty>::check_CRC()
{
   uint8_t high = uart.buffer.pop_back();
	uint8_t low  = uart.buffer.pop_back();
   auto [low_, high_] = CRC16(uart.buffer.begin(), uart.buffer.end());
   return (high == high_) and (low == low_);
}

template<size_t qty>
int Modbus_master<qty>::set_modbus_time (Baudrate baudrate)
{
   return baudrate == Baudrate::BR9600  ? 4 :
          baudrate == Baudrate::BR14400 ? 3 :
          baudrate == Baudrate::BR19200 ? 2 :
          baudrate == Baudrate::BR28800 ? 2 : 1;
}

} //namespace mcu 


