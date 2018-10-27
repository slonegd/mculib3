#define STM32F030x6


#include "rcc.h"
#include <iostream>
#include <type_traits>
#include <thread>

mcu::RCC rcc;
auto& CMSIS = *reinterpret_cast<mcu::RCC::CMSIS_type*>(&rcc);

bool make()
{
   auto& rcc {mcu::RCC::make()};
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

bool set_APBprescaler()
{
   CMSIS.CFGR = 0;
   bool good {true};
   rcc.set (mcu::RCC::APBprescaler::APBdiv16);
   good &=     (CMSIS.CFGR & RCC_CFGR_PPRE_0)
       and     (CMSIS.CFGR & RCC_CFGR_PPRE_1)
       and     (CMSIS.CFGR & RCC_CFGR_PPRE_2);

   rcc.set (mcu::RCC::APBprescaler::APBdiv8);
   good &= not (CMSIS.CFGR & RCC_CFGR_PPRE_0)
       and     (CMSIS.CFGR & RCC_CFGR_PPRE_1)
       and     (CMSIS.CFGR & RCC_CFGR_PPRE_2);

   rcc.set (mcu::RCC::APBprescaler::APBdiv4);
   good &=     (CMSIS.CFGR & RCC_CFGR_PPRE_0)
       and not (CMSIS.CFGR & RCC_CFGR_PPRE_1)
       and     (CMSIS.CFGR & RCC_CFGR_PPRE_2);

   rcc.set (mcu::RCC::APBprescaler::APBdiv2);
   good &= not (CMSIS.CFGR & RCC_CFGR_PPRE_0)
       and not (CMSIS.CFGR & RCC_CFGR_PPRE_1)
       and     (CMSIS.CFGR & RCC_CFGR_PPRE_2);

   rcc.set (mcu::RCC::APBprescaler::APBnotdiv);
   good &= not (CMSIS.CFGR & RCC_CFGR_PPRE_2);

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
   good &=     (CMSIS.CFGR & RCC_CFGR_PLLMUL_1)
       and     (CMSIS.CFGR & RCC_CFGR_PLLMUL_2)
       and     (CMSIS.CFGR & RCC_CFGR_PLLMUL_3);

   rcc.set (mcu::RCC::PLLmultiplier::_15);
   good &=     (CMSIS.CFGR & RCC_CFGR_PLLMUL_0)
       and not (CMSIS.CFGR & RCC_CFGR_PLLMUL_1)
       and     (CMSIS.CFGR & RCC_CFGR_PLLMUL_2)
       and     (CMSIS.CFGR & RCC_CFGR_PLLMUL_3);

   rcc.set (mcu::RCC::PLLmultiplier::_10);
   good &= not (CMSIS.CFGR & RCC_CFGR_PLLMUL_0)
       and not (CMSIS.CFGR & RCC_CFGR_PLLMUL_1)
       and not (CMSIS.CFGR & RCC_CFGR_PLLMUL_2)
       and     (CMSIS.CFGR & RCC_CFGR_PLLMUL_3);

   rcc.set (mcu::RCC::PLLmultiplier::_9);
   good &=     (CMSIS.CFGR & RCC_CFGR_PLLMUL_0)
       and     (CMSIS.CFGR & RCC_CFGR_PLLMUL_1)
       and     (CMSIS.CFGR & RCC_CFGR_PLLMUL_2)
       and not (CMSIS.CFGR & RCC_CFGR_PLLMUL_3);

   rcc.set (mcu::RCC::PLLmultiplier::_2);
   good &= not (CMSIS.CFGR & RCC_CFGR_PLLMUL_0)
       and not (CMSIS.CFGR & RCC_CFGR_PLLMUL_1)
       and not (CMSIS.CFGR & RCC_CFGR_PLLMUL_2)
       and not (CMSIS.CFGR & RCC_CFGR_PLLMUL_3);


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

bool clock_enable()
{
   CMSIS.AHBENR = 0;
   bool good {true};

   rcc.clock_enable<Periph::GPIOA>();
   good &= bool(CMSIS.AHBENR & RCC_AHBENR_GPIOAEN_Msk);

   rcc.clock_enable<Periph::GPIOB>();
   good &= bool(CMSIS.AHBENR & RCC_AHBENR_GPIOBEN_Msk);

   rcc.clock_enable<Periph::GPIOC>();
   good &= bool(CMSIS.AHBENR & RCC_AHBENR_GPIOCEN_Msk);

   rcc.clock_enable<Periph::GPIOD>();
   good &= bool(CMSIS.AHBENR & RCC_AHBENR_GPIODEN_Msk);

   rcc.clock_enable<Periph::GPIOF>();
   good &= bool(CMSIS.AHBENR & RCC_AHBENR_GPIOFEN_Msk);

   return good;
}

int main()
{
   std::cout << '\n'
             << "Тесты класса RCC для STM32F0:" << std::endl;

   auto test = [](auto s, auto f){
      std::cout << s << (f() ? "\033[32mпрошёл\033[0m" : "\033[31mпровален\033[0m") << std::endl;
   };

   test ("RCC::make                  ", make);
   test ("RCC::set_AHBprescaler      ", set_AHBprescaler);
   test ("RCC::set_APBprescaler      ", set_APBprescaler);
   test ("RCC::set_SystemClock       ", set_SystemClock);
   test ("RCC::set_PLLsource         ", set_PLLsource);
   test ("RCC::set_PLLmultiplier     ", set_PLLmultiplier);
   test ("RCC::on_PLL                ", on_PLL);
   test ("RCC::wait_PLL_ready        ", wait_PLL_ready);
   test ("RCC::clock_enable          ", clock_enable);
}