#define STM32F103xB
#define F_CPU   72000000UL


#include "rcc.h"
#include <iostream>
#include <type_traits>
#include <thread>

mcu::RCC rcc;
auto& CMSIS = *reinterpret_cast<mcu::RCC::CMSIS_type*>(&rcc);

bool make()
{
   auto& rcc {mcu::make_reference<mcu::Periph::RCC>()};
   return reinterpret_cast<size_t>(&rcc) == RCC_BASE 
      and std::is_same_v<std::remove_reference_t<decltype(rcc)>, mcu::RCC>;
}

bool set_AHBprescaler()
{
   CMSIS.CFGR = 0;
   bool good {true};
   rcc.set (mcu::RCC::AHBprescaler::AHBdiv512);
   good &=     (CMSIS.CFGR & RCC_CFGR_HPRE_0)
       and     (CMSIS.CFGR & RCC_CFGR_HPRE_1)
       and     (CMSIS.CFGR & RCC_CFGR_HPRE_2)
       and     (CMSIS.CFGR & RCC_CFGR_HPRE_3);

   rcc.set (mcu::RCC::AHBprescaler::AHBdiv256);
   good &= not (CMSIS.CFGR & RCC_CFGR_HPRE_0)
       and     (CMSIS.CFGR & RCC_CFGR_HPRE_1)
       and     (CMSIS.CFGR & RCC_CFGR_HPRE_2)
       and     (CMSIS.CFGR & RCC_CFGR_HPRE_3);

   rcc.set (mcu::RCC::AHBprescaler::AHBdiv128);
   good &=     (CMSIS.CFGR & RCC_CFGR_HPRE_0)
       and not (CMSIS.CFGR & RCC_CFGR_HPRE_1)
       and     (CMSIS.CFGR & RCC_CFGR_HPRE_2)
       and     (CMSIS.CFGR & RCC_CFGR_HPRE_3);

   rcc.set (mcu::RCC::AHBprescaler::AHBdiv64);
   good &= not (CMSIS.CFGR & RCC_CFGR_HPRE_0)
       and not (CMSIS.CFGR & RCC_CFGR_HPRE_1)
       and     (CMSIS.CFGR & RCC_CFGR_HPRE_2)
       and     (CMSIS.CFGR & RCC_CFGR_HPRE_3);

   rcc.set (mcu::RCC::AHBprescaler::AHBdiv16);
   good &=     (CMSIS.CFGR & RCC_CFGR_HPRE_0)
       and     (CMSIS.CFGR & RCC_CFGR_HPRE_1)
       and not (CMSIS.CFGR & RCC_CFGR_HPRE_2)
       and     (CMSIS.CFGR & RCC_CFGR_HPRE_3);

   rcc.set (mcu::RCC::AHBprescaler::AHBdiv8);
   good &= not (CMSIS.CFGR & RCC_CFGR_HPRE_0)
       and     (CMSIS.CFGR & RCC_CFGR_HPRE_1)
       and not (CMSIS.CFGR & RCC_CFGR_HPRE_2)
       and     (CMSIS.CFGR & RCC_CFGR_HPRE_3);

   rcc.set (mcu::RCC::AHBprescaler::AHBdiv4);
   good &=     (CMSIS.CFGR & RCC_CFGR_HPRE_0)
       and not (CMSIS.CFGR & RCC_CFGR_HPRE_1)
       and not (CMSIS.CFGR & RCC_CFGR_HPRE_2)
       and     (CMSIS.CFGR & RCC_CFGR_HPRE_3);

   rcc.set (mcu::RCC::AHBprescaler::AHBdiv2);
   good &= not (CMSIS.CFGR & RCC_CFGR_HPRE_0)
       and not (CMSIS.CFGR & RCC_CFGR_HPRE_1)
       and not (CMSIS.CFGR & RCC_CFGR_HPRE_2)
       and     (CMSIS.CFGR & RCC_CFGR_HPRE_3);

   rcc.set (mcu::RCC::AHBprescaler::AHBnotdiv);
   good &= not (CMSIS.CFGR & RCC_CFGR_HPRE_3);

   return good;
}

