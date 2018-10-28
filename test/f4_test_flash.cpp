#define STM32F405xx

#include "periph_flash.h"
#include <iostream>
#include <type_traits>
#include <thread>

mcu::FLASH flash;
auto& cmsis = *reinterpret_cast<mcu::FLASH::CMSIS_type*>(&flash);

// тесты возвращают true, если прошли
bool set_latency()
{
   bool good {true};

   cmsis.ACR = 0;
   flash.set (mcu::FLASH::Latency::_7);
   good &= (cmsis.ACR == FLASH_ACR_LATENCY_7WS);

   flash.set (mcu::FLASH::Latency::_6);
   good &= (cmsis.ACR == FLASH_ACR_LATENCY_6WS);

   flash.set (mcu::FLASH::Latency::_5);
   good &= (cmsis.ACR == FLASH_ACR_LATENCY_5WS);

   flash.set (mcu::FLASH::Latency::_4);
   good &= (cmsis.ACR == FLASH_ACR_LATENCY_4WS);

   flash.set (mcu::FLASH::Latency::_3);
   good &= (cmsis.ACR == FLASH_ACR_LATENCY_3WS);

   flash.set (mcu::FLASH::Latency::_2);
   good &= (cmsis.ACR == FLASH_ACR_LATENCY_2WS);

   flash.set (mcu::FLASH::Latency::_1);
   good &= (cmsis.ACR == FLASH_ACR_LATENCY_1WS);

   flash.set (mcu::FLASH::Latency::_0);
   good &= (cmsis.ACR == FLASH_ACR_LATENCY_0WS);

   return good;
}

bool lock()
{
   cmsis.CR = 0;
   flash.lock();
   return cmsis.CR & FLASH_CR_LOCK_Msk;
}

bool is_lock()
{
   bool good {true};

   cmsis.CR = 0;
   good &= not flash.is_lock();
   cmsis.CR |= FLASH_CR_LOCK_Msk;
   good &=     flash.is_lock();

   return good;
}

bool unlock()
{
   cmsis.KEYR = 0;
   cmsis.CR |= FLASH_CR_LOCK_Msk;
   std::thread { [&](){flash.unlock();} }.detach();
   while (cmsis.KEYR != 0x45670123) {}
   while (cmsis.KEYR != 0xCDEF89AB) {}
   return true;
}

bool set_progMode()
{
   cmsis.CR = 0;
   flash.set_progMode();
   return cmsis.CR & FLASH_CR_PG_Msk;
}

bool is_endOfProg()
{
   bool good {true};

   cmsis.SR = 0;
   good &= not flash.is_endOfProg();
   cmsis.SR |= FLASH_SR_EOP_Msk;
   good &=     flash.is_endOfProg();

   return good;
}

bool clear_flag_endOfProg()
{
   cmsis.SR = 0;
   flash.clear_flag_endOfProg();
   return cmsis.SR & FLASH_SR_EOP_Msk;
}

bool is_busy()
{
   bool good {true};

   cmsis.SR = 0;
   good &= not flash.is_busy();
   cmsis.SR |= FLASH_SR_BSY_Msk;
   good &=     flash.is_busy();

   return good;
}

bool set_ProgSize()
{
   bool good {true};

   cmsis.CR = 0;
   flash.set (mcu::FLASH::ProgSize::x64);
   good &=     (cmsis.CR & FLASH_CR_PSIZE_0)
       and     (cmsis.CR & FLASH_CR_PSIZE_1);

   flash.set (mcu::FLASH::ProgSize::x32);
   good &= not (cmsis.CR & FLASH_CR_PSIZE_0)
       and     (cmsis.CR & FLASH_CR_PSIZE_1);

   flash.set (mcu::FLASH::ProgSize::x16);
   good &=     (cmsis.CR & FLASH_CR_PSIZE_0)
       and not (cmsis.CR & FLASH_CR_PSIZE_1);

   flash.set (mcu::FLASH::ProgSize::x8);
   good &= not (cmsis.CR & FLASH_CR_PSIZE_0)
       and not (cmsis.CR & FLASH_CR_PSIZE_1);
   
   return good;
}


