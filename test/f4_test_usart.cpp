#define STM32F405xx
#define F_CPU   168000000UL

#include "usart.h"
#include <iostream>
#include <type_traits>

mcu::USART usart;
auto& CMSIS = *reinterpret_cast<mcu::USART::CMSIS_type*>(&usart);

bool DR_read {false};
bool SR_read {false};
bool clear_interrupt_flags()
{
   class uint32_test1 {
      __IO uint32_t DR;
   public:

      operator __IO uint32_t&() { DR_read = true; return DR; }
      uint32_t operator=(uint32_t v) { DR = v; return DR; }
   };

   struct uint32_test2 : public mcu::USART_bits::SR {
      uint32_t read() { SR_read = true; DR_read = false; return *reinterpret_cast<uint32_t*>(this); }
   };

   class USART_test
   {
   public:
           uint32_test2         SR;   // USART Status register,                   offset: 0x00
           uint32_test1         DR;   // USART Data register,                     offset: 0x04
      __IO uint32_t             BRR;  // USART Baud rate register,                offset: 0x08
      __IO mcu::USART_bits::CR1 CR1;  // USART Control register 1,                offset: 0x0C
      __IO mcu::USART_bits::CR2 CR2;  // USART Control register 2,                offset: 0x10
      __IO mcu::USART_bits::CR3 CR3;  // USART Control register 3,                offset: 0x14
      __IO uint32_t             GTPR; // USART Guard time and prescaler register, offset: 0x18
   };

   mcu::USART_<USART_test> usart;
   auto& CMSIS = *reinterpret_cast<mcu::USART_<USART_test>::CMSIS_type*>(&usart);

   bool good {true};
   usart.clear_interrupt_flags();
   good &= DR_read and SR_read;
   return good;
}

bool make_reference()
{
   bool good {true};
   auto& usart1 {mcu::make_reference<mcu::Periph::USART1>()};
   good &= reinterpret_cast<size_t>(&usart1) == USART1_BASE 
      and std::is_same_v<std::remove_reference_t<decltype(usart1)>, mcu::USART>;
   
   auto& usart2 {mcu::make_reference<mcu::Periph::USART2>()};
   good &= reinterpret_cast<size_t>(&usart2) == USART2_BASE 
      and std::is_same_v<std::remove_reference_t<decltype(usart2)>, mcu::USART>;

   auto& usart3 {mcu::make_reference<mcu::Periph::USART3>()};
   good &= reinterpret_cast<size_t>(&usart3) == USART3_BASE 
      and std::is_same_v<std::remove_reference_t<decltype(usart3)>, mcu::USART>;

   auto& usart4 {mcu::make_reference<mcu::Periph::USART4>()};
   good &= reinterpret_cast<size_t>(&usart4) == UART4_BASE 
      and std::is_same_v<std::remove_reference_t<decltype(usart4)>, mcu::USART>;

   auto& usart5 {mcu::make_reference<mcu::Periph::USART5>()};
   good &= reinterpret_cast<size_t>(&usart5) == UART5_BASE 
      and std::is_same_v<std::remove_reference_t<decltype(usart5)>, mcu::USART>;

   auto& usart6 {mcu::make_reference<mcu::Periph::USART6>()};
   good &= reinterpret_cast<size_t>(&usart6) == USART6_BASE 
      and std::is_same_v<std::remove_reference_t<decltype(usart6)>, mcu::USART>;
   return good;
}

bool parity()
{
   CMSIS.CR1 = 0;
   bool good {true};
   usart.set(mcu::USART::Parity::odd);
   good &= bool (CMSIS.CR1 & USART_CR1_PS_Msk);
   usart.set(mcu::USART::Parity::even);
   good &= not (CMSIS.CR1 & USART_CR1_PS_Msk);
   return good;
}

bool wake_method()
{
   CMSIS.CR1 = 0;
   bool good {true};
   usart.set(mcu::USART::WakeMethod::idle);
   good &= not (CMSIS.CR1 & USART_CR1_WAKE_Msk);
   usart.set(mcu::USART::WakeMethod::address);
   good &= bool (CMSIS.CR1 & USART_CR1_WAKE_Msk);
   return good;
}

bool data_bits()
{
   CMSIS.CR1 = 0;
   bool good {true};
   usart.set(mcu::USART::DataBits::_9);
   good &= bool (CMSIS.CR1 & USART_CR1_M_Msk);
   usart.set(mcu::USART::DataBits::_8);
   good &= not (CMSIS.CR1 & USART_CR1_M_Msk);
   return good;
}

bool over_sample()
{
   CMSIS.CR1 = 0;
   bool good {true};
   usart.set(mcu::USART::OverSample::by16);
   good &= bool (CMSIS.CR1 & USART_CR1_OVER8_Msk);
   usart.set(mcu::USART::OverSample::by8);
   good &= not (CMSIS.CR1 & USART_CR1_OVER8_Msk);
   return good;
}

bool break_detection()
{
   CMSIS.CR2 = 0;
   bool good {true};
   usart.set(mcu::USART::BreakDetection::_11bit);
   good &= bool (CMSIS.CR2 & USART_CR2_LBDL_Msk);
   usart.set(mcu::USART::BreakDetection::_10bit);
   good &= not (CMSIS.CR2 & USART_CR2_LBDIE_Msk);
   return good;
}

bool stop_bits()
{
   CMSIS.CR2 = 0;
   bool good {true};
   usart.set(mcu::USART::StopBits::_1_5);
   good &= bool (CMSIS.CR2 & USART_CR2_STOP_0)
       and      (CMSIS.CR2 & USART_CR2_STOP_1);
   usart.set(mcu::USART::StopBits::_2);
   good &= not  (CMSIS.CR2 & USART_CR2_STOP_0)
       and bool (CMSIS.CR2 & USART_CR2_STOP_1);
   usart.set(mcu::USART::StopBits::_0_5);
   good &= bool (CMSIS.CR2 & USART_CR2_STOP_0)
       and not  (CMSIS.CR2 & USART_CR2_STOP_1);
   usart.set(mcu::USART::StopBits::_1);
   good &= not  (CMSIS.CR2 & USART_CR2_STOP_0)
       and not  (CMSIS.CR2 & USART_CR2_STOP_1);
   return good;
}

bool send_byte()
{
   CMSIS.DR = 0;
   bool good {true};
   usart.send_byte(0xFF);
   good &= bool (CMSIS.DR & USART_DR_DR_Msk);
   usart.send_byte(100);
   good &= bool (CMSIS.DR & USART_DR_DR_Msk);
   return good;
}

bool baudrate()
{
   CMSIS.BRR = 0;
   bool good {true};
   usart.set(mcu::Baudrate::BR115200, 10000000);
   good &= bool (CMSIS.BRR & USART_BRR_DIV_Mantissa_Msk);
   usart.set(mcu::Baudrate::BR57600, 20000000);
   good &= bool (CMSIS.BRR & USART_BRR_DIV_Mantissa_Msk);
   return good;
}

                  size_t make();

                  struct MockRCC {
                     bool good {false};
                     bool APB1 {false};
                     bool APB2 {false};
                     static MockRCC& make_reference(){return *reinterpret_cast<MockRCC*>(make());}
                     template <mcu::Periph p> void clock_enable(){good = true;}
                     size_t get_APB1_clock () { return APB1 = true; }
                     size_t get_APB2_clock () { return APB2 = true; }
                     void reset_APB(){APB1 = false; APB2 = false;}
                  }mockRcc;

                  size_t make ()
                  {
                     return reinterpret_cast<size_t>(&mockRcc);
                  }

