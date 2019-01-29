BOOST_AUTO_TEST_SUITE (test_suite_main)

mcu::FLASH flash;
auto& cmsis = flash.like_CMSIS();

BOOST_AUTO_TEST_CASE (like_CMSIS)
{
   auto same = std::is_same_v<std::remove_reference_t<decltype(cmsis)>, FLASH_TypeDef>;
   auto address_flash = reinterpret_cast<size_t>(&flash);
   auto address_CMSIS = reinterpret_cast<size_t>(&cmsis);
   BOOST_CHECK_EQUAL (address_flash, address_CMSIS);
   BOOST_CHECK_EQUAL (same, true);
}

BOOST_AUTO_TEST_CASE (set_latency)
{
   cmsis.ACR = 0;
#if defined(STM32F0)
   flash.set (mcu::FLASH::Latency::_1);
   BOOST_CHECK_EQUAL (cmsis.ACR, FLASH_ACR_LATENCY_Msk);

   flash.set (mcu::FLASH::Latency::_0);
   BOOST_CHECK_EQUAL (cmsis.ACR, 0);
#elif defined(STM32F1)
   flash.set (mcu::FLASH::Latency::_24_48MHz);
   BOOST_CHECK_EQUAL (cmsis.ACR, FLASH_ACR_LATENCY_0);

   flash.set (mcu::FLASH::Latency::_48_72MHz);
   BOOST_CHECK_EQUAL (cmsis.ACR, FLASH_ACR_LATENCY_1);
   
   flash.set (mcu::FLASH::Latency::_0_24MHz);
   BOOST_CHECK_EQUAL (cmsis.ACR, 0);
#endif
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

   Timeout timeout {100_ms};
   while (cmsis.KEYR != 0x45670123 and not timeout) {}
   BOOST_CHECK_EQUAL (bool(timeout), false);

   timeout.restart();
   while (cmsis.KEYR != 0xCDEF89AB and not timeout) {}
   BOOST_CHECK_EQUAL (bool(timeout), false);
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

template<mcu::FLASH::Sector s>
auto test = [&]() {
   cmsis.CR = 0;
   std::thread { [&](){flash.start_erase<s>();} }.detach();

   auto address = mcu::FLASH::address<s>();
   
   Timeout timeout {100_ms};
   while (not(cmsis.CR & FLASH_CR_PER_Msk) and not timeout) { }
   BOOST_CHECK_EQUAL (bool(timeout), false);
   BOOST_CHECK_EQUAL (bool(cmsis.CR & FLASH_CR_STRT_Msk), false);

   timeout.restart();
   while ( cmsis.AR != address and not timeout ) { }
   BOOST_CHECK_EQUAL (bool(timeout), false);
   BOOST_CHECK_EQUAL (bool(cmsis.CR & FLASH_CR_PER_Msk), true);
   BOOST_CHECK_EQUAL (cmsis.AR, address);
   BOOST_CHECK_EQUAL (bool(cmsis.CR & FLASH_CR_STRT_Msk), false);

   timeout.restart();
   while (not(cmsis.CR & FLASH_CR_STRT_Msk) and not timeout) { }
   BOOST_CHECK_EQUAL (bool(timeout), false);
   BOOST_CHECK_EQUAL (bool(cmsis.CR & FLASH_CR_PER_Msk), true);
   BOOST_CHECK_EQUAL (cmsis.AR, address);
   BOOST_CHECK_EQUAL (bool(cmsis.CR & FLASH_CR_STRT_Msk), true);
};

BOOST_AUTO_TEST_CASE (start_erase_sector)
{
   test<mcu::FLASH::Sector::_11>();
   test<mcu::FLASH::Sector::_31>();
   test<mcu::FLASH::Sector::_3>();
}

BOOST_AUTO_TEST_CASE (address)
{
   BOOST_CHECK_EQUAL (mcu::FLASH::address<mcu::FLASH::Sector:: _0>(), 0x08000000);
   BOOST_CHECK_EQUAL (mcu::FLASH::address<mcu::FLASH::Sector:: _1>(), 0x08000400);
   BOOST_CHECK_EQUAL (mcu::FLASH::address<mcu::FLASH::Sector:: _2>(), 0x08000800);
   BOOST_CHECK_EQUAL (mcu::FLASH::address<mcu::FLASH::Sector:: _3>(), 0x08000C00);
   BOOST_CHECK_EQUAL (mcu::FLASH::address<mcu::FLASH::Sector:: _4>(), 0x08001000);
   BOOST_CHECK_EQUAL (mcu::FLASH::address<mcu::FLASH::Sector:: _5>(), 0x08001400);
   BOOST_CHECK_EQUAL (mcu::FLASH::address<mcu::FLASH::Sector:: _6>(), 0x08001800);
   BOOST_CHECK_EQUAL (mcu::FLASH::address<mcu::FLASH::Sector:: _7>(), 0x08001C00);
   BOOST_CHECK_EQUAL (mcu::FLASH::address<mcu::FLASH::Sector:: _8>(), 0x08002000);
   BOOST_CHECK_EQUAL (mcu::FLASH::address<mcu::FLASH::Sector:: _9>(), 0x08002400);
   BOOST_CHECK_EQUAL (mcu::FLASH::address<mcu::FLASH::Sector::_10>(), 0x08002800);
   BOOST_CHECK_EQUAL (mcu::FLASH::address<mcu::FLASH::Sector::_11>(), 0x08002C00);
   BOOST_CHECK_EQUAL (mcu::FLASH::address<mcu::FLASH::Sector::_12>(), 0x08003000);
   BOOST_CHECK_EQUAL (mcu::FLASH::address<mcu::FLASH::Sector::_30>(), 0x08007800);
   BOOST_CHECK_EQUAL (mcu::FLASH::address<mcu::FLASH::Sector::_31>(), 0x08007C00);
#if defined(STM32F1)
   BOOST_CHECK_EQUAL (mcu::FLASH::address<mcu::FLASH::Sector::_32>(), 0x08008000);
   BOOST_CHECK_EQUAL (mcu::FLASH::address<mcu::FLASH::Sector::_33>(), 0x08008400);
   BOOST_CHECK_EQUAL (mcu::FLASH::address<mcu::FLASH::Sector::_43>(), 0x0800AC00);
   BOOST_CHECK_EQUAL (mcu::FLASH::address<mcu::FLASH::Sector::_54>(), 0x0800D800);
   BOOST_CHECK_EQUAL (mcu::FLASH::address<mcu::FLASH::Sector::_65>(), 0x08010400);
   BOOST_CHECK_EQUAL (mcu::FLASH::address<mcu::FLASH::Sector::_76>(), 0x08013000);
   BOOST_CHECK_EQUAL (mcu::FLASH::address<mcu::FLASH::Sector::_87>(), 0x08015C00);
   BOOST_CHECK_EQUAL (mcu::FLASH::address<mcu::FLASH::Sector::_98>(), 0x08018800);
   BOOST_CHECK_EQUAL (mcu::FLASH::address<mcu::FLASH::Sector::_109>(), 0x0801B400);
   BOOST_CHECK_EQUAL (mcu::FLASH::address<mcu::FLASH::Sector::_115>(), 0x0801CC00);
   BOOST_CHECK_EQUAL (mcu::FLASH::address<mcu::FLASH::Sector::_120>(), 0x0801E000);
   BOOST_CHECK_EQUAL (mcu::FLASH::address<mcu::FLASH::Sector::_127>(), 0x0801FC00);
   
#endif
}

BOOST_AUTO_TEST_CASE (size)
{
   BOOST_CHECK_EQUAL (mcu::FLASH::size<mcu::FLASH::Sector:: _0>(), 1024);
   BOOST_CHECK_EQUAL (mcu::FLASH::size<mcu::FLASH::Sector:: _1>(), 1024);
   BOOST_CHECK_EQUAL (mcu::FLASH::size<mcu::FLASH::Sector:: _2>(), 1024);
   // ...
   BOOST_CHECK_EQUAL (mcu::FLASH::size<mcu::FLASH::Sector::_11>(), 1024);
   // ...
   BOOST_CHECK_EQUAL (mcu::FLASH::size<mcu::FLASH::Sector::_30>(), 1024);
   BOOST_CHECK_EQUAL (mcu::FLASH::size<mcu::FLASH::Sector::_31>(), 1024);

#if defined(STM32F1)
   BOOST_CHECK_EQUAL (mcu::FLASH::size<mcu::FLASH::Sector::_50>(), 1024);
   BOOST_CHECK_EQUAL (mcu::FLASH::size<mcu::FLASH::Sector::_71>(), 1024);
   BOOST_CHECK_EQUAL (mcu::FLASH::size<mcu::FLASH::Sector::_100>(), 1024);
   BOOST_CHECK_EQUAL (mcu::FLASH::size<mcu::FLASH::Sector::_121>(), 1024);
#endif
}

BOOST_AUTO_TEST_SUITE_END()