#define STM32F405xx
#define F_OSC   8000000UL
#define F_CPU   168000000UL

// #include <iostream>

#include "rcc.h"
#include "pin.h"
// #include "buttons.h"
#include "uart.h"


/// эта функция вызываеться первой в startup файле
extern "C" void init_clock ()
{
   // FLASH::set (FLASH::Latency::_5);

   mcu::RCC::make_reference()
      .on_HSE()
      .wait_HSE_ready()
      .set      (mcu::RCC::AHBprescaler::AHBnotdiv)
      .set_APB1 (mcu::RCC::APBprescaler::APBdiv4)
      .set_APB2 (mcu::RCC::APBprescaler::APBdiv2)
      .set      (mcu::RCC:: SystemClock::CS_PLL)
      .set_PLLM<4>()
      .set_PLLN<168>()
      .set      (mcu::RCC::     PLLPdiv::PLLdiv2)
      // .set_PLLQ<4>()
      .set      (mcu::RCC::   PLLsource::sHSE)
      .on_PLL()
      .wait_PLL_ready();
}


int main()
{
//    std::array <mcu::Pin, 2> pins {mcu::Pin::make<mcu::PA1, mcu::PinMode::Input>(),mcu::Pin::make<mcu::PA2, mcu::PinMode::Input>()};
   // mcu::RCC::make_reference().clock_enable<mcu::Periph::GPIOA>();
//    mcu::Buttons <2> buttons (pins);
   // auto buttons = mcu::Buttons::make<mcu::PA1, mcu::PA2>();
   // mcu::make_reference<mcu::Periph::USART6>()
   //    .set(mcu::USART::Parity::odd);
   //    .set(mcu::USART::WakeMethod::address)
   //    .set(mcu::USART::DataBits::_9)
   //    .set(mcu::USART::OverSample::by16)
   //    .set(mcu::USART::BreakDetection::_11bit)
   //    .set(mcu::USART::StopBits::_1_5)
   //    .send_byte(12)
   //    .enable()
   //    .disable()
   //    .rx_enable()
   //    .rx_disable()
   //    .tx_enable()
   //    .tx_disable()
   //    .rts_enable()
   //    .DMA_tx_enable()
   //    .DMA_rx_enable()
   //    .parity_enable()
   //    .enableIDLEinterrupt()
   //    .enable_tx_complete_interrupt()
   //    .disable_tx_complete_interrupt()
   //    .clear_interrupt_flags()
   //    .set (mcu::Baudrate::BR9600, 1000);


   // mcu::USART usart;

   // usart.is_IDLE_interrupt();
   // usart.is_tx_complete();

   
   // usart.clock_enable<mcu::Periph::USART1>();
   // usart.DMA_channel(mcu::Periph::USART3);
   // usart.IRQn(mcu::Periph::USART2);
   // usart.pin_mode(mcu::Periph::USART4);
   // usart.receive_data_adr();
   // usart.transmit_data_adr();
   // usart.is_rx_support<mcu::PA1, mcu::Periph::USART1>();
   // usart.is_tx_support<mcu::PC2, mcu::Periph::USART3>();
   // usart.is_rts_support<mcu::PG8, mcu::Periph::USART6>();
   // usart.clock<mcu::Periph::USART1>();

//    auto usart_ = mcu::UART::make<mcu::Periph::USART1, mcu::PA10, mcu::PA9, mcu::PA12>();

   
   while(1) {
      
   } // while(1) {

}