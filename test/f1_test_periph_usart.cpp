#define BOOST_TEST_MODULE f1_test_periph_usart
#include <boost/test/unit_test.hpp>

#define F_CPU   72'000'000UL
#define STM32F103xB

#include <type_traits>
#include "mock_rcc.h"
#include "mock_afio.h"
#include "mock_gpio.h"
#include "meta.h"
#include "catch_static_assert.h"
#include "periph_usart.h"
#undef static_assert
// #include "timeout.h"
// #include "literals.h"

BOOST_AUTO_TEST_SUITE (test_suite_main)


mcu::USART usart;
auto& cmsis = usart.like_CMSIS();

BOOST_AUTO_TEST_CASE (like_CMSIS)
{
   auto same = std::is_same_v<std::remove_reference_t<decltype(cmsis)>, USART_TypeDef>;
   auto address_usart = reinterpret_cast<size_t>(&usart);
   auto address_CMSIS = reinterpret_cast<size_t>(&cmsis);
   BOOST_CHECK_EQUAL (same, true);
   BOOST_CHECK_EQUAL (address_usart, address_CMSIS);
   BOOST_CHECK_EQUAL (sizeof(usart), sizeof(cmsis));
}

BOOST_AUTO_TEST_CASE (set_parity)
{
   cmsis.CR1 = 0;
   usart.set (mcu::USART::Parity::odd);
   BOOST_CHECK_EQUAL (cmsis.CR1, USART_CR1_PS_Msk);

   usart.set(mcu::USART::Parity::even);
   BOOST_CHECK_EQUAL (cmsis.CR1, 0);
}

BOOST_AUTO_TEST_CASE (set_wake_method)
{
   cmsis.CR1 = 0;
   usart.set (mcu::USART::WakeMethod::address);
   BOOST_CHECK_EQUAL (cmsis.CR1, USART_CR1_WAKE_Msk);

   usart.set (mcu::USART::WakeMethod::idle);
   BOOST_CHECK_EQUAL (cmsis.CR1, 0);
}

BOOST_AUTO_TEST_CASE (set_data_bits)
{
   cmsis.CR1 = 0;
   usart.set (mcu::USART::DataBits::_9);
   BOOST_CHECK_EQUAL (cmsis.CR1, USART_CR1_M_Msk);

   usart.set (mcu::USART::DataBits::_8);
   BOOST_CHECK_EQUAL (cmsis.CR1, 0);
}

BOOST_AUTO_TEST_CASE (set_break_detection)
{
   cmsis.CR2 = 0;
   usart.set (mcu::USART::BreakDetection::_11bit);
   BOOST_CHECK_EQUAL (cmsis.CR2, USART_CR2_LBDL_Msk);

   usart.set(mcu::USART::BreakDetection::_10bit);
   BOOST_CHECK_EQUAL (cmsis.CR2, 0);
}

BOOST_AUTO_TEST_CASE (set_stop_bits)
{
   cmsis.CR2 = 0;
   usart.set (mcu::USART::StopBits::_1_5);
   BOOST_CHECK_EQUAL (cmsis.CR2, 
        USART_CR2_STOP_0
      | USART_CR2_STOP_1
   );

   usart.set (mcu::USART::StopBits::_2);
   BOOST_CHECK_EQUAL (cmsis.CR2, 
        USART_CR2_STOP_1
   );

   usart.set (mcu::USART::StopBits::_0_5);
   BOOST_CHECK_EQUAL (cmsis.CR2, 
        USART_CR2_STOP_0
   );

   usart.set(mcu::USART::StopBits::_1);
   BOOST_CHECK_EQUAL (cmsis.CR2, 
        0
   );
}

