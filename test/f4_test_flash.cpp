#define BOOST_TEST_MODULE f4_test_flash
#include <boost/test/unit_test.hpp>

#define STM32F405xx
#define TEST

#include "periph_flash.h"
#include <iostream>
#include <type_traits>
#include <thread>

BOOST_AUTO_TEST_SUITE (test_suite_main)

mcu::FLASH flash;
auto& cmsis = *reinterpret_cast<mcu::FLASH::CMSIS_type*>(&flash);

BOOST_AUTO_TEST_CASE (make_reference)
{
   auto& flash {mcu::make_reference<mcu::Periph::FLASH>()};
   auto address = reinterpret_cast<size_t>(&flash);
   auto same = std::is_same_v<std::remove_reference_t<decltype(flash)>, mcu::FLASH>;
   BOOST_CHECK_EQUAL (address, FLASH_R_BASE);
   BOOST_CHECK_EQUAL (same, true);
}

BOOST_AUTO_TEST_CASE (set_latency)
{
   cmsis.ACR = 0;
   flash.set (mcu::FLASH::Latency::_7);
   BOOST_CHECK_EQUAL (cmsis.ACR, FLASH_ACR_LATENCY_7WS);

   flash.set (mcu::FLASH::Latency::_6);
   BOOST_CHECK_EQUAL (cmsis.ACR, FLASH_ACR_LATENCY_6WS);

   flash.set (mcu::FLASH::Latency::_5);
   BOOST_CHECK_EQUAL (cmsis.ACR, FLASH_ACR_LATENCY_5WS);

   flash.set (mcu::FLASH::Latency::_4);
   BOOST_CHECK_EQUAL (cmsis.ACR, FLASH_ACR_LATENCY_4WS);

   flash.set (mcu::FLASH::Latency::_3);
   BOOST_CHECK_EQUAL (cmsis.ACR, FLASH_ACR_LATENCY_3WS);

   flash.set (mcu::FLASH::Latency::_2);
   BOOST_CHECK_EQUAL (cmsis.ACR, FLASH_ACR_LATENCY_2WS);

   flash.set (mcu::FLASH::Latency::_1);
   BOOST_CHECK_EQUAL (cmsis.ACR, FLASH_ACR_LATENCY_1WS);

   flash.set (mcu::FLASH::Latency::_0);
   BOOST_CHECK_EQUAL (cmsis.ACR, FLASH_ACR_LATENCY_0WS);
}

BOOST_AUTO_TEST_CASE (lock)
{
   cmsis.CR = 0;
   flash.lock();
   BOOST_CHECK_EQUAL (cmsis.CR, FLASH_CR_LOCK_Msk);
}

BOOST_AUTO_TEST_CASE (is_lock)
{
   cmsis.CR = 0;
   BOOST_CHECK_EQUAL (flash.is_lock(), false);
   cmsis.CR = FLASH_CR_LOCK_Msk;
   BOOST_CHECK_EQUAL (flash.is_lock(), true);
}

BOOST_AUTO_TEST_CASE (unlock)
{
   cmsis.KEYR = 0;
   cmsis.CR |= FLASH_CR_LOCK_Msk;
   std::thread { [&](){flash.unlock();} }.detach();
   while (cmsis.KEYR != 0x45670123) {}
   while (cmsis.KEYR != 0xCDEF89AB) {}
}

BOOST_AUTO_TEST_CASE (set_progMode)
{
   cmsis.CR = 0;
   flash.set_progMode();
   BOOST_CHECK_EQUAL (cmsis.CR, FLASH_CR_PG_Msk);
}

BOOST_AUTO_TEST_CASE (is_endOfProg)
{
   cmsis.SR = 0;
   BOOST_CHECK_EQUAL (flash.is_endOfProg(), false);
   cmsis.SR = FLASH_SR_EOP_Msk;
   BOOST_CHECK_EQUAL (flash.is_endOfProg(), true);
}

BOOST_AUTO_TEST_CASE (clear_flag_endOfProg)
{
   cmsis.SR = 0;
   flash.clear_flag_endOfProg();
   BOOST_CHECK_EQUAL (cmsis.SR, FLASH_SR_EOP_Msk);
}

BOOST_AUTO_TEST_CASE (is_busy)
{
   cmsis.SR = 0;
   BOOST_CHECK_EQUAL (flash.is_busy(), false);
   cmsis.SR = FLASH_SR_BSY_Msk;
   BOOST_CHECK_EQUAL (flash.is_busy(), true);
}