bool clock_enable()
{
   bool good {true};
   usart.clock_enable<mcu::Periph::USART1, MockRCC>();
   good &= MockRCC::make_reference().good;
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

bool DMA_channel()
{
   bool good {true};
   good &= bool (mcu::Channel::_4 == usart.DMA_channel(mcu::Periph::USART1))
       and not  (mcu::Channel::_5 == usart.DMA_channel(mcu::Periph::USART1))
       and bool (mcu::Channel::_4 == usart.DMA_channel(mcu::Periph::USART2))
       and not  (mcu::Channel::_5 == usart.DMA_channel(mcu::Periph::USART2))
       and bool (mcu::Channel::_4 == usart.DMA_channel(mcu::Periph::USART3))
       and not  (mcu::Channel::_5 == usart.DMA_channel(mcu::Periph::USART3))
       and bool (mcu::Channel::_4 == usart.DMA_channel(mcu::Periph::USART4))
       and not  (mcu::Channel::_5 == usart.DMA_channel(mcu::Periph::USART4))
       and bool (mcu::Channel::_4 == usart.DMA_channel(mcu::Periph::USART5))
       and not  (mcu::Channel::_5 == usart.DMA_channel(mcu::Periph::USART5))
       and bool (mcu::Channel::_5 == usart.DMA_channel(mcu::Periph::USART6))
       and not  (mcu::Channel::_4 == usart.DMA_channel(mcu::Periph::USART6));
   return good;
}

bool IRQn()
{
   bool good {true};
   good &= bool (USART1_IRQn == usart.IRQn(mcu::Periph::USART1))
       and not  (USART2_IRQn == usart.IRQn(mcu::Periph::USART1))
       and bool (USART2_IRQn == usart.IRQn(mcu::Periph::USART2))
       and not  (USART1_IRQn == usart.IRQn(mcu::Periph::USART2))
       and bool (USART3_IRQn == usart.IRQn(mcu::Periph::USART3))
       and not  (USART1_IRQn == usart.IRQn(mcu::Periph::USART3))
       and bool (UART4_IRQn  == usart.IRQn(mcu::Periph::USART4))
       and not  (UART5_IRQn  == usart.IRQn(mcu::Periph::USART4))
       and bool (UART5_IRQn  == usart.IRQn(mcu::Periph::USART5))
       and not  (UART4_IRQn  == usart.IRQn(mcu::Periph::USART5))
       and bool (USART6_IRQn == usart.IRQn(mcu::Periph::USART6))
       and not  (USART1_IRQn == usart.IRQn(mcu::Periph::USART6));
   return good;
}

bool number_clock()
{
   bool good {true};
   usart.clock<mcu::Periph::USART1, MockRCC>();
   good &=     MockRCC::make_reference().APB2;
   good &= not MockRCC::make_reference().APB1;
   MockRCC::make_reference().reset_APB();

   usart.clock<mcu::Periph::USART2, MockRCC>();
   good &=     MockRCC::make_reference().APB1;
   good &= not MockRCC::make_reference().APB2;
   MockRCC::make_reference().reset_APB();

   usart.clock<mcu::Periph::USART3, MockRCC>();
   good &=     MockRCC::make_reference().APB1;
   good &= not MockRCC::make_reference().APB2;
   MockRCC::make_reference().reset_APB();

   usart.clock<mcu::Periph::USART4, MockRCC>();
   good &=     MockRCC::make_reference().APB1;
   good &= not MockRCC::make_reference().APB2;
   MockRCC::make_reference().reset_APB();

   usart.clock<mcu::Periph::USART5, MockRCC>();
   good &=     MockRCC::make_reference().APB1;
   good &= not MockRCC::make_reference().APB2;
   MockRCC::make_reference().reset_APB();

   usart.clock<mcu::Periph::USART6, MockRCC>();
   good &=     MockRCC::make_reference().APB2;
   good &= not MockRCC::make_reference().APB1;
   MockRCC::make_reference().reset_APB();
   return good;
}

bool is_rx_support()
{
   bool good {true};
   good &=     usart.is_rx_support<mcu::PA10, mcu::Periph::USART1>();
   good &=     usart.is_rx_support<mcu::PB7,  mcu::Periph::USART1>();
   good &= not usart.is_rx_support<mcu::PA1,  mcu::Periph::USART1>();

   good &=     usart.is_rx_support<mcu::PA3,  mcu::Periph::USART2>();
   good &=     usart.is_rx_support<mcu::PD6,  mcu::Periph::USART2>();
   good &= not usart.is_rx_support<mcu::PA10, mcu::Periph::USART2>();

   good &=     usart.is_rx_support<mcu::PB11, mcu::Periph::USART3>();
   good &=     usart.is_rx_support<mcu::PD9,  mcu::Periph::USART3>();
   good &=     usart.is_rx_support<mcu::PC11, mcu::Periph::USART3>();
   good &= not usart.is_rx_support<mcu::PC10, mcu::Periph::USART3>();

   good &=     usart.is_rx_support<mcu::PA1,  mcu::Periph::USART4>();
   good &=     usart.is_rx_support<mcu::PC11, mcu::Periph::USART4>();
   good &= not usart.is_rx_support<mcu::PC10, mcu::Periph::USART4>();

   good &=     usart.is_rx_support<mcu::PD2,  mcu::Periph::USART5>();
   good &= not usart.is_rx_support<mcu::PC10, mcu::Periph::USART5>();

   good &=     usart.is_rx_support<mcu::PC7,  mcu::Periph::USART6>();
   good &=     usart.is_rx_support<mcu::PG9,  mcu::Periph::USART6>();
   good &= not usart.is_rx_support<mcu::PC10, mcu::Periph::USART6>();
   return good; 
}

bool is_tx_support()
{
   bool good {true};
   good &=     usart.is_tx_support<mcu::PA9,  mcu::Periph::USART1>();
   good &=     usart.is_tx_support<mcu::PB6,  mcu::Periph::USART1>();
   good &= not usart.is_tx_support<mcu::PA1,  mcu::Periph::USART1>();

   good &=     usart.is_tx_support<mcu::PA2,  mcu::Periph::USART2>();
   good &=     usart.is_tx_support<mcu::PD5,  mcu::Periph::USART2>();
   good &= not usart.is_tx_support<mcu::PA10, mcu::Periph::USART2>();

   good &=     usart.is_tx_support<mcu::PB10, mcu::Periph::USART3>();
   good &=     usart.is_tx_support<mcu::PD8,  mcu::Periph::USART3>();
   good &=     usart.is_tx_support<mcu::PC10, mcu::Periph::USART3>();
   good &= not usart.is_tx_support<mcu::PC11, mcu::Periph::USART3>();

   good &=     usart.is_tx_support<mcu::PA0,  mcu::Periph::USART4>();
   good &=     usart.is_tx_support<mcu::PC10, mcu::Periph::USART4>();
   good &= not usart.is_tx_support<mcu::PC11, mcu::Periph::USART4>();

   good &=     usart.is_tx_support<mcu::PC12, mcu::Periph::USART5>();
   good &= not usart.is_tx_support<mcu::PC10, mcu::Periph::USART5>();

   good &=     usart.is_tx_support<mcu::PC6,  mcu::Periph::USART6>();
   good &=     usart.is_tx_support<mcu::PG14, mcu::Periph::USART6>();
   good &= not usart.is_tx_support<mcu::PC10, mcu::Periph::USART6>();
   return good; 
}

bool is_rts_support()
{
   bool good {true};
   good &=     usart.is_rts_support<mcu::PA12, mcu::Periph::USART1>();
   good &= not usart.is_rts_support<mcu::PA1,  mcu::Periph::USART1>();

   good &=     usart.is_rts_support<mcu::PD4,  mcu::Periph::USART2>();
   good &= not usart.is_rts_support<mcu::PA10, mcu::Periph::USART2>();

   good &=     usart.is_rts_support<mcu::PB14, mcu::Periph::USART3>();
   good &=     usart.is_rts_support<mcu::PD12, mcu::Periph::USART3>();
   good &= not usart.is_rts_support<mcu::PC11, mcu::Periph::USART3>();

   good &=     usart.is_rts_support<mcu::PG8,  mcu::Periph::USART6>();
   good &=     usart.is_rts_support<mcu::PG12, mcu::Periph::USART6>();
   good &= not usart.is_rts_support<mcu::PC10, mcu::Periph::USART6>();
   return good; 
}

int main()
{
   std::cout << '\n'
             << "Тесты класса USART для STM32F0:" << std::endl;

   auto test = [](auto s, auto f){
      std::cout << s << (f() ? "\033[32mпрошёл\033[0m" : "\033[31mпровален\033[0m") << std::endl;
   };

   test ("USART::make_reference           ", make_reference);
   test ("USART::parity                   ", parity);
   test ("USART::wake_method              ", wake_method);
   test ("USART::data_bits                ", data_bits);
   test ("USART::over_sample              ", over_sample);
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
   test ("USART::DMA_channel              ", DMA_channel);
   test ("USART::IRQn                     ", IRQn);
   test ("USART::number_clock             ", number_clock);
   test ("USART::is_rx_support            ", is_rx_support);
   test ("USART::is_tx_support            ", is_tx_support);
}