bool start_erase_sector()
{
   cmsis.CR = 0;

   std::thread { [&](){flash.start_erase (mcu::FLASH::Sector::_11);} }.detach();
   while ( not (cmsis.CR & FLASH_CR_SER_Msk) ) { }

   while ( not (       (cmsis.CR & FLASH_CR_SNB_0)
               and     (cmsis.CR & FLASH_CR_SNB_1)
               and not (cmsis.CR & FLASH_CR_SNB_2)
               and     (cmsis.CR & FLASH_CR_SNB_3)
           )
   ) { }
   while ( not (cmsis.CR & FLASH_CR_STRT_Msk) ) { }

   cmsis.CR &= ~(FLASH_CR_SER_Msk | FLASH_CR_STRT_Msk);

   std::thread { [&](){flash.start_erase (mcu::FLASH::Sector::_7);} }.detach();
   while ( not (cmsis.CR & FLASH_CR_SER_Msk) ) { }

   while ( not (       (cmsis.CR & FLASH_CR_SNB_0)
               and     (cmsis.CR & FLASH_CR_SNB_1)
               and     (cmsis.CR & FLASH_CR_SNB_2)
               and not (cmsis.CR & FLASH_CR_SNB_3)
           )
   ) { }
   while ( not (cmsis.CR & FLASH_CR_STRT_Msk) ) { }

   cmsis.CR &= ~(FLASH_CR_SER_Msk | FLASH_CR_STRT_Msk);

   std::thread { [&](){flash.start_erase (mcu::FLASH::Sector::_0);} }.detach();
   while ( not (cmsis.CR & FLASH_CR_SER_Msk) ) { }

   while ( not (   not (cmsis.CR & FLASH_CR_SNB_0)
               and not (cmsis.CR & FLASH_CR_SNB_1)
               and not (cmsis.CR & FLASH_CR_SNB_2)
               and not (cmsis.CR & FLASH_CR_SNB_3)
           )
   ) { }
   while ( not (cmsis.CR & FLASH_CR_STRT_Msk) ) { }


   return true;
}

bool address()
{
   return mcu::FLASH::address<mcu::FLASH::Sector::_0>()  == 0x08000000
      and mcu::FLASH::address<mcu::FLASH::Sector::_1>()  == 0x08004000
      and mcu::FLASH::address<mcu::FLASH::Sector::_2>()  == 0x08008000
      and mcu::FLASH::address<mcu::FLASH::Sector::_3>()  == 0x0800C000
      and mcu::FLASH::address<mcu::FLASH::Sector::_4>()  == 0x08010000
      and mcu::FLASH::address<mcu::FLASH::Sector::_5>()  == 0x08020000
      and mcu::FLASH::address<mcu::FLASH::Sector::_6>()  == 0x08040000
      and mcu::FLASH::address<mcu::FLASH::Sector::_7>()  == 0x08060000
      and mcu::FLASH::address<mcu::FLASH::Sector::_8>()  == 0x08080000
      and mcu::FLASH::address<mcu::FLASH::Sector::_9>()  == 0x080A0000
      and mcu::FLASH::address<mcu::FLASH::Sector::_10>() == 0x080C0000
      and mcu::FLASH::address<mcu::FLASH::Sector::_11>() == 0x080E0000;
}

bool size()
{
   return mcu::FLASH::size<mcu::FLASH::Sector::_0>()  ==  16 * 1024
      and mcu::FLASH::size<mcu::FLASH::Sector::_1>()  ==  16 * 1024
      and mcu::FLASH::size<mcu::FLASH::Sector::_2>()  ==  16 * 1024
      and mcu::FLASH::size<mcu::FLASH::Sector::_3>()  ==  16 * 1024
      and mcu::FLASH::size<mcu::FLASH::Sector::_4>()  ==  64 * 1024
      and mcu::FLASH::size<mcu::FLASH::Sector::_5>()  == 128 * 1024
      and mcu::FLASH::size<mcu::FLASH::Sector::_6>()  == 128 * 1024
      and mcu::FLASH::size<mcu::FLASH::Sector::_7>()  == 128 * 1024
      and mcu::FLASH::size<mcu::FLASH::Sector::_8>()  == 128 * 1024
      and mcu::FLASH::size<mcu::FLASH::Sector::_9>()  == 128 * 1024
      and mcu::FLASH::size<mcu::FLASH::Sector::_10>() == 128 * 1024
      and mcu::FLASH::size<mcu::FLASH::Sector::_11>() == 128 * 1024;

}






int main()
{
   std::cout << '\n'
             << "Тесты класса FLASH для STM32F4:" << std::endl;

   auto test = [](auto s, auto f){
      std::cout << s << (f() ? "\033[32mпрошёл\033[0m" : "\033[31mпровален\033[0m") << std::endl;
   };

   test ("FLASH::set_latency              ", set_latency);
   test ("FLASH::lock                     ", lock);
   test ("FLASH::is_lock                  ", is_lock);
   test ("FLASH::unlock                   ", unlock);
   test ("FLASH::set_progMode             ", set_progMode);
   test ("FLASH::is_endOfProg             ", is_endOfProg);
   test ("FLASH::clear_flag_endOfProg     ", clear_flag_endOfProg);
   test ("FLASH::is_busy                  ", is_busy);
   test ("FLASH::set_ProgSize             ", set_ProgSize);
   test ("FLASH::start_erase_sector       ", start_erase_sector);
   test ("FLASH::address                  ", address);
   test ("FLASH::size                     ", size);

   std::cout << '\n' << std::endl;
}