bool set_APB1()
{
   CMSIS.CFGR = 0;
   bool good {true};
   rcc.set_APB1 (mcu::RCC::APBprescaler::APBdiv16);
   good &=      (CMSIS.CFGR & RCC_CFGR_PPRE1_0)
       and      (CMSIS.CFGR & RCC_CFGR_PPRE1_1)
       and      (CMSIS.CFGR & RCC_CFGR_PPRE1_2);

   rcc.set_APB1 (mcu::RCC::APBprescaler::APBdiv8);
   good &= not  (CMSIS.CFGR & RCC_CFGR_PPRE1_0)
       and      (CMSIS.CFGR & RCC_CFGR_PPRE1_1)
       and      (CMSIS.CFGR & RCC_CFGR_PPRE1_2);

   rcc.set_APB1 (mcu::RCC::APBprescaler::APBdiv4);
   good &=      (CMSIS.CFGR & RCC_CFGR_PPRE1_0)
       and not  (CMSIS.CFGR & RCC_CFGR_PPRE1_1)
       and      (CMSIS.CFGR & RCC_CFGR_PPRE1_2);

   rcc.set_APB1 (mcu::RCC::APBprescaler::APBdiv2);
   good &= not  (CMSIS.CFGR & RCC_CFGR_PPRE1_0)
       and not  (CMSIS.CFGR & RCC_CFGR_PPRE1_1)
       and      (CMSIS.CFGR & RCC_CFGR_PPRE1_2);

   rcc.set_APB1 (mcu::RCC::APBprescaler::APBnotdiv);
   good &= not  (CMSIS.CFGR & RCC_CFGR_PPRE1_0)
       and not  (CMSIS.CFGR & RCC_CFGR_PPRE1_1)
       and not  (CMSIS.CFGR & RCC_CFGR_PPRE1_2);

   return good;
}

bool set_APB2()
{
   CMSIS.CFGR = 0;
   bool good {true};
   rcc.set_APB2 (mcu::RCC::APBprescaler::APBdiv16);
   good &=      (CMSIS.CFGR & RCC_CFGR_PPRE2_0)
       and      (CMSIS.CFGR & RCC_CFGR_PPRE2_1)
       and      (CMSIS.CFGR & RCC_CFGR_PPRE2_2);

   rcc.set_APB2 (mcu::RCC::APBprescaler::APBdiv8);
   good &= not  (CMSIS.CFGR & RCC_CFGR_PPRE2_0)
       and      (CMSIS.CFGR & RCC_CFGR_PPRE2_1)
       and      (CMSIS.CFGR & RCC_CFGR_PPRE2_2);

   rcc.set_APB2 (mcu::RCC::APBprescaler::APBdiv4);
   good &=      (CMSIS.CFGR & RCC_CFGR_PPRE2_0)
       and not  (CMSIS.CFGR & RCC_CFGR_PPRE2_1)
       and      (CMSIS.CFGR & RCC_CFGR_PPRE2_2);

   rcc.set_APB2 (mcu::RCC::APBprescaler::APBdiv2);
   good &= not  (CMSIS.CFGR & RCC_CFGR_PPRE2_0)
       and not  (CMSIS.CFGR & RCC_CFGR_PPRE2_1)
       and      (CMSIS.CFGR & RCC_CFGR_PPRE2_2);

   rcc.set_APB2 (mcu::RCC::APBprescaler::APBnotdiv);
   good &= not  (CMSIS.CFGR & RCC_CFGR_PPRE2_0)
       and not  (CMSIS.CFGR & RCC_CFGR_PPRE2_1)
       and not  (CMSIS.CFGR & RCC_CFGR_PPRE2_2);

   return good;
}

bool set_SystemClock()
{
   CMSIS.CFGR = 0;
   bool good {true};
   rcc.set (mcu::RCC::SystemClock::CS_PLL);
   good &= not (CMSIS.CFGR & RCC_CFGR_SW_0)
       and     (CMSIS.CFGR & RCC_CFGR_SW_1);

   rcc.set (mcu::RCC::SystemClock::CS_HSE);
   good &=     (CMSIS.CFGR & RCC_CFGR_SW_0)
       and not (CMSIS.CFGR & RCC_CFGR_SW_1);

   rcc.set (mcu::RCC::SystemClock::CS_HSI);
   good &= not (CMSIS.CFGR & RCC_CFGR_SW_0)
       and not (CMSIS.CFGR & RCC_CFGR_SW_1);

   return good;
}

bool set_PLLsource()
{
   CMSIS.CFGR = 0;
   bool good {true};
   rcc.set (mcu::RCC::PLLsource::HSE);
   good &=     bool(CMSIS.CFGR & RCC_CFGR_PLLSRC_Msk);

   rcc.set (mcu::RCC::PLLsource::HSIdiv2);
   good &= not bool(CMSIS.CFGR & RCC_CFGR_PLLSRC_Msk);

   return good;
}