BOOST_AUTO_TEST_CASE (set_baudrate)
{
   cmsis.BRR = 0;
   auto& rcc = REF(RCC);
   rcc.set_APB1 (mcu::RCC::APBprescaler::APBnotdiv);
   rcc.set_APB2 (mcu::RCC::APBprescaler::APBnotdiv);
   usart.set (mcu::USART::Baudrate::BR115200, mcu::Periph::USART1);
   BOOST_CHECK_EQUAL (cmsis.BRR, F_CPU / 115200);

   rcc.set_APB2 (mcu::RCC::APBprescaler::APBdiv2);
   usart.set (mcu::USART::Baudrate::BR115200, mcu::Periph::USART1);
   BOOST_CHECK_EQUAL (cmsis.BRR, F_CPU / 115200 / 2);

   rcc.set_APB2 (mcu::RCC::APBprescaler::APBdiv16);
   usart.set (mcu::USART::Baudrate::BR115200, mcu::Periph::USART1);
   BOOST_CHECK_EQUAL (cmsis.BRR, F_CPU / 115200 / 16);

   usart.set (mcu::USART::Baudrate::BR115200, mcu::Periph::USART2);
   BOOST_CHECK_EQUAL (cmsis.BRR, F_CPU / 115200);

   rcc.set_APB1 (mcu::RCC::APBprescaler::APBdiv8);
   usart.set (mcu::USART::Baudrate::BR115200, mcu::Periph::USART2);
   BOOST_CHECK_EQUAL (cmsis.BRR, F_CPU / 115200 / 8);

   usart.set (mcu::USART::Baudrate::BR115200, mcu::Periph::USART3);
   BOOST_CHECK_EQUAL (cmsis.BRR, F_CPU / 115200 / 8);

   rcc.set_APB1 (mcu::RCC::APBprescaler::APBdiv2);
   usart.set (mcu::USART::Baudrate::BR115200, mcu::Periph::USART3);
   BOOST_CHECK_EQUAL (cmsis.BRR, F_CPU / 115200 / 2);
}

BOOST_AUTO_TEST_CASE (enable)
{
   cmsis.CR1 = 0;
   usart.enable();
   BOOST_CHECK_EQUAL (cmsis.CR1, USART_CR1_UE_Msk);
}

BOOST_AUTO_TEST_CASE (disable)
{
   cmsis.CR1 = USART_CR1_UE_Msk | USART_CR1_RE_Msk;
   usart.disable();
   BOOST_CHECK_EQUAL (cmsis.CR1, USART_CR1_RE_Msk);
}

BOOST_AUTO_TEST_CASE (rx_enable)
{
   cmsis.CR1 = 0;
   usart.rx_enable();
   BOOST_CHECK_EQUAL (cmsis.CR1, USART_CR1_RE_Msk);
}

BOOST_AUTO_TEST_CASE (rx_disable)
{
   cmsis.CR1 = USART_CR1_UE_Msk | USART_CR1_RE_Msk;
   usart.rx_disable();
   BOOST_CHECK_EQUAL (cmsis.CR1, USART_CR1_UE_Msk);
}

BOOST_AUTO_TEST_CASE (tx_enable)
{
   cmsis.CR1 = 0;
   usart.tx_enable();
   BOOST_CHECK_EQUAL (cmsis.CR1, USART_CR1_TE_Msk);
}

BOOST_AUTO_TEST_CASE (tx_disable)
{
   cmsis.CR1 = USART_CR1_UE_Msk | USART_CR1_TE_Msk;
   usart.tx_disable();
   BOOST_CHECK_EQUAL (cmsis.CR1, USART_CR1_UE_Msk);
}

BOOST_AUTO_TEST_CASE (rts_enable)
{
   cmsis.CR3 = 0;
   usart.rts_enable();
   BOOST_CHECK_EQUAL (cmsis.CR3, USART_CR3_RTSE_Msk);
}

BOOST_AUTO_TEST_CASE (rts_disable)
{
   cmsis.CR3 = USART_CR3_RTSE_Msk | USART_CR3_DMAR_Msk;
   usart.rts_disable();
   BOOST_CHECK_EQUAL (cmsis.CR3, USART_CR3_DMAR_Msk);
}

BOOST_AUTO_TEST_CASE (DMA_tx_enable)
{
   cmsis.CR3 = 0;
   usart.DMA_tx_enable();
   BOOST_CHECK_EQUAL (cmsis.CR3, USART_CR3_DMAT_Msk);
}

BOOST_AUTO_TEST_CASE (DMA_rx_enable)
{
   cmsis.CR3 = 0;
   usart.DMA_rx_enable();
   BOOST_CHECK_EQUAL (cmsis.CR3, USART_CR3_DMAR_Msk);
}

BOOST_AUTO_TEST_CASE (parity_enable)
{
   cmsis.CR1 = 0;
   usart.parity_enable();
   BOOST_CHECK_EQUAL (cmsis.CR1, USART_CR1_PCE_Msk);
}

BOOST_AUTO_TEST_CASE (parity_disable)
{
   cmsis.CR1 = USART_CR1_PCE_Msk | USART_CR1_UE_Msk;
   usart.parity_disable();
   BOOST_CHECK_EQUAL (cmsis.CR1, USART_CR1_UE_Msk);
}