BOOST_AUTO_TEST_CASE (set_ProgSize)
{
   cmsis.CR = 0;
   uint32_t result {0};
   flash.set (mcu::FLASH::ProgSize::x64);
   result = FLASH_CR_PSIZE_0 | FLASH_CR_PSIZE_1;
   BOOST_CHECK_EQUAL (cmsis.CR, result);

   flash.set (mcu::FLASH::ProgSize::x32);
   result = FLASH_CR_PSIZE_1;
   BOOST_CHECK_EQUAL (cmsis.CR, result);

   flash.set (mcu::FLASH::ProgSize::x16);
   result = FLASH_CR_PSIZE_0;
   BOOST_CHECK_EQUAL (cmsis.CR, result);

   flash.set (mcu::FLASH::ProgSize::x8);
   result = 0;
   BOOST_CHECK_EQUAL (cmsis.CR, result);
}

BOOST_AUTO_TEST_CASE (start_erase_sector)
{
   // TODO: rewrite
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
}

BOOST_AUTO_TEST_CASE (address)
{
   BOOST_CHECK_EQUAL (mcu::FLASH::address<mcu::FLASH::Sector:: _0>(), 0x08000000);
   BOOST_CHECK_EQUAL (mcu::FLASH::address<mcu::FLASH::Sector:: _1>(), 0x08004000);
   BOOST_CHECK_EQUAL (mcu::FLASH::address<mcu::FLASH::Sector:: _2>(), 0x08008000);
   BOOST_CHECK_EQUAL (mcu::FLASH::address<mcu::FLASH::Sector:: _3>(), 0x0800C000);
   BOOST_CHECK_EQUAL (mcu::FLASH::address<mcu::FLASH::Sector:: _4>(), 0x08010000);
   BOOST_CHECK_EQUAL (mcu::FLASH::address<mcu::FLASH::Sector:: _5>(), 0x08020000);
   BOOST_CHECK_EQUAL (mcu::FLASH::address<mcu::FLASH::Sector:: _6>(), 0x08040000);
   BOOST_CHECK_EQUAL (mcu::FLASH::address<mcu::FLASH::Sector:: _7>(), 0x08060000);
   BOOST_CHECK_EQUAL (mcu::FLASH::address<mcu::FLASH::Sector:: _8>(), 0x08080000);
   BOOST_CHECK_EQUAL (mcu::FLASH::address<mcu::FLASH::Sector:: _9>(), 0x080A0000);
   BOOST_CHECK_EQUAL (mcu::FLASH::address<mcu::FLASH::Sector::_10>(), 0x080C0000);
   BOOST_CHECK_EQUAL (mcu::FLASH::address<mcu::FLASH::Sector::_11>(), 0x080E0000);
}

BOOST_AUTO_TEST_CASE (size)
{
   BOOST_CHECK_EQUAL (mcu::FLASH::size<mcu::FLASH::Sector:: _0>(),  16 * 1024);
   BOOST_CHECK_EQUAL (mcu::FLASH::size<mcu::FLASH::Sector:: _1>(),  16 * 1024);
   BOOST_CHECK_EQUAL (mcu::FLASH::size<mcu::FLASH::Sector:: _2>(),  16 * 1024);
   BOOST_CHECK_EQUAL (mcu::FLASH::size<mcu::FLASH::Sector:: _3>(),  16 * 1024);
   BOOST_CHECK_EQUAL (mcu::FLASH::size<mcu::FLASH::Sector:: _4>(),  64 * 1024);
   BOOST_CHECK_EQUAL (mcu::FLASH::size<mcu::FLASH::Sector:: _5>(), 128 * 1024);
   BOOST_CHECK_EQUAL (mcu::FLASH::size<mcu::FLASH::Sector:: _6>(), 128 * 1024);
   BOOST_CHECK_EQUAL (mcu::FLASH::size<mcu::FLASH::Sector:: _7>(), 128 * 1024);
   BOOST_CHECK_EQUAL (mcu::FLASH::size<mcu::FLASH::Sector:: _8>(), 128 * 1024);
   BOOST_CHECK_EQUAL (mcu::FLASH::size<mcu::FLASH::Sector:: _9>(), 128 * 1024);
   BOOST_CHECK_EQUAL (mcu::FLASH::size<mcu::FLASH::Sector::_10>(), 128 * 1024);
   BOOST_CHECK_EQUAL (mcu::FLASH::size<mcu::FLASH::Sector::_11>(), 128 * 1024);
}

BOOST_AUTO_TEST_SUITE_END()