#define STM32F103xB
#define F_CPU   72000000UL
#include <iostream>
#include <type_traits>
#include "rcc.h"

struct MockRCC {
   bool good {false};
   template <mcu::Periph p> void clock_enable(){good = true;}
}mockRcc;
namespace mcu {
template <Periph p> std::enable_if_t<p == Periph::TEST_RCC, MockRCC&> make_reference() {return mockRcc;}
}

#include "f1_afio.h"

mcu::AFIO_ afio;
auto& CMSIS = *reinterpret_cast<mcu::AFIO_::CMSIS_type*>(&afio);

bool make()
{
   bool good {true};
   auto& afio {mcu::make_reference<mcu::Periph::AFIO_>()};
   return reinterpret_cast<size_t>(&afio) == AFIO_BASE 
      and std::is_same_v<std::remove_reference_t<decltype(afio)>, mcu::AFIO_>;
}

bool clock_enable()
{
   bool good {true};
   afio.clock_enable<mcu::Periph::AFIO_, mcu::Periph::TEST_RCC>();
   good &= mockRcc.good;
   return good;
}

bool remap()
{
   bool good {true};
   CMSIS.MAPR = 0;
   afio.remap<mcu::Periph::USART1>();
   good &= bool (CMSIS.MAPR & AFIO_MAPR_USART1_REMAP_Msk);
   CMSIS.MAPR = 0;
   afio.remap<mcu::Periph::USART2>();
   good &= bool (CMSIS.MAPR & AFIO_MAPR_USART2_REMAP_Msk);
   CMSIS.MAPR = 0;
   afio.remap<mcu::Periph::USART3, mcu::AFIO_bits::MAPR::Remap::Full>();
   good &= bool (CMSIS.MAPR & AFIO_MAPR_USART3_REMAP_0);
   good &= bool (CMSIS.MAPR & AFIO_MAPR_USART3_REMAP_1);
   CMSIS.MAPR = 0;
   afio.remap<mcu::Periph::USART3, mcu::AFIO_bits::MAPR::Remap::Partial_2>();
   good &= not  (CMSIS.MAPR & AFIO_MAPR_USART3_REMAP_0);
   good &= bool (CMSIS.MAPR & AFIO_MAPR_USART3_REMAP_1);
   CMSIS.MAPR = 0;
   afio.remap<mcu::Periph::USART3, mcu::AFIO_bits::MAPR::Remap::Partial_1>();
   good &= bool (CMSIS.MAPR & AFIO_MAPR_USART3_REMAP_0);
   good &= not  (CMSIS.MAPR & AFIO_MAPR_USART3_REMAP_1);
   CMSIS.MAPR = 0;
   afio.remap<mcu::Periph::USART3, mcu::AFIO_bits::MAPR::Remap::No>();
   good &= not  (CMSIS.MAPR & AFIO_MAPR_USART3_REMAP_0);
   good &= not  (CMSIS.MAPR & AFIO_MAPR_USART3_REMAP_1);
   
   return good;
}

int main()
{
   std::cout << '\n'
             << "Тесты класса AFIO для STM32F1:" << std::endl;

   auto test = [](auto s, auto f){
      std::cout << s << (f() ? "\033[32mпрошёл\033[0m" : "\033[31mпровален\033[0m") << std::endl;
   };

   test ("AFIO::make            ", make);
   test ("AFIO::clock_enable    ", clock_enable);
   test ("AFIO::remap           ", remap);
}