BOOST_AUTO_TEST_CASE (enable_IDLE_interrupt)
{
   cmsis.CR1 = 0;
   usart.enable_IDLE_interrupt();
   BOOST_CHECK_EQUAL (cmsis.CR1, USART_CR1_IDLEIE_Msk);
}

BOOST_AUTO_TEST_CASE (is_IDLE_interrupt)
{
   cmsis.SR = 0;
   BOOST_CHECK_EQUAL (usart.is_IDLE_interrupt(), false);
   cmsis.SR = USART_SR_IDLE_Msk;
   BOOST_CHECK_EQUAL (usart.is_IDLE_interrupt(), true);
}

BOOST_AUTO_TEST_CASE (enable_tx_complete_interrupt)
{
   cmsis.CR1 = 0;
   usart.enable_tx_complete_interrupt();
   BOOST_CHECK_EQUAL (cmsis.CR1, USART_CR1_TCIE_Msk);
}

BOOST_AUTO_TEST_CASE (disable_tx_complete_interrupt)
{
   cmsis.CR1 = USART_CR1_TCIE_Msk | USART_CR1_UE_Msk;
   usart.disable_tx_complete_interrupt();
   BOOST_CHECK_EQUAL (cmsis.CR1, USART_CR1_UE_Msk);
}

BOOST_AUTO_TEST_CASE (is_tx_complete)
{
   cmsis.SR = 0;
   BOOST_CHECK_EQUAL (usart.is_tx_complete(), false);
   cmsis.SR = USART_SR_TC_Msk;
   BOOST_CHECK_EQUAL (usart.is_tx_complete(), true);
}

BOOST_AUTO_TEST_CASE (is_tx_complete_interrupt_enable)
{
   cmsis.CR1 = 0;
   BOOST_CHECK_EQUAL (usart.is_tx_complete_interrupt_enable(), false);
   cmsis.CR1 = USART_CR1_TCIE_Msk;
   BOOST_CHECK_EQUAL (usart.is_tx_complete_interrupt_enable(), true);
}

BOOST_AUTO_TEST_CASE (read_SR)
{
   cmsis.SR = 1;
   BOOST_CHECK_EQUAL (usart.read_SR(), 1);
   cmsis.SR = 0x0FFF;
   BOOST_CHECK_EQUAL (usart.read_SR(), 0x0FFF);
}

BOOST_AUTO_TEST_CASE (read_DR)
{
   cmsis.DR = 2;
   BOOST_CHECK_EQUAL (usart.read_DR(), 2);
   cmsis.DR = 0x0FFC;
   BOOST_CHECK_EQUAL (usart.read_DR(), 0x0FFC);
}

BOOST_AUTO_TEST_CASE (receive_data_adr)
{
   BOOST_CHECK_EQUAL (
      usart.receive_data_adr()
      , size_t(&usart) + offsetof(mcu::USART::CMSIS_type, DR)
   );
}

BOOST_AUTO_TEST_CASE (transmit_data_adr)
{
   BOOST_CHECK_EQUAL (
      usart.transmit_data_adr()
      , size_t(&usart) + offsetof(mcu::USART::CMSIS_type, DR)
   );
}

BOOST_AUTO_TEST_CASE (IRQn)
{
   BOOST_CHECK_EQUAL (usart.IRQn(mcu::Periph::USART1), USART1_IRQn);
   BOOST_CHECK_EQUAL (usart.IRQn(mcu::Periph::USART2), USART2_IRQn);
   BOOST_CHECK_EQUAL (usart.IRQn(mcu::Periph::USART3), USART3_IRQn);
}

BOOST_AUTO_TEST_CASE (stream)
{

   BOOST_CHECK (usart.default_stream<mcu::PA9 >() == mcu::Periph::DMA1_stream4);
   BOOST_CHECK (usart.default_stream<mcu::PB6 >() == mcu::Periph::DMA1_stream4);
   BOOST_CHECK (usart.default_stream<mcu::PA10>() == mcu::Periph::DMA1_stream5);
   BOOST_CHECK (usart.default_stream<mcu::PB7 >() == mcu::Periph::DMA1_stream5);
   BOOST_CHECK (usart.default_stream<mcu::PA2 >() == mcu::Periph::DMA1_stream7);
   BOOST_CHECK (usart.default_stream<mcu::PD5 >() == mcu::Periph::DMA1_stream7);
   BOOST_CHECK (usart.default_stream<mcu::PA3 >() == mcu::Periph::DMA1_stream6);
   BOOST_CHECK (usart.default_stream<mcu::PD6 >() == mcu::Periph::DMA1_stream6);
   BOOST_CHECK (usart.default_stream<mcu::PB10>() == mcu::Periph::DMA1_stream2);
   BOOST_CHECK (usart.default_stream<mcu::PC10>() == mcu::Periph::DMA1_stream2);
   BOOST_CHECK (usart.default_stream<mcu::PD8 >() == mcu::Periph::DMA1_stream2);
   BOOST_CHECK (usart.default_stream<mcu::PB11>() == mcu::Periph::DMA1_stream3);
   BOOST_CHECK (usart.default_stream<mcu::PC11>() == mcu::Periph::DMA1_stream3);
   BOOST_CHECK (usart.default_stream<mcu::PD9 >() == mcu::Periph::DMA1_stream3);

   STATIC_ASSERTATION_REQUIRED (usart.default_stream<mcu::PA1>(), "неверный аргумент шаблона class Pin");
}

BOOST_AUTO_TEST_CASE (pin_mode)
{

   BOOST_CHECK (usart.pin_mode<mcu::PA9 >() == mcu::PinMode::USART1_TX);
   BOOST_CHECK (usart.pin_mode<mcu::PB6 >() == mcu::PinMode::USART1_TX);
   BOOST_CHECK (usart.pin_mode<mcu::PA10>() == mcu::PinMode::USART1_RX);
   BOOST_CHECK (usart.pin_mode<mcu::PB7 >() == mcu::PinMode::USART1_RX);
   BOOST_CHECK (usart.pin_mode<mcu::PA2 >() == mcu::PinMode::USART2_TX);
   BOOST_CHECK (usart.pin_mode<mcu::PD5 >() == mcu::PinMode::USART2_TX);
   BOOST_CHECK (usart.pin_mode<mcu::PA3 >() == mcu::PinMode::USART2_RX);
   BOOST_CHECK (usart.pin_mode<mcu::PD6 >() == mcu::PinMode::USART2_RX);
   BOOST_CHECK (usart.pin_mode<mcu::PB10>() == mcu::PinMode::USART3_TX);
   BOOST_CHECK (usart.pin_mode<mcu::PC10>() == mcu::PinMode::USART3_TX);
   BOOST_CHECK (usart.pin_mode<mcu::PD8 >() == mcu::PinMode::USART3_TX);
   BOOST_CHECK (usart.pin_mode<mcu::PB11>() == mcu::PinMode::USART3_RX);
   BOOST_CHECK (usart.pin_mode<mcu::PC11>() == mcu::PinMode::USART3_RX);
   BOOST_CHECK (usart.pin_mode<mcu::PD9 >() == mcu::PinMode::USART3_RX);

   STATIC_ASSERTATION_REQUIRED (usart.pin_mode<mcu::PB2>(), "неверный аргумент шаблона class Pin");
}

BOOST_AUTO_TEST_CASE (pin_static_assert)
{
   STATIC_ASSERTATION_REQUIRED (
        WRAP(usart.pin_static_assert<mcu::Periph::USART1, mcu::PB2, mcu::PE9>())
      , "USART1 возможен только с парами пинов TX/PA9, RX/PA10 или TX/PB6, RX/PB7"
   );
   usart.pin_static_assert<mcu::Periph::USART1, mcu::PA9, mcu::PA10>();

   STATIC_ASSERTATION_REQUIRED (
        WRAP(usart.pin_static_assert<mcu::Periph::USART2, mcu::PA9, mcu::PA10>())
      , "USART2 возможен только с парами пинов TX/PA2, RX/PA3 или TX/PD5, RX/PD6"
   );
   usart.pin_static_assert<mcu::Periph::USART2, mcu::PA2, mcu::PA3>();

   STATIC_ASSERTATION_REQUIRED (
        WRAP(usart.pin_static_assert<mcu::Periph::USART3, mcu::PA9, mcu::PA10>())
      , "USART3 возможен только с парами пинов TX/PB10, RX/PB11 или TX/PC10, RX/PC11 или TX/PD8, RX/PD9"
   );
   usart.pin_static_assert<mcu::Periph::USART3, mcu::PD8, mcu::PD9>();
}

BOOST_AUTO_TEST_SUITE_END()
