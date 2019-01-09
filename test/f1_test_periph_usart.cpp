#define BOOST_TEST_MODULE f1_test_periph_usart
#include <boost/test/unit_test.hpp>

#define F_CPU   72'000'000UL
#define STM32F103xB

#include <type_traits>
#include "mock_rcc.h"
#include "mock_afio.h"
#include "mock_gpio.h"
#include "periph_usart.h"
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

BOOST_AUTO_TEST_CASE (send_byte)
{
   cmsis.DR = 0;
   usart.send_byte (0xFF);
   BOOST_CHECK_EQUAL (cmsis.DR, 0xFF);
   usart.send_byte (100);
   BOOST_CHECK_EQUAL (cmsis.DR, 100);
}

BOOST_AUTO_TEST_CASE (set_baudrate)
{
   cmsis.BRR = 0;
   usart.set (mcu::USART::Baudrate::BR115200, 10000000);
   BOOST_CHECK_EQUAL (cmsis.BRR, 0xFF);

}

BOOST_AUTO_TEST_SUITE_END()

/*

bool baudrate()
{
   CMSIS.BRR = 0;
   bool good {true};
   usart.set(mcu::USART::Baudrate::BR115200, 10000000);
   good &= bool (CMSIS.BRR & USART_BRR_DIV_Mantissa_Msk);
   usart.set(mcu::USART::Baudrate::BR57600, 20000000);
   good &= bool (CMSIS.BRR & USART_BRR_DIV_Mantissa_Msk);
   return good;
}

bool clock_enable()
{
   bool good {true};
   usart.clock_enable<mcu::Periph::USART1, mcu::Periph::TEST_RCC>();
   good &= mockRcc.good;
   return good;
}

bool enable()
{
   CMSIS.CR1 = 0;
   bool good {true};
   usart.enable();
   good &= bool (CMSIS.CR1 & USART_CR1_UE_Msk);
   return good;
}

bool disable()
{
   bool good {true};
   usart.disable();
   good &= not (CMSIS.CR1 & USART_CR1_UE_Msk);
   return good;
}

bool rx_enable()
{
   CMSIS.CR1 = 0;
   bool good {true};
   usart.rx_enable();
   good &= bool (CMSIS.CR1 & USART_CR1_RE_Msk);
   return good;
}

bool rx_disable()
{
   bool good {true};
   usart.rx_disable();
   good &= not (CMSIS.CR1 & USART_CR1_RE_Msk);
   return good;
}

bool tx_enable()
{
   CMSIS.CR1 = 0;
   bool good {true};
   usart.tx_enable();
   good &= bool (CMSIS.CR1 & USART_CR1_TE_Msk);
   return good;
}

bool tx_disable()
{
   bool good {true};
   usart.tx_disable();
   good &= not (CMSIS.CR1 & USART_CR1_TE_Msk);
   return good;
}

bool rts_enable()
{
   CMSIS.CR3 = 0;
   bool good {true};
   usart.rts_enable();
   good &= bool (CMSIS.CR3 & USART_CR3_RTSE_Msk);
   return good;
}

bool rts_disable()
{
   bool good {true};
   usart.rts_disable();
   good &= not (CMSIS.CR3 & USART_CR3_RTSE_Msk);
   return good;
}

bool DMA_tx_enable()
{
   CMSIS.CR3 = 0;
   bool good {true};
   usart.DMA_tx_enable();
   good &= bool (CMSIS.CR3 & USART_CR3_DMAT_Msk);
   return good;
}

bool DMA_rx_enable()
{
   CMSIS.CR3 = 0;
   bool good {true};
   usart.DMA_rx_enable();
   good &= bool (CMSIS.CR3 & USART_CR3_DMAR_Msk);
   return good;
}

bool parity_enable()
{
   CMSIS.CR1 = 0;
   bool good {true};
   usart.parity_enable();
   good &= bool (CMSIS.CR1 & USART_CR1_PCE_Msk);
   return good;
}

bool parity_disable()
{
   bool good {true};
   usart.parity_disable();
   good &= not (CMSIS.CR1 & USART_CR1_PCE_Msk);
   return good;
}

bool enable_IDLE_interrupt()
{
   CMSIS.CR1 = 0;
   bool good {true};
   usart.enable_IDLE_interrupt();
   good &= bool (CMSIS.CR1 & USART_CR1_IDLEIE_Msk);
   return good;
}

bool is_IDLE_interrupt()
{
   CMSIS.SR = 0;
   bool good {true};
   good &= not usart.is_IDLE_interrupt();
   CMSIS.SR |= USART_SR_IDLE;
   good &= usart.is_IDLE_interrupt();
   return good;
}

bool en_complete_interrupt()
{
   CMSIS.CR1 = 0;
   bool good {true};
   usart.enable_tx_complete_interrupt();
   good &= bool (CMSIS.CR1 & USART_CR1_TCIE_Msk);
   return good;
}

bool dis_complete_interrupt()
{
   bool good {true};
   usart.disable_tx_complete_interrupt();
   good &= not (CMSIS.CR1 & USART_CR1_TCIE_Msk);
   return good;
}

bool is_tx_complete()
{
   CMSIS.SR = 0;
   bool good {true};
   good &= not usart.is_tx_complete();
   CMSIS.SR |= USART_SR_TC;
   good &= usart.is_tx_complete();
   return good;
}

bool is_en_complete_interrupt()
{
   CMSIS.CR1 = 0;
   bool good {true};
   good &= not usart.is_tx_complete_interrupt_enable();
   usart.enable_tx_complete_interrupt();
   good &= usart.is_tx_complete_interrupt_enable();
   return good;
}

bool receive_data_adr()
{
   bool good{true};
   good &= bool ((reinterpret_cast<size_t>(&usart) + offsetof(mcu::USART::CMSIS_type, DR)) == usart.receive_data_adr());
   return good;
}

bool transmit_data_adr()
{
   bool good{true};
   good &= bool ((reinterpret_cast<size_t>(&usart) + offsetof(mcu::USART::CMSIS_type, DR)) == usart.receive_data_adr());
   return good;
}

// bool DMA_channel()
// {
//    bool good {true};
//    good &=  bool (mcu::DMA_bits::Channel::_4 == usart.DMA_channel(mcu::PinMode::USART1_TX))
//         and bool (mcu::DMA_bits::Channel::_5 == usart.DMA_channel(mcu::PinMode::USART1_RX))
//         and bool (mcu::DMA_bits::Channel::_7 == usart.DMA_channel(mcu::PinMode::USART2_TX))
//         and bool (mcu::DMA_bits::Channel::_6 == usart.DMA_channel(mcu::PinMode::USART2_RX))
//         and bool (mcu::DMA_bits::Channel::_2 == usart.DMA_channel(mcu::PinMode::USART3_TX))
//         and bool (mcu::DMA_bits::Channel::_3 == usart.DMA_channel(mcu::PinMode::USART3_RX));
//    return good;
// }

bool IRQn()
{
   bool good {true};
   good &= bool (USART1_IRQn == usart.IRQn(mcu::Periph::USART1))
       and not  (USART2_IRQn == usart.IRQn(mcu::Periph::USART1))
       and bool (USART2_IRQn == usart.IRQn(mcu::Periph::USART2))
       and not  (USART1_IRQn == usart.IRQn(mcu::Periph::USART2))
       and bool (USART3_IRQn == usart.IRQn(mcu::Periph::USART3))
       and not  (USART1_IRQn == usart.IRQn(mcu::Periph::USART3));
   return good;
}

bool number_clock()
{
   bool good {true};
   usart.clock<mcu::Periph::USART1, mcu::Periph::TEST_RCC>();
   good &=     mockRcc.APB2;
   good &= not mockRcc.APB1;
   mockRcc.reset_APB();

   usart.clock<mcu::Periph::USART2, mcu::Periph::TEST_RCC>();
   good &=     mockRcc.APB1;
   good &= not mockRcc.APB2;
   mockRcc.reset_APB();

   usart.clock<mcu::Periph::USART3, mcu::Periph::TEST_RCC>();
   good &=     mockRcc.APB1;
   good &= not mockRcc.APB2;
   mockRcc.reset_APB();
   return good;
}

int main()
{
   std::cout << '\n'
             << "Тесты класса USART для STM32F0:" << std::endl;

   auto test = [](auto s, auto f){
      std::cout << s << (f() ? "\033[32mпрошёл\033[0m" : "\033[31mпровален\033[0m") << std::endl;
   };

   test ("USART::make_reference           ", make_reference_);
   test ("USART::parity                   ", parity);
   test ("USART::wake_method              ", wake_method);
   test ("USART::data_bits                ", data_bits);
   test ("USART::break_detection          ", break_detection);
   test ("USART::stop_bits                ", stop_bits);
   test ("USART::send_byte                ", send_byte);
   test ("USART::baudrate                 ", baudrate);
   test ("USART::clock_enable             ", clock_enable);
   test ("USART::enable                   ", enable);
   test ("USART::disable                  ", disable);
   test ("USART::rx_enable                ", rx_enable);
   test ("USART::rx_disable               ", rx_disable);
   test ("USART::tx_enable                ", tx_enable);
   test ("USART::tx_disable               ", tx_disable);
   test ("USART::rts_enable               ", rts_enable);
   test ("USART::rts_disable              ", rts_disable);
   test ("USART::DMA_tx_enable            ", DMA_tx_enable);
   test ("USART::DMA_rx_enable            ", DMA_rx_enable);
   test ("USART::parity_enable            ", parity_enable);
   test ("USART::parity_disable           ", parity_disable);
   test ("USART::enable_IDLE_interrupt    ", enable_IDLE_interrupt);
   test ("USART::is_IDLE_interrupt        ", is_IDLE_interrupt);
   test ("USART::en_complete_interrupt    ", en_complete_interrupt);
   test ("USART::dis_complete_interrupt   ", dis_complete_interrupt);
   test ("USART::is_tx_complete           ", is_tx_complete);
   test ("USART::is_en_complete_interrupt ", is_en_complete_interrupt);
   test ("USART::clear_interrupt_flags    ", clear_interrupt_flags);
   test ("USART::receive_data_adr         ", receive_data_adr);
   test ("USART::transmit_data_adr        ", transmit_data_adr);
//    test ("USART::DMA_channel              ", DMA_channel);
   test ("USART::IRQn                     ", IRQn);
   test ("USART::number_clock             ", number_clock);

}
*/