bool set_PLLmultiplier()
{
   CMSIS.CFGR = 0;
   bool good {true};
   rcc.set (mcu::RCC::PLLmultiplier::_16);
   good &= not (CMSIS.CFGR & RCC_CFGR_PLLMULL_0)
       and     (CMSIS.CFGR & RCC_CFGR_PLLMULL_1)
       and     (CMSIS.CFGR & RCC_CFGR_PLLMULL_2)
       and     (CMSIS.CFGR & RCC_CFGR_PLLMULL_3);

   rcc.set (mcu::RCC::PLLmultiplier::_15);
   good &=     (CMSIS.CFGR & RCC_CFGR_PLLMULL_0)
       and not (CMSIS.CFGR & RCC_CFGR_PLLMULL_1)
       and     (CMSIS.CFGR & RCC_CFGR_PLLMULL_2)
       and     (CMSIS.CFGR & RCC_CFGR_PLLMULL_3);

   rcc.set (mcu::RCC::PLLmultiplier::_14);
   good &= not (CMSIS.CFGR & RCC_CFGR_PLLMULL_0)
       and not (CMSIS.CFGR & RCC_CFGR_PLLMULL_1)
       and     (CMSIS.CFGR & RCC_CFGR_PLLMULL_2)
       and     (CMSIS.CFGR & RCC_CFGR_PLLMULL_3);

   rcc.set (mcu::RCC::PLLmultiplier::_13);
   good &=     (CMSIS.CFGR & RCC_CFGR_PLLMULL_0)
       and     (CMSIS.CFGR & RCC_CFGR_PLLMULL_1)
       and not (CMSIS.CFGR & RCC_CFGR_PLLMULL_2)
       and     (CMSIS.CFGR & RCC_CFGR_PLLMULL_3);

   rcc.set (mcu::RCC::PLLmultiplier::_12);
   good &= not (CMSIS.CFGR & RCC_CFGR_PLLMULL_0)
       and     (CMSIS.CFGR & RCC_CFGR_PLLMULL_1)
       and not (CMSIS.CFGR & RCC_CFGR_PLLMULL_2)
       and     (CMSIS.CFGR & RCC_CFGR_PLLMULL_3);

   rcc.set (mcu::RCC::PLLmultiplier::_11);
   good &=     (CMSIS.CFGR & RCC_CFGR_PLLMULL_0)
       and not (CMSIS.CFGR & RCC_CFGR_PLLMULL_1)
       and not (CMSIS.CFGR & RCC_CFGR_PLLMULL_2)
       and     (CMSIS.CFGR & RCC_CFGR_PLLMULL_3);

   rcc.set (mcu::RCC::PLLmultiplier::_10);
   good &= not (CMSIS.CFGR & RCC_CFGR_PLLMULL_0)
       and not (CMSIS.CFGR & RCC_CFGR_PLLMULL_1)
       and not (CMSIS.CFGR & RCC_CFGR_PLLMULL_2)
       and     (CMSIS.CFGR & RCC_CFGR_PLLMULL_3);

   rcc.set (mcu::RCC::PLLmultiplier::_9);
   good &=     (CMSIS.CFGR & RCC_CFGR_PLLMULL_0)
       and     (CMSIS.CFGR & RCC_CFGR_PLLMULL_1)
       and     (CMSIS.CFGR & RCC_CFGR_PLLMULL_2)
       and not (CMSIS.CFGR & RCC_CFGR_PLLMULL_3);

   rcc.set (mcu::RCC::PLLmultiplier::_8);
   good &= not (CMSIS.CFGR & RCC_CFGR_PLLMULL_0)
       and     (CMSIS.CFGR & RCC_CFGR_PLLMULL_1)
       and     (CMSIS.CFGR & RCC_CFGR_PLLMULL_2)
       and not (CMSIS.CFGR & RCC_CFGR_PLLMULL_3);

   rcc.set (mcu::RCC::PLLmultiplier::_7);
   good &=     (CMSIS.CFGR & RCC_CFGR_PLLMULL_0)
       and not (CMSIS.CFGR & RCC_CFGR_PLLMULL_1)
       and     (CMSIS.CFGR & RCC_CFGR_PLLMULL_2)
       and not (CMSIS.CFGR & RCC_CFGR_PLLMULL_3);

   rcc.set (mcu::RCC::PLLmultiplier::_6);
   good &= not (CMSIS.CFGR & RCC_CFGR_PLLMULL_0)
       and not (CMSIS.CFGR & RCC_CFGR_PLLMULL_1)
       and     (CMSIS.CFGR & RCC_CFGR_PLLMULL_2)
       and not (CMSIS.CFGR & RCC_CFGR_PLLMULL_3);

   rcc.set (mcu::RCC::PLLmultiplier::_5);
   good &=     (CMSIS.CFGR & RCC_CFGR_PLLMULL_0)
       and     (CMSIS.CFGR & RCC_CFGR_PLLMULL_1)
       and not (CMSIS.CFGR & RCC_CFGR_PLLMULL_2)
       and not (CMSIS.CFGR & RCC_CFGR_PLLMULL_3);

   rcc.set (mcu::RCC::PLLmultiplier::_4);
   good &= not (CMSIS.CFGR & RCC_CFGR_PLLMULL_0)
       and     (CMSIS.CFGR & RCC_CFGR_PLLMULL_1)
       and not (CMSIS.CFGR & RCC_CFGR_PLLMULL_2)
       and not (CMSIS.CFGR & RCC_CFGR_PLLMULL_3);

   rcc.set (mcu::RCC::PLLmultiplier::_3);
   good &=     (CMSIS.CFGR & RCC_CFGR_PLLMULL_0)
       and not (CMSIS.CFGR & RCC_CFGR_PLLMULL_1)
       and not (CMSIS.CFGR & RCC_CFGR_PLLMULL_2)
       and not (CMSIS.CFGR & RCC_CFGR_PLLMULL_3);

   rcc.set (mcu::RCC::PLLmultiplier::_2);
   good &= not (CMSIS.CFGR & RCC_CFGR_PLLMULL_0)
       and not (CMSIS.CFGR & RCC_CFGR_PLLMULL_1)
       and not (CMSIS.CFGR & RCC_CFGR_PLLMULL_2)
       and not (CMSIS.CFGR & RCC_CFGR_PLLMULL_3);

   return good;
}

bool on_HSE()
{
   CMSIS.CR = 0;
   rcc.on_HSE();
   return CMSIS.CR & RCC_CR_HSEON_Msk;
}

bool wait_HSE_ready()
{
   CMSIS.CR = 0;
   bool good {true};
   bool work {true};
   auto worker = [&](){rcc.wait_HSE_ready(); work = false;};
   std::thread {worker}.detach();
   std::this_thread::sleep_for(std::chrono::milliseconds(100));
   good &= work;
   CMSIS.CR |= RCC_CR_HSERDY_Msk;
   while (work) {}

   return good;
}

bool on_PLL()
{
   CMSIS.CR = 0;
   rcc.on_PLL();
   return CMSIS.CR & RCC_CR_PLLON_Msk;
}

bool wait_PLL_ready()
{
   CMSIS.CR = 0;
   bool good {true};
   bool work {true};
   auto worker = [&]() { rcc.wait_PLL_ready(); work = false; };
   std::thread {worker}.detach();
   std::this_thread::sleep_for(std::chrono::milliseconds(100));
   good &= work;
   CMSIS.CR |= RCC_CR_PLLRDY_Msk;
   while (work) {}

   return good;
}

bool get_APB_clock()
{
   bool good {true};
   good &= (rcc.get_APB_clock(mcu::RCC::APBprescaler::APBnotdiv) == F_CPU);
   good &= (rcc.get_APB_clock(mcu::RCC::APBprescaler::APBdiv2)   == F_CPU/2);
   good &= (rcc.get_APB_clock(mcu::RCC::APBprescaler::APBdiv4)   == F_CPU/4);
   good &= (rcc.get_APB_clock(mcu::RCC::APBprescaler::APBdiv8)   == F_CPU/8);
   good &= (rcc.get_APB_clock(mcu::RCC::APBprescaler::APBdiv16)  == F_CPU/16);
   return good;
}

bool get_APB1_clock()
{
   bool good {true};
   CMSIS.CFGR = 0;
   CMSIS.CFGR |= RCC_CFGR_PPRE1_DIV16;
   good &= (rcc.get_APB1_clock() == F_CPU/16);
   CMSIS.CFGR = 0;
   CMSIS.CFGR |= RCC_CFGR_PPRE1_DIV8;
   good &= (rcc.get_APB1_clock() == F_CPU/8);
   CMSIS.CFGR = 0;
   CMSIS.CFGR |= RCC_CFGR_PPRE1_DIV4;
   good &= (rcc.get_APB1_clock() == F_CPU/4);
   CMSIS.CFGR = 0;
   CMSIS.CFGR |= RCC_CFGR_PPRE1_DIV2;
   good &= (rcc.get_APB1_clock() == F_CPU/2);
   CMSIS.CFGR = 0;
   CMSIS.CFGR |= RCC_CFGR_PPRE1_DIV1;
   good &= (rcc.get_APB1_clock() == F_CPU);
   CMSIS.CFGR = 0;
   CMSIS.CFGR |= RCC_CFGR_PPRE2_DIV8;
   good &= not (rcc.get_APB1_clock() == F_CPU/8);
   return good;
}

