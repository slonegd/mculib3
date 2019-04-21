BOOST_AUTO_TEST_SUITE (test_suite_main)

mcu::RCC rcc {};
auto& CMSIS = rcc.like_CMSIS();

BOOST_AUTO_TEST_CASE (like_CMSIS)
{
   auto same = std::is_same_v<std::remove_reference_t<decltype(CMSIS)>, RCC_TypeDef>;
   auto address_rcc = reinterpret_cast<size_t>(&rcc);
   auto address_CMSIS = reinterpret_cast<size_t>(&CMSIS);
   BOOST_CHECK_EQUAL (address_rcc, address_CMSIS);
   BOOST_CHECK_EQUAL (same, true);
}

BOOST_AUTO_TEST_CASE (set_AHBprescaler)
{
   CMSIS.CFGR = 0;
   rcc.set (mcu::RCC::AHBprescaler::AHBdiv512);
   BOOST_CHECK_EQUAL (CMSIS.CFGR, RCC_CFGR_HPRE_DIV512);

   rcc.set (mcu::RCC::AHBprescaler::AHBdiv256);
   BOOST_CHECK_EQUAL (CMSIS.CFGR, RCC_CFGR_HPRE_DIV256);

   rcc.set (mcu::RCC::AHBprescaler::AHBdiv128);
   BOOST_CHECK_EQUAL (CMSIS.CFGR, RCC_CFGR_HPRE_DIV128);

   rcc.set (mcu::RCC::AHBprescaler::AHBdiv64);
   BOOST_CHECK_EQUAL (CMSIS.CFGR, RCC_CFGR_HPRE_DIV64);

   rcc.set (mcu::RCC::AHBprescaler::AHBdiv16);
   BOOST_CHECK_EQUAL (CMSIS.CFGR, RCC_CFGR_HPRE_DIV16);

   rcc.set (mcu::RCC::AHBprescaler::AHBdiv8);
   BOOST_CHECK_EQUAL (CMSIS.CFGR, RCC_CFGR_HPRE_DIV8);

   rcc.set (mcu::RCC::AHBprescaler::AHBdiv4);
   BOOST_CHECK_EQUAL (CMSIS.CFGR, RCC_CFGR_HPRE_DIV4);

   rcc.set (mcu::RCC::AHBprescaler::AHBdiv2);
   BOOST_CHECK_EQUAL (CMSIS.CFGR, RCC_CFGR_HPRE_DIV2);

   rcc.set (mcu::RCC::AHBprescaler::AHBnotdiv);
   BOOST_CHECK_EQUAL (CMSIS.CFGR, RCC_CFGR_HPRE_DIV1);
}

#if defined(STM32F1) or defined(STM32F4) or defined(STM32F7)
BOOST_AUTO_TEST_CASE (set_APB1prescaler)
{
   CMSIS.CFGR = 0;
   rcc.set_APB1 (mcu::RCC::APBprescaler::APBdiv16);
   BOOST_CHECK_EQUAL (CMSIS.CFGR, RCC_CFGR_PPRE1_DIV16);

   rcc.set_APB1 (mcu::RCC::APBprescaler::APBdiv8);
   BOOST_CHECK_EQUAL (CMSIS.CFGR, RCC_CFGR_PPRE1_DIV8);

   rcc.set_APB1 (mcu::RCC::APBprescaler::APBdiv4);
   BOOST_CHECK_EQUAL (CMSIS.CFGR, RCC_CFGR_PPRE1_DIV4);

   rcc.set_APB1 (mcu::RCC::APBprescaler::APBdiv2);
   BOOST_CHECK_EQUAL (CMSIS.CFGR, RCC_CFGR_PPRE1_DIV2);

   rcc.set_APB1 (mcu::RCC::APBprescaler::APBnotdiv);
   BOOST_CHECK_EQUAL (CMSIS.CFGR, RCC_CFGR_PPRE1_DIV1);
}

BOOST_AUTO_TEST_CASE (set_APB2prescaler)
{
   CMSIS.CFGR = 0;
   rcc.set_APB2 (mcu::RCC::APBprescaler::APBdiv16);
   BOOST_CHECK_EQUAL (CMSIS.CFGR, RCC_CFGR_PPRE2_DIV16);

   rcc.set_APB2 (mcu::RCC::APBprescaler::APBdiv8);
   BOOST_CHECK_EQUAL (CMSIS.CFGR, RCC_CFGR_PPRE2_DIV8);

   rcc.set_APB2 (mcu::RCC::APBprescaler::APBdiv4);
   BOOST_CHECK_EQUAL (CMSIS.CFGR, RCC_CFGR_PPRE2_DIV4);

   rcc.set_APB2 (mcu::RCC::APBprescaler::APBdiv2);
   BOOST_CHECK_EQUAL (CMSIS.CFGR, RCC_CFGR_PPRE2_DIV2);

   rcc.set_APB2 (mcu::RCC::APBprescaler::APBnotdiv);
   BOOST_CHECK_EQUAL (CMSIS.CFGR, RCC_CFGR_PPRE2_DIV1);
}

#elif defined(STM32F0)
BOOST_AUTO_TEST_CASE (set_APBprescaler)
{
   CMSIS.CFGR = 0;
   uint32_t result {0};
   rcc.set (mcu::RCC::APBprescaler::APBdiv16);
   result = RCC_CFGR_PPRE_0 | RCC_CFGR_PPRE_1 | RCC_CFGR_PPRE_2;
   BOOST_CHECK_EQUAL (CMSIS.CFGR, result);

   rcc.set (mcu::RCC::APBprescaler::APBdiv8);
   result = RCC_CFGR_PPRE_1 | RCC_CFGR_PPRE_2;
   BOOST_CHECK_EQUAL (CMSIS.CFGR, result);

   rcc.set (mcu::RCC::APBprescaler::APBdiv4);
   result = RCC_CFGR_PPRE_0 | RCC_CFGR_PPRE_2;
   BOOST_CHECK_EQUAL (CMSIS.CFGR, result);

   rcc.set (mcu::RCC::APBprescaler::APBdiv2);
   result = RCC_CFGR_PPRE_2;
   BOOST_CHECK_EQUAL (CMSIS.CFGR, result);

   rcc.set (mcu::RCC::APBprescaler::APBnotdiv);
   result = 0;
   BOOST_CHECK_EQUAL (CMSIS.CFGR, result);
}
#endif

BOOST_AUTO_TEST_CASE (set_SystemClock)
{
   CMSIS.CFGR = 0;
   rcc.set (mcu::RCC::SystemClock::CS_PLL);
   BOOST_CHECK_EQUAL (CMSIS.CFGR, RCC_CFGR_SW_PLL);

   rcc.set (mcu::RCC::SystemClock::CS_HSE);
   BOOST_CHECK_EQUAL (CMSIS.CFGR, RCC_CFGR_SW_HSE);

   rcc.set (mcu::RCC::SystemClock::CS_HSI);
   BOOST_CHECK_EQUAL (CMSIS.CFGR, RCC_CFGR_SW_HSI);
}

#if defined(STM32F4) or defined(STM32F7)
BOOST_AUTO_TEST_CASE (set_PLLPdiv)
{
   CMSIS.PLLCFGR = 0;
   rcc.set (mcu::RCC::PLLPdiv::_8);
   BOOST_CHECK_EQUAL (CMSIS.PLLCFGR, RCC_PLLCFGR_PLLP_0 | RCC_PLLCFGR_PLLP_1);

   rcc.set (mcu::RCC::PLLPdiv::_6);
   BOOST_CHECK_EQUAL (CMSIS.PLLCFGR, RCC_PLLCFGR_PLLP_1);

   rcc.set (mcu::RCC::PLLPdiv::_4);
   BOOST_CHECK_EQUAL (CMSIS.PLLCFGR, RCC_PLLCFGR_PLLP_0);

   rcc.set (mcu::RCC::PLLPdiv::_2);
   BOOST_CHECK_EQUAL (CMSIS.PLLCFGR, 0);
}
#endif

BOOST_AUTO_TEST_CASE (set_PLLsource)
{
#if defined(STM32F4) or defined(STM32F7)
   CMSIS.PLLCFGR = 0;
   rcc.set (mcu::RCC::PLLsource::HSE);
   BOOST_CHECK_EQUAL (CMSIS.PLLCFGR, RCC_PLLCFGR_PLLSRC_HSE);

   rcc.set (mcu::RCC::PLLsource::HSIdiv2);
   BOOST_CHECK_EQUAL (CMSIS.PLLCFGR, RCC_PLLCFGR_PLLSRC_HSI);

#elif defined(STM32F0) or defined(STM32F1)
   CMSIS.CFGR = 0;
   rcc.set (mcu::RCC::PLLsource::HSE);
   BOOST_CHECK_EQUAL (CMSIS.CFGR, RCC_CFGR_PLLSRC_HSE_PREDIV);

   rcc.set (mcu::RCC::PLLsource::HSIdiv2);
   BOOST_CHECK_EQUAL (CMSIS.CFGR, RCC_CFGR_PLLSRC_HSI_DIV2);
#endif
}

#if defined(STM32F0) or defined(STM32F1)
BOOST_AUTO_TEST_CASE (set_PLLmultipler)
{
   CMSIS.CFGR = 0;
   rcc.set (mcu::RCC::PLLmultiplier::_16);
   BOOST_CHECK_EQUAL (CMSIS.CFGR,
        RCC_CFGR_PLLMUL_1
      | RCC_CFGR_PLLMUL_2
      | RCC_CFGR_PLLMUL_3
   );

   rcc.set (mcu::RCC::PLLmultiplier::_15);
   BOOST_CHECK_EQUAL (CMSIS.CFGR,
        RCC_CFGR_PLLMUL_0
      | RCC_CFGR_PLLMUL_2
      | RCC_CFGR_PLLMUL_3
   );

   rcc.set (mcu::RCC::PLLmultiplier::_3);
   BOOST_CHECK_EQUAL (CMSIS.CFGR,
        RCC_CFGR_PLLMUL_0
   );

   rcc.set (mcu::RCC::PLLmultiplier::_2);
   BOOST_CHECK_EQUAL (CMSIS.CFGR,
        0
   );
}
#endif

BOOST_AUTO_TEST_CASE (get_APB_clock)
{
#if  defined(STM32F1) or defined(STM32F4) or defined(STM32F7)
   rcc.set_APB1 (mcu::RCC::APBprescaler::APBnotdiv);
   BOOST_CHECK_EQUAL (rcc.get_APB1_clock(), F_CPU);

   rcc.set_APB2 (mcu::RCC::APBprescaler::APBdiv2);
   BOOST_CHECK_EQUAL (rcc.get_APB2_clock(), F_CPU / 2);

   rcc.set_APB1 (mcu::RCC::APBprescaler::APBdiv4);
   BOOST_CHECK_EQUAL (rcc.get_APB1_clock(), F_CPU / 4);

   rcc.set_APB2 (mcu::RCC::APBprescaler::APBdiv8);
   BOOST_CHECK_EQUAL (rcc.get_APB2_clock(), F_CPU / 8);

#elif defined(STM32F0)
   rcc.set (mcu::RCC::APBprescaler::APBnotdiv);
   BOOST_CHECK_EQUAL (rcc.get_APB_clock(), F_CPU);

   rcc.set (mcu::RCC::APBprescaler::APBdiv2);
   BOOST_CHECK_EQUAL (rcc.get_APB_clock(), F_CPU / 2);

   rcc.set (mcu::RCC::APBprescaler::APBdiv4);
   BOOST_CHECK_EQUAL (rcc.get_APB_clock(), F_CPU / 4);

   rcc.set (mcu::RCC::APBprescaler::APBdiv8);
   BOOST_CHECK_EQUAL (rcc.get_APB_clock(), F_CPU / 8);
#endif
}

#if defined(STM32F4) or defined(STM32F7)
BOOST_AUTO_TEST_CASE (set_PLLM)
{
   CMSIS.PLLCFGR = 0;
   STATIC_ASSERTATION_REQUIRED ( rcc.set_PLLM<0>(), "");
   STATIC_ASSERTATION_REQUIRED ( rcc.set_PLLM<1>(), "");
   STATIC_ASSERTATION_REQUIRED ( rcc.set_PLLM<64>(), "");

   rcc.set_PLLM<63>();
   BOOST_CHECK_EQUAL (CMSIS.PLLCFGR, 
        RCC_PLLCFGR_PLLM_0 
      | RCC_PLLCFGR_PLLM_1
      | RCC_PLLCFGR_PLLM_2
      | RCC_PLLCFGR_PLLM_3
      | RCC_PLLCFGR_PLLM_4
      | RCC_PLLCFGR_PLLM_5
   );

   rcc.set_PLLM<15>();
   BOOST_CHECK_EQUAL (CMSIS.PLLCFGR, 
        RCC_PLLCFGR_PLLM_0 
      | RCC_PLLCFGR_PLLM_1
      | RCC_PLLCFGR_PLLM_2
      | RCC_PLLCFGR_PLLM_3
   );

   rcc.set_PLLM<10>();
   BOOST_CHECK_EQUAL (CMSIS.PLLCFGR, 
        RCC_PLLCFGR_PLLM_1
      | RCC_PLLCFGR_PLLM_3
   );

   rcc.set_PLLM<9>();
   BOOST_CHECK_EQUAL (CMSIS.PLLCFGR, 
        RCC_PLLCFGR_PLLM_0
      | RCC_PLLCFGR_PLLM_3
   );

   rcc.set_PLLM<2>();
   BOOST_CHECK_EQUAL (CMSIS.PLLCFGR, RCC_PLLCFGR_PLLM_1);
}

BOOST_AUTO_TEST_CASE (set_PLLN)
{
   CMSIS.PLLCFGR = 0;
   STATIC_ASSERTATION_REQUIRED ( rcc.set_PLLN<0>(), "");
   STATIC_ASSERTATION_REQUIRED ( rcc.set_PLLN<49>(), "");
   STATIC_ASSERTATION_REQUIRED ( rcc.set_PLLN<433>(), "");

   rcc.set_PLLN<50>();
   auto res = CMSIS.PLLCFGR >> RCC_PLLCFGR_PLLN_Pos;
   BOOST_CHECK_EQUAL (res, 50);

   rcc.set_PLLN<51>();
   res = CMSIS.PLLCFGR >> RCC_PLLCFGR_PLLN_Pos;
   BOOST_CHECK_EQUAL (res, 51);

   rcc.set_PLLN<431>();
   res = CMSIS.PLLCFGR >> RCC_PLLCFGR_PLLN_Pos;
   BOOST_CHECK_EQUAL (res, 431);

   rcc.set_PLLN<432>();
   res = CMSIS.PLLCFGR >> RCC_PLLCFGR_PLLN_Pos;
   BOOST_CHECK_EQUAL (res, 432);
}

BOOST_AUTO_TEST_CASE (set_PLLQ)
{
   CMSIS.PLLCFGR = 0;
   STATIC_ASSERTATION_REQUIRED ( rcc.set_PLLQ<0>(), "");
   STATIC_ASSERTATION_REQUIRED ( rcc.set_PLLQ<1>(), "");
   STATIC_ASSERTATION_REQUIRED ( rcc.set_PLLQ<16>(), "");
   
   rcc.set_PLLQ<15>();
   auto res = CMSIS.PLLCFGR >> RCC_PLLCFGR_PLLQ_Pos;
   BOOST_CHECK_EQUAL (res, 15);

   rcc.set_PLLQ<14>();
   res = CMSIS.PLLCFGR >> RCC_PLLCFGR_PLLQ_Pos;
   BOOST_CHECK_EQUAL (res, 14);

   rcc.set_PLLQ<3>();
   res = CMSIS.PLLCFGR >> RCC_PLLCFGR_PLLQ_Pos;
   BOOST_CHECK_EQUAL (res, 3);

   rcc.set_PLLQ<2>();
   res = CMSIS.PLLCFGR >> RCC_PLLCFGR_PLLQ_Pos;
   BOOST_CHECK_EQUAL (res, 2);
}
#endif

BOOST_AUTO_TEST_CASE (on_HSE)
{
   CMSIS.CR = 0;
   rcc.on_HSE();
   BOOST_CHECK_EQUAL (CMSIS.CR, RCC_CR_HSEON_Msk);
}

BOOST_AUTO_TEST_CASE (wait_HSE_ready)
{
   CMSIS.CR = 0;

   volatile bool work {true};
   auto worker = [&]() { rcc.wait_HSE_ready(); work = false; };
   std::thread {worker}.detach();

   std::this_thread::sleep_for(std::chrono::milliseconds(10));

   BOOST_CHECK_EQUAL (work, true);

   CMSIS.CR |= RCC_CR_HSERDY_Msk;
   Timeout timeout {100};
   while (work) {
      if (timeout) break;
   }

   BOOST_CHECK_EQUAL (bool(timeout), false);
   BOOST_CHECK_EQUAL (work, false);
}

BOOST_AUTO_TEST_CASE (on_PLL)
{
   CMSIS.CR = 0;
   rcc.on_PLL();
   BOOST_CHECK_EQUAL (CMSIS.CR, RCC_CR_PLLON_Msk);
}

BOOST_AUTO_TEST_CASE (wait_PLL_ready)
{
   CMSIS.CR = 0;

   volatile bool work {true};
   auto worker = [&]() { rcc.wait_PLL_ready(); work = false; };
   std::thread {worker}.detach();

   std::this_thread::sleep_for(std::chrono::milliseconds(10));

   BOOST_CHECK_EQUAL (work, true);

   CMSIS.CR |= RCC_CR_PLLRDY_Msk;
   Timeout timeout {100};
   while (work) {
      if (timeout) break;
   }

   BOOST_CHECK_EQUAL (bool(timeout), false);
   BOOST_CHECK_EQUAL (work, false);
}

BOOST_AUTO_TEST_CASE (clock_enable)
{
#if defined(STM32F4) or defined(STM32F7)
   CMSIS.AHB1ENR = 0;
   rcc.clock_enable<mcu::Periph::GPIOA>();
   BOOST_CHECK_EQUAL (CMSIS.AHB1ENR, RCC_AHB1ENR_GPIOAEN_Msk);

   rcc.clock_enable<mcu::Periph::GPIOB>();
   BOOST_CHECK_EQUAL (CMSIS.AHB1ENR, RCC_AHB1ENR_GPIOBEN_Msk | RCC_AHB1ENR_GPIOAEN_Msk);

   CMSIS.AHB1ENR = 0;
   rcc.clock_enable<mcu::Periph::GPIOC>();
   BOOST_CHECK_EQUAL (CMSIS.AHB1ENR, RCC_AHB1ENR_GPIOCEN_Msk);

   CMSIS.AHB1ENR = 0;
   rcc.clock_enable<mcu::Periph::GPIOD>();
   BOOST_CHECK_EQUAL (CMSIS.AHB1ENR, RCC_AHB1ENR_GPIODEN_Msk);

   CMSIS.AHB1ENR = 0;
   rcc.clock_enable<mcu::Periph::GPIOE>();
   BOOST_CHECK_EQUAL (CMSIS.AHB1ENR, RCC_AHB1ENR_GPIOEEN_Msk);

   CMSIS.AHB1ENR = 0;
   rcc.clock_enable<mcu::Periph::GPIOF>();
   BOOST_CHECK_EQUAL (CMSIS.AHB1ENR, RCC_AHB1ENR_GPIOFEN_Msk);

   CMSIS.AHB1ENR = 0;
   rcc.clock_enable<mcu::Periph::GPIOG>();
   BOOST_CHECK_EQUAL (CMSIS.AHB1ENR, RCC_AHB1ENR_GPIOGEN_Msk);

   CMSIS.AHB1ENR = 0;
   rcc.clock_enable<mcu::Periph::GPIOH>();
   BOOST_CHECK_EQUAL (CMSIS.AHB1ENR, RCC_AHB1ENR_GPIOHEN_Msk);

   CMSIS.AHB1ENR = 0;
   rcc.clock_enable<mcu::Periph::GPIOI>();
   BOOST_CHECK_EQUAL (CMSIS.AHB1ENR, RCC_AHB1ENR_GPIOIEN_Msk);

#elif defined(STM32F0)
   CMSIS.AHBENR = 0;
   rcc.clock_enable<mcu::Periph::GPIOA>();
   BOOST_CHECK_EQUAL (CMSIS.AHBENR, RCC_AHBENR_GPIOAEN_Msk);

   CMSIS.AHBENR = 0;
   rcc.clock_enable<mcu::Periph::GPIOB>();
   BOOST_CHECK_EQUAL (CMSIS.AHBENR, RCC_AHBENR_GPIOBEN_Msk);

   CMSIS.AHBENR = 0;
   rcc.clock_enable<mcu::Periph::GPIOC>();
   BOOST_CHECK_EQUAL (CMSIS.AHBENR, RCC_AHBENR_GPIOCEN_Msk);

   CMSIS.AHBENR = 0;
   rcc.clock_enable<mcu::Periph::GPIOD>();
   BOOST_CHECK_EQUAL (CMSIS.AHBENR, RCC_AHBENR_GPIODEN_Msk);

   CMSIS.AHBENR = 0;
   rcc.clock_enable<mcu::Periph::GPIOF>();
   BOOST_CHECK_EQUAL (CMSIS.AHBENR, RCC_AHBENR_GPIOFEN_Msk);

#elif defined(STM32F1)
   CMSIS.APB2ENR = 0;
   rcc.clock_enable<mcu::Periph::GPIOA>();
   BOOST_CHECK_EQUAL (CMSIS.APB2ENR, RCC_APB2ENR_IOPAEN_Msk);

   CMSIS.APB2ENR = 0;
   rcc.clock_enable<mcu::Periph::GPIOB>();
   BOOST_CHECK_EQUAL (CMSIS.APB2ENR, RCC_APB2ENR_IOPBEN_Msk);

   CMSIS.APB2ENR = 0;
   rcc.clock_enable<mcu::Periph::GPIOC>();
   BOOST_CHECK_EQUAL (CMSIS.APB2ENR, RCC_APB2ENR_IOPCEN_Msk);

   CMSIS.APB2ENR = 0;
   rcc.clock_enable<mcu::Periph::GPIOD>();
   BOOST_CHECK_EQUAL (CMSIS.APB2ENR, RCC_APB2ENR_IOPDEN_Msk);

   CMSIS.APB2ENR = 0;
   rcc.clock_enable<mcu::Periph::GPIOE>();
   BOOST_CHECK_EQUAL (CMSIS.APB2ENR, RCC_APB2ENR_IOPEEN_Msk);

   CMSIS.APB2ENR = 0;
   rcc.clock_enable<mcu::Periph::AFIO>();
   BOOST_CHECK_EQUAL (CMSIS.APB2ENR, RCC_APB2ENR_AFIOEN_Msk);
#endif


   CMSIS.APB2ENR = 0;
   rcc.clock_enable<mcu::Periph::USART1>();
   BOOST_CHECK_EQUAL (CMSIS.APB2ENR, RCC_APB2ENR_USART1EN_Msk);

#if defined(STM32F1) or defined(STM32F4) or defined(STM32F7)
   CMSIS.APB1ENR = 0;
   rcc.clock_enable<mcu::Periph::USART2>();
   BOOST_CHECK_EQUAL (CMSIS.APB1ENR, RCC_APB1ENR_USART2EN_Msk);

   CMSIS.APB1ENR = 0;
   rcc.clock_enable<mcu::Periph::USART3>();
   BOOST_CHECK_EQUAL (CMSIS.APB1ENR, RCC_APB1ENR_USART3EN_Msk);
#endif
#if defined(STM32F4) or defined(STM32F7)
   CMSIS.APB1ENR = 0;
   rcc.clock_enable<mcu::Periph::USART4>();
   BOOST_CHECK_EQUAL (CMSIS.APB1ENR, RCC_APB1ENR_UART4EN_Msk);

   CMSIS.APB1ENR = 0;
   rcc.clock_enable<mcu::Periph::USART5>();
   BOOST_CHECK_EQUAL (CMSIS.APB1ENR, RCC_APB1ENR_UART5EN_Msk);

   CMSIS.APB2ENR = 0;
   rcc.clock_enable<mcu::Periph::USART6>();
   BOOST_CHECK_EQUAL (CMSIS.APB2ENR, RCC_APB2ENR_USART6EN_Msk);
#endif
#if defined(STM32F7) 
   CMSIS.APB1ENR = 0;
   rcc.clock_enable<mcu::Periph::USART7>();
   BOOST_CHECK_EQUAL (CMSIS.APB1ENR, RCC_APB1ENR_UART7EN_Msk);

   CMSIS.APB1ENR = 0;
   rcc.clock_enable<mcu::Periph::USART8>();
   BOOST_CHECK_EQUAL (CMSIS.APB1ENR, RCC_APB1ENR_UART8EN_Msk);
#endif

#if defined(STM32F0)
   CMSIS.APB2ENR = 0;
   rcc.clock_enable<mcu::Periph::TIM1>();
   BOOST_CHECK_EQUAL (CMSIS.APB2ENR, RCC_APB2ENR_TIM1EN_Msk);

   CMSIS.APB1ENR = 0;
   rcc.clock_enable<mcu::Periph::TIM3>();
   BOOST_CHECK_EQUAL (CMSIS.APB1ENR, RCC_APB1ENR_TIM3EN_Msk);

   CMSIS.APB1ENR = 0;
   rcc.clock_enable<mcu::Periph::TIM14>();
   BOOST_CHECK_EQUAL (CMSIS.APB1ENR, RCC_APB1ENR_TIM14EN_Msk);

   CMSIS.APB2ENR = 0;
   rcc.clock_enable<mcu::Periph::TIM16>();
   BOOST_CHECK_EQUAL (CMSIS.APB2ENR, RCC_APB2ENR_TIM16EN_Msk);

   CMSIS.APB2ENR = 0;
   rcc.clock_enable<mcu::Periph::TIM17>();
   BOOST_CHECK_EQUAL (CMSIS.APB2ENR, RCC_APB2ENR_TIM17EN_Msk);
#endif

#if defined(STM32F0) or defined(STM32F1) 
   CMSIS.AHBENR = 0;
   rcc.clock_enable<mcu::Periph::DMA1>();
   BOOST_CHECK_EQUAL (CMSIS.AHBENR, RCC_AHBENR_DMA1EN_Msk);

#elif defined(STM32F4) or defined(STM32F7)
   CMSIS.AHB1ENR = 0;
   rcc.clock_enable<mcu::Periph::DMA1>();
   BOOST_CHECK_EQUAL (CMSIS.AHB1ENR, RCC_AHB1ENR_DMA1EN_Msk);

   CMSIS.AHB1ENR = 0;
   rcc.clock_enable<mcu::Periph::DMA2>();
   BOOST_CHECK_EQUAL (CMSIS.AHB1ENR, RCC_AHB1ENR_DMA2EN_Msk);
#endif

#if defined(STM32F4)
   CMSIS.APB2ENR = 0;
   rcc.clock_enable<mcu::Periph::SPI1>();
   BOOST_CHECK_EQUAL (CMSIS.APB2ENR, RCC_APB2ENR_SPI1EN_Msk);

   CMSIS.APB1ENR = 0;
   rcc.clock_enable<mcu::Periph::SPI2>();
   BOOST_CHECK_EQUAL (CMSIS.APB1ENR, RCC_APB1ENR_SPI2EN_Msk);

   CMSIS.APB1ENR = 0;
   rcc.clock_enable<mcu::Periph::SPI3>();
   BOOST_CHECK_EQUAL (CMSIS.APB1ENR, RCC_APB1ENR_SPI3EN_Msk);
#endif

#if defined(STM32F0)
   CMSIS.APB2ENR = 0;
   rcc.clock_enable<mcu::Periph::ADC1>();
   BOOST_CHECK_EQUAL (CMSIS.APB2ENR, RCC_APB2ENR_ADCEN_Msk);
#endif
}

BOOST_AUTO_TEST_CASE (bad_clock_enable)
{
   STATIC_ASSERTATION_REQUIRED (
      rcc.clock_enable<mcu::Periph::RCC>()
      , "допиши clock_enable");
}

BOOST_AUTO_TEST_CASE (clock)
{
#if defined(STM32F0)
   rcc.set (mcu::RCC::APBprescaler::APBnotdiv);
   BOOST_CHECK_EQUAL (rcc.clock(mcu::Periph::USART1), F_CPU);

   rcc.set (mcu::RCC::APBprescaler::APBdiv2);
   BOOST_CHECK_EQUAL (rcc.clock(mcu::Periph::USART1), F_CPU / 2);

   rcc.set (mcu::RCC::APBprescaler::APBdiv16);
   BOOST_CHECK_EQUAL (rcc.clock(mcu::Periph::USART1), F_CPU / 16);

#elif defined(STM32F1) or defined(STM32F4) or defined(STM32F7) 
   rcc.set_APB1 (mcu::RCC::APBprescaler::APBnotdiv);
   rcc.set_APB2 (mcu::RCC::APBprescaler::APBnotdiv);
   BOOST_CHECK_EQUAL (rcc.clock(mcu::Periph::USART1), F_CPU);
   BOOST_CHECK_EQUAL (rcc.clock(mcu::Periph::USART2), F_CPU);
   BOOST_CHECK_EQUAL (rcc.clock(mcu::Periph::USART3), F_CPU);

   rcc.set_APB2 (mcu::RCC::APBprescaler::APBdiv2);
   BOOST_CHECK_EQUAL (rcc.clock(mcu::Periph::USART1), F_CPU / 2);
   BOOST_CHECK_EQUAL (rcc.clock(mcu::Periph::USART2), F_CPU);
   BOOST_CHECK_EQUAL (rcc.clock(mcu::Periph::USART3), F_CPU);

   rcc.set_APB1 (mcu::RCC::APBprescaler::APBdiv16);
   BOOST_CHECK_EQUAL (rcc.clock(mcu::Periph::USART1), F_CPU / 2);
   BOOST_CHECK_EQUAL (rcc.clock(mcu::Periph::USART2), F_CPU / 16);
   BOOST_CHECK_EQUAL (rcc.clock(mcu::Periph::USART3), F_CPU / 16);
#endif
#if defined(STM32F4) or defined(STM32F7)
   BOOST_CHECK_EQUAL (rcc.clock(mcu::Periph::USART4), F_CPU / 16);
   BOOST_CHECK_EQUAL (rcc.clock(mcu::Periph::USART5), F_CPU / 16);
   BOOST_CHECK_EQUAL (rcc.clock(mcu::Periph::USART6), F_CPU / 2);
#endif
#if defined(STM32F7)
   BOOST_CHECK_EQUAL (rcc.clock(mcu::Periph::USART7), F_CPU / 16);
   BOOST_CHECK_EQUAL (rcc.clock(mcu::Periph::USART8), F_CPU / 16);
#endif
}

BOOST_AUTO_TEST_SUITE_END()