bool get_APB2_clock()
{
   bool good {true};
   CMSIS.CFGR = 0;
   CMSIS.CFGR |= RCC_CFGR_PPRE2_DIV16;
   good &= (rcc.get_APB2_clock() == F_CPU/16);
   CMSIS.CFGR = 0;
   CMSIS.CFGR |= RCC_CFGR_PPRE2_DIV8;
   good &= (rcc.get_APB2_clock() == F_CPU/8);
   CMSIS.CFGR = 0;
   CMSIS.CFGR |= RCC_CFGR_PPRE2_DIV4;
   good &= (rcc.get_APB2_clock() == F_CPU/4);
   CMSIS.CFGR = 0;
   CMSIS.CFGR |= RCC_CFGR_PPRE2_DIV2;
   good &= (rcc.get_APB2_clock() == F_CPU/2);
   CMSIS.CFGR = 0;
   CMSIS.CFGR |= RCC_CFGR_PPRE2_DIV1;
   good &= (rcc.get_APB2_clock() == F_CPU);
   CMSIS.CFGR = 0;
   CMSIS.CFGR |= RCC_CFGR_PPRE1_DIV8;
   good &= not (rcc.get_APB2_clock() == F_CPU/8);
   return good;
}

bool clock_enable()
{
   CMSIS.AHBENR = 0;
   bool good {true};

   rcc.clock_enable<mcu::Periph::GPIOA>();
   good &= bool(CMSIS.APB2ENR & RCC_APB2ENR_IOPAEN_Msk);

   rcc.clock_enable<mcu::Periph::GPIOB>();
   good &= bool(CMSIS.APB2ENR & RCC_APB2ENR_IOPBEN_Msk);

   rcc.clock_enable<mcu::Periph::GPIOC>();
   good &= bool(CMSIS.APB2ENR & RCC_APB2ENR_IOPCEN_Msk);

   rcc.clock_enable<mcu::Periph::GPIOD>();
   good &= bool(CMSIS.APB2ENR & RCC_APB2ENR_IOPDEN_Msk);

   rcc.clock_enable<mcu::Periph::GPIOE>();
   good &= bool(CMSIS.APB2ENR & RCC_APB2ENR_IOPEEN_Msk);

   rcc.clock_enable<mcu::Periph::USART1>();
   good &= bool(CMSIS.APB2ENR & RCC_APB2ENR_USART1EN_Msk);

   rcc.clock_enable<mcu::Periph::USART2>();
   good &= bool(CMSIS.APB1ENR & RCC_APB1ENR_USART2EN_Msk);

   rcc.clock_enable<mcu::Periph::USART3>();
   good &= bool(CMSIS.APB1ENR & RCC_APB1ENR_USART3EN_Msk);

   return good;
}

int main()
{
   std::cout << '\n'
             << "Тесты класса RCC для STM32F1:" << std::endl;

   auto test = [](auto s, auto f){
      std::cout << s << (f() ? "\033[32mпрошёл\033[0m" : "\033[31mпровален\033[0m") << std::endl;
   };

   test ("RCC::make              ", make);
   test ("RCC::set_AHBprescaler  ", set_AHBprescaler);
   test ("RCC::set_APB1          ", set_APB1);
   test ("RCC::set_APB2          ", set_APB2);
   test ("RCC::set_SystemClock   ", set_SystemClock);
   test ("RCC::set_PLLsource     ", set_PLLsource);
   test ("RCC::set_PLLmultiplier ", set_PLLmultiplier);
   test ("RCC::on_HSE            ", on_HSE);
   test ("RCC::wait_HSE_ready    ", wait_HSE_ready);
   test ("RCC::on_PLL            ", on_PLL);
   test ("RCC::wait_PLL_ready    ", wait_PLL_ready);
   test ("RCC::get_APB_clock     ", get_APB_clock);
   test ("RCC::get_APB1_clock    ", get_APB1_clock);
   test ("RCC::get_APB2_clock    ", get_APB2_clock);
   test ("RCC::clock_enable      ", clock_enable);
}