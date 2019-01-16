BOOST_AUTO_TEST_SUITE (test_suite_main)

mcu::GPIO gpio {};
auto& cmsis = gpio.like_CMSIS();

BOOST_AUTO_TEST_CASE (like_CMSIS)
{
   auto same = std::is_same_v<std::remove_reference_t<decltype(cmsis)>, GPIO_TypeDef>;
   auto address_rcc = reinterpret_cast<size_t>(&gpio);
   auto address_CMSIS = reinterpret_cast<size_t>(&cmsis);
   BOOST_CHECK_EQUAL (address_rcc, address_CMSIS);
   BOOST_CHECK_EQUAL (same, true);
}

BOOST_AUTO_TEST_CASE (set_n)
{
   cmsis.BSRR = 0;
   gpio.set (0);
   BOOST_CHECK_EQUAL (cmsis.BSRR, GPIO_BSRR_BS_0);

   cmsis.BSRR = 0;
   gpio.set (1);
   BOOST_CHECK_EQUAL (cmsis.BSRR, GPIO_BSRR_BS_1);

   cmsis.BSRR = 0;
   gpio.set (14);
   BOOST_CHECK_EQUAL (cmsis.BSRR, GPIO_BSRR_BS_14);

   cmsis.BSRR = 0;
   gpio.set (15);
   BOOST_CHECK_EQUAL (cmsis.BSRR, GPIO_BSRR_BS_15);
}

BOOST_AUTO_TEST_CASE (clear_n)
{
   cmsis.BSRR = 0;
   gpio.clear (0);
   BOOST_CHECK_EQUAL (cmsis.BSRR, GPIO_BSRR_BR_0);

   cmsis.BSRR = 0;
   gpio.clear (1);
   BOOST_CHECK_EQUAL (cmsis.BSRR, GPIO_BSRR_BR_1);

   cmsis.BSRR = 0;
   gpio.clear (14);
   BOOST_CHECK_EQUAL (cmsis.BSRR, GPIO_BSRR_BR_14);

   cmsis.BSRR = 0;
   gpio.clear (15);
   BOOST_CHECK_EQUAL (cmsis.BSRR, GPIO_BSRR_BR_15);
}

BOOST_AUTO_TEST_CASE (is_set_n)
{
   cmsis.IDR = 0;
   BOOST_CHECK_EQUAL (gpio.is_set(0) , false);
   BOOST_CHECK_EQUAL (gpio.is_set(1) , false);
   BOOST_CHECK_EQUAL (gpio.is_set(14), false);
   BOOST_CHECK_EQUAL (gpio.is_set(15), false);

   cmsis.IDR = 1 << 0;
   BOOST_CHECK_EQUAL (gpio.is_set(0) , true);
   BOOST_CHECK_EQUAL (gpio.is_set(1) , false);
   BOOST_CHECK_EQUAL (gpio.is_set(14), false);
   BOOST_CHECK_EQUAL (gpio.is_set(15), false);

   cmsis.IDR = 1 << 1;
   BOOST_CHECK_EQUAL (gpio.is_set(0) , false);
   BOOST_CHECK_EQUAL (gpio.is_set(1) , true);
   BOOST_CHECK_EQUAL (gpio.is_set(14), false);
   BOOST_CHECK_EQUAL (gpio.is_set(15), false);

   cmsis.IDR = 1 << 0 | 1 << 1;
   BOOST_CHECK_EQUAL (gpio.is_set(0) , true);
   BOOST_CHECK_EQUAL (gpio.is_set(1) , true);
   BOOST_CHECK_EQUAL (gpio.is_set(14), false);
   BOOST_CHECK_EQUAL (gpio.is_set(15), false);

   cmsis.IDR |= 1 << 15;
   BOOST_CHECK_EQUAL (gpio.is_set(0) , true);
   BOOST_CHECK_EQUAL (gpio.is_set(1) , true);
   BOOST_CHECK_EQUAL (gpio.is_set(14), false);
   BOOST_CHECK_EQUAL (gpio.is_set(15), true);

   cmsis.IDR &= ~(1 << 1);
   BOOST_CHECK_EQUAL (gpio.is_set(0) , true);
   BOOST_CHECK_EQUAL (gpio.is_set(1) , false);
   BOOST_CHECK_EQUAL (gpio.is_set(14), false);
   BOOST_CHECK_EQUAL (gpio.is_set(15), true);

   cmsis.IDR |= 1 << 14;
   BOOST_CHECK_EQUAL (gpio.is_set(0) , true);
   BOOST_CHECK_EQUAL (gpio.is_set(1) , false);
   BOOST_CHECK_EQUAL (gpio.is_set(14), true);
   BOOST_CHECK_EQUAL (gpio.is_set(15), true);
}

BOOST_AUTO_TEST_CASE (toogle_n)
{
   cmsis.BSRR = 0;
   cmsis.IDR  = 1 << 0;
   gpio.toggle (0);
   BOOST_CHECK_EQUAL (cmsis.BSRR, GPIO_BSRR_BR_0);

   cmsis.BSRR = 0;
   cmsis.IDR  = 0;
   gpio.toggle (0);
   BOOST_CHECK_EQUAL (cmsis.BSRR, GPIO_BSRR_BS_0);

   cmsis.BSRR = 0;
   cmsis.IDR  = 1 << 0;
   gpio.toggle (1);
   BOOST_CHECK_EQUAL (cmsis.BSRR, GPIO_BSRR_BS_1);

   cmsis.BSRR = 0;
   gpio.toggle (14);
   BOOST_CHECK_EQUAL (cmsis.BSRR, GPIO_BSRR_BS_14);
   cmsis.BSRR = 0;
   gpio.toggle (0);
   BOOST_CHECK_EQUAL (cmsis.BSRR, GPIO_BSRR_BR_0);

   cmsis.BSRR = 0;
   cmsis.IDR  = 1 << 15;
   gpio.toggle (15);
   BOOST_CHECK_EQUAL (cmsis.BSRR, GPIO_BSRR_BR_15);

   cmsis.BSRR = 0;
   cmsis.IDR  = 0;
   gpio.toggle (15);
   BOOST_CHECK_EQUAL (cmsis.BSRR, GPIO_BSRR_BS_15);
}

#if defined(STM32F0) or defined(STM32F4) or defined(STM32F7)
BOOST_AUTO_TEST_CASE (init_input_output)
{
   cmsis.MODER = 0xFFFFFFFF;
   gpio.init<mcu::PA0, mcu::PinMode::Input>();
   BOOST_CHECK_EQUAL (cmsis.MODER, 0xFFFFFFFF & ~(GPIO_MODER_MODER0_Msk));

   cmsis.MODER = 0xFFFFFFFF;
   gpio.init<mcu::PB0, mcu::PinMode::Input>();
   BOOST_CHECK_EQUAL (cmsis.MODER, 0xFFFFFFFF & ~(GPIO_MODER_MODER0_Msk));

   cmsis.MODER = 0;
   gpio.init<mcu::PA1, mcu::PinMode::Output>();
   BOOST_CHECK_EQUAL (cmsis.MODER, GPIO_MODER_MODER1_0);

   gpio.init<mcu::PC15, mcu::PinMode::Output>();
   BOOST_CHECK_EQUAL (cmsis.MODER, GPIO_MODER_MODER1_0 | GPIO_MODER_MODER15_0);
}

BOOST_AUTO_TEST_CASE (init_alternate)
{
   // PinMode::Alternate_0
   cmsis.MODER  = 0;
   cmsis.AFR[0] = 0xFFFFFFFF;
   gpio.init<mcu::PA0, mcu::PinMode::Alternate_0>();
   BOOST_CHECK_EQUAL (cmsis.MODER, GPIO_MODER_MODER0_1);
   BOOST_CHECK_EQUAL (cmsis.AFR[0], 0xFFFFFFFF & ~(GPIO_AFRL_AFRL0_Msk));

   gpio.init<mcu::PB0, mcu::PinMode::Alternate_0>();
   BOOST_CHECK_EQUAL (cmsis.MODER, GPIO_MODER_MODER0_1);
   BOOST_CHECK_EQUAL (cmsis.AFR[0], 0xFFFFFFFF & ~(GPIO_AFRL_AFRL0_Msk));

   cmsis.MODER  = 0;
   cmsis.AFR[0] = 0xFFFFFFFF;
   gpio.init<mcu::PC7, mcu::PinMode::Alternate_0>();
   BOOST_CHECK_EQUAL (cmsis.MODER, GPIO_MODER_MODER7_1);
   BOOST_CHECK_EQUAL (cmsis.AFR[0], 0xFFFFFFFF & ~(GPIO_AFRL_AFRL7_Msk));

   cmsis.MODER  = 0;
   cmsis.AFR[1] = 0xFFFFFFFF;
   gpio.init<mcu::PD8, mcu::PinMode::Alternate_0>();
   BOOST_CHECK_EQUAL (cmsis.MODER, GPIO_MODER_MODER8_1);
   BOOST_CHECK_EQUAL (cmsis.AFR[1], 0xFFFFFFFF & ~(GPIO_AFRH_AFRH0_Msk));

   gpio.init<mcu::PA15, mcu::PinMode::Alternate_0>();
   BOOST_CHECK_EQUAL (cmsis.MODER, GPIO_MODER_MODER8_1 | GPIO_MODER_MODER15_1);
   BOOST_CHECK_EQUAL (cmsis.AFR[1], 0xFFFFFFFF & ~(GPIO_AFRH_AFRH0_Msk | GPIO_AFRH_AFRH7_Msk));

   // PinMode::Alternate_1
   cmsis.MODER  = 0;
   cmsis.AFR[0] = 0;
   cmsis.AFR[1] = 0;
   gpio.init<mcu::PA0, mcu::PinMode::Alternate_1>();
   BOOST_CHECK_EQUAL (cmsis.MODER, GPIO_MODER_MODER0_1);
   BOOST_CHECK_EQUAL (cmsis.AFR[0], GPIO_AFRL_AFRL0_0);

   gpio.init<mcu::PA1, mcu::PinMode::Alternate_1>();
   BOOST_CHECK_EQUAL (cmsis.MODER, GPIO_MODER_MODER0_1 | GPIO_MODER_MODER1_1);
   BOOST_CHECK_EQUAL (cmsis.AFR[0], GPIO_AFRL_AFRL0_0 | GPIO_AFRL_AFRL1_0);

   gpio.init<mcu::PA8, mcu::PinMode::Alternate_1>();
   BOOST_CHECK_EQUAL (cmsis.MODER, GPIO_MODER_MODER0_1 | GPIO_MODER_MODER1_1 | GPIO_MODER_MODER8_1);
   BOOST_CHECK_EQUAL (cmsis.AFR[0], GPIO_AFRL_AFRL0_0 | GPIO_AFRL_AFRL1_0);
   BOOST_CHECK_EQUAL (cmsis.AFR[1], GPIO_AFRH_AFRH0_0);

   // PinMode::Alternate_2
   gpio.init<mcu::PA1, mcu::PinMode::Alternate_2>();
   BOOST_CHECK_EQUAL (cmsis.MODER, GPIO_MODER_MODER0_1 | GPIO_MODER_MODER1_1 | GPIO_MODER_MODER8_1);
   BOOST_CHECK_EQUAL (cmsis.AFR[0], GPIO_AFRL_AFRL0_0 | GPIO_AFRL_AFRL1_1);
   BOOST_CHECK_EQUAL (cmsis.AFR[1], GPIO_AFRH_AFRH0_0);

   // PinMode::Alternate_7
   gpio.init<mcu::PA15, mcu::PinMode::Alternate_7>();
   BOOST_CHECK_EQUAL (cmsis.MODER, GPIO_MODER_MODER0_1 | GPIO_MODER_MODER1_1 | GPIO_MODER_MODER8_1 | GPIO_MODER_MODER15_1);
   BOOST_CHECK_EQUAL (cmsis.AFR[0], GPIO_AFRL_AFRL0_0 | GPIO_AFRL_AFRL1_1);
   BOOST_CHECK_EQUAL (cmsis.AFR[1], GPIO_AFRH_AFRH0_0 | GPIO_AFRH_AFRH7_0 | GPIO_AFRH_AFRH7_1 | GPIO_AFRH_AFRH7_2);

#if defined(STM32F4) or defined(STM32F7)
   // PinMode::Alternate_8
   gpio.init<mcu::PA14, mcu::PinMode::Alternate_8>();
   BOOST_CHECK_EQUAL (cmsis.MODER, 
        GPIO_MODER_MODER0_1 
      | GPIO_MODER_MODER1_1 
      | GPIO_MODER_MODER8_1 
      | GPIO_MODER_MODER15_1 
      | GPIO_MODER_MODER14_1
   );
   BOOST_CHECK_EQUAL (cmsis.AFR[0], 
        GPIO_AFRL_AFRL0_0 
      | GPIO_AFRL_AFRL1_1
   );
   BOOST_CHECK_EQUAL (cmsis.AFR[1], 
        GPIO_AFRH_AFRH0_0 
      | GPIO_AFRH_AFRH7_0 
      | GPIO_AFRH_AFRH7_1 
      | GPIO_AFRH_AFRH7_2
      | GPIO_AFRH_AFRH6_3
   );
#endif
}
#endif // #if defined(STM32F0) or defined(STM32F4) or defined(STM32F7)

#if defined(STM32F1)
BOOST_AUTO_TEST_CASE (init_input_output)
{
   cmsis.CRL = 0;
   cmsis.CRH = 0;
   gpio.init<mcu::PA0, mcu::PinMode::Input>();
   BOOST_CHECK_EQUAL (cmsis.CRL,
      GPIO_CRL_CNF0_0
   );

   gpio.init<mcu::PB1, mcu::PinMode::Input>();
   BOOST_CHECK_EQUAL (cmsis.CRL,
        GPIO_CRL_CNF0_0
      | GPIO_CRL_CNF1_0
   );

   gpio.init<mcu::PC7, mcu::PinMode::Input>();
   BOOST_CHECK_EQUAL (cmsis.CRL,
        GPIO_CRL_CNF0_0
      | GPIO_CRL_CNF1_0
      | GPIO_CRL_CNF7_0
   );

   gpio.init<mcu::PD8, mcu::PinMode::Input>();
   BOOST_CHECK_EQUAL (cmsis.CRL,
        GPIO_CRL_CNF0_0
      | GPIO_CRL_CNF1_0
      | GPIO_CRL_CNF7_0
   );
   BOOST_CHECK_EQUAL (cmsis.CRH,
        GPIO_CRH_CNF8_0
   );

   gpio.init<mcu::PE14, mcu::PinMode::Input>();
   BOOST_CHECK_EQUAL (cmsis.CRL,
        GPIO_CRL_CNF0_0
      | GPIO_CRL_CNF1_0
      | GPIO_CRL_CNF7_0
   );
   BOOST_CHECK_EQUAL (cmsis.CRH,
        GPIO_CRH_CNF8_0
      | GPIO_CRH_CNF14_0
   );

   gpio.init<mcu::PB0, mcu::PinMode::Output>();
   BOOST_CHECK_EQUAL (cmsis.CRL,
        GPIO_CRL_MODE0_1
      | GPIO_CRL_CNF1_0
      | GPIO_CRL_CNF7_0
   );
   BOOST_CHECK_EQUAL (cmsis.CRH,
        GPIO_CRH_CNF8_0
      | GPIO_CRH_CNF14_0
   );

   gpio.init<mcu::PA3, mcu::PinMode::Output>();
   BOOST_CHECK_EQUAL (cmsis.CRL,
        GPIO_CRL_MODE0_1
      | GPIO_CRL_CNF1_0
      | GPIO_CRL_MODE3_1
      | GPIO_CRL_CNF7_0
   );
   BOOST_CHECK_EQUAL (cmsis.CRH,
        GPIO_CRH_CNF8_0
      | GPIO_CRH_CNF14_0
   );
}

std::stringstream process;
void prepare_test()
{
   cmsis.CRL = 0;
   cmsis.CRH = 0;
   mcu::make_reference<mcu::Periph::RCC>()
      .set_stream (process);
   mcu::make_reference<mcu::Periph::AFIO>()
      .set_stream (process);
}
void clear_stream() { process.str (std::string{}); }

BOOST_AUTO_TEST_CASE (init_alternate_usart1)
{
   prepare_test();

   gpio.init<mcu::PA9, mcu::PinMode::USART1_TX>();
   BOOST_CHECK_EQUAL (cmsis.CRL,
        0
   );
   BOOST_CHECK_EQUAL (cmsis.CRH,
        GPIO_CRH_CNF9_1
      | GPIO_CRH_MODE9_1
   );
   BOOST_CHECK_EQUAL (process.str(),
      "включение тактирования AFIO\n"
   );
   clear_stream();

   gpio.init<mcu::PB6, mcu::PinMode::USART1_TX>();
   BOOST_CHECK_EQUAL (cmsis.CRL,
        GPIO_CRL_CNF6_1
      | GPIO_CRL_MODE6_1
   );
   BOOST_CHECK_EQUAL (cmsis.CRH,
        GPIO_CRH_CNF9_1
      | GPIO_CRH_MODE9_1
   );
   BOOST_CHECK_EQUAL (process.str(),
      "включение тактирования AFIO\n"
      "ремапинг USART1\n"
   );
   clear_stream();

   auto test1 = [&]() { gpio.init<mcu::PB7, mcu::PinMode::USART1_TX>(); };
   STATIC_ASSERTATION_REQUIRED ( test1(), "USART1_TX возможно только с PA9 или PB6");

   gpio.init<mcu::PA10, mcu::PinMode::USART1_RX>();
   BOOST_CHECK_EQUAL (cmsis.CRL,
        GPIO_CRL_CNF6_1
      | GPIO_CRL_MODE6_1
   );
   BOOST_CHECK_EQUAL (cmsis.CRH,
        GPIO_CRH_CNF9_1
      | GPIO_CRH_MODE9_1
      | GPIO_CRH_CNF10_1
      | GPIO_CRH_MODE10_1
   );
   BOOST_CHECK_EQUAL (process.str(),
      "включение тактирования AFIO\n"
   );
   clear_stream();

   gpio.init<mcu::PB7, mcu::PinMode::USART1_RX>();
   BOOST_CHECK_EQUAL (cmsis.CRL,
        GPIO_CRL_CNF6_1
      | GPIO_CRL_MODE6_1
      | GPIO_CRL_CNF7_1
      | GPIO_CRL_MODE7_1
   );
   BOOST_CHECK_EQUAL (cmsis.CRH,
        GPIO_CRH_CNF9_1
      | GPIO_CRH_MODE9_1
      | GPIO_CRH_CNF10_1
      | GPIO_CRH_MODE10_1
   );
   BOOST_CHECK_EQUAL (process.str(),
      "включение тактирования AFIO\n"
      "ремапинг USART1\n"
   );
   clear_stream();

   auto test2 = [&]() { gpio.init<mcu::PB6, mcu::PinMode::USART1_RX>(); };
   STATIC_ASSERTATION_REQUIRED ( test2(), "USART1_RX возможно только с PA10 и PB7");
}

BOOST_AUTO_TEST_CASE (init_alternate_usart2)
{
   prepare_test();

   gpio.init<mcu::PA2, mcu::PinMode::USART2_TX>();
   BOOST_CHECK_EQUAL (cmsis.CRL,
        GPIO_CRL_CNF2_1
      | GPIO_CRL_MODE2_1
   );
   BOOST_CHECK_EQUAL (cmsis.CRH,
        0
   );
   BOOST_CHECK_EQUAL (process.str(),
      "включение тактирования AFIO\n"
   );
   clear_stream();

   gpio.init<mcu::PD5, mcu::PinMode::USART2_TX>();
   BOOST_CHECK_EQUAL (cmsis.CRL,
        GPIO_CRL_CNF2_1
      | GPIO_CRL_MODE2_1
      | GPIO_CRL_CNF5_1
      | GPIO_CRL_MODE5_1
   );
   BOOST_CHECK_EQUAL (cmsis.CRH,
        0
   );
   BOOST_CHECK_EQUAL (process.str(),
      "включение тактирования AFIO\n"
      "ремапинг USART2\n"
   );
   clear_stream();

   auto test1 = [&]() { gpio.init<mcu::PD6, mcu::PinMode::USART2_TX>(); };
   STATIC_ASSERTATION_REQUIRED ( test1(), "USART2_TX возможно только с PA2 или PD5");

   gpio.init<mcu::PA3, mcu::PinMode::USART2_RX>();
   BOOST_CHECK_EQUAL (cmsis.CRL,
        GPIO_CRL_CNF2_1
      | GPIO_CRL_MODE2_1
      | GPIO_CRL_CNF3_1
      | GPIO_CRL_MODE3_1
      | GPIO_CRL_CNF5_1
      | GPIO_CRL_MODE5_1
   );
   BOOST_CHECK_EQUAL (cmsis.CRH,
        0
   );
   BOOST_CHECK_EQUAL (process.str(),
      "включение тактирования AFIO\n"
   );
   clear_stream();

   gpio.init<mcu::PD6, mcu::PinMode::USART2_RX>();
   BOOST_CHECK_EQUAL (cmsis.CRL,
        GPIO_CRL_CNF2_1
      | GPIO_CRL_MODE2_1
      | GPIO_CRL_CNF3_1
      | GPIO_CRL_MODE3_1
      | GPIO_CRL_CNF5_1
      | GPIO_CRL_MODE5_1
      | GPIO_CRL_CNF6_1
      | GPIO_CRL_MODE6_1
   );
   BOOST_CHECK_EQUAL (cmsis.CRH,
        0
   );
   BOOST_CHECK_EQUAL (process.str(),
      "включение тактирования AFIO\n"
      "ремапинг USART2\n"
   );
   clear_stream();

   auto test2 = [&]() { gpio.init<mcu::PD5, mcu::PinMode::USART2_RX>(); };
   STATIC_ASSERTATION_REQUIRED ( test2(), "USART2_RX возможно только с PA3 или PD6");
}

BOOST_AUTO_TEST_CASE (init_alternate_usart3)
{
   prepare_test();

   gpio.init<mcu::PB10, mcu::PinMode::USART3_TX>();
   BOOST_CHECK_EQUAL (cmsis.CRL,
        0
   );
   BOOST_CHECK_EQUAL (cmsis.CRH,
        GPIO_CRH_CNF10_1
      | GPIO_CRH_MODE10_1
   );
   BOOST_CHECK_EQUAL (process.str(),
      "включение тактирования AFIO\n"
   );
   clear_stream();

   gpio.init<mcu::PC10, mcu::PinMode::USART3_TX>();
   BOOST_CHECK_EQUAL (cmsis.CRL,
        0
   );
   BOOST_CHECK_EQUAL (cmsis.CRH,
        GPIO_CRH_CNF10_1
      | GPIO_CRH_MODE10_1
   );
   BOOST_CHECK_EQUAL (process.str(),
      "включение тактирования AFIO\n"
      "частичный ремапинг USART3\n"
   );
   clear_stream();

   gpio.init<mcu::PD8, mcu::PinMode::USART3_TX>();
   BOOST_CHECK_EQUAL (cmsis.CRL,
        0
   );
   BOOST_CHECK_EQUAL (cmsis.CRH,
        GPIO_CRH_CNF8_1
      | GPIO_CRH_MODE8_1
      | GPIO_CRH_CNF10_1
      | GPIO_CRH_MODE10_1
   );
   BOOST_CHECK_EQUAL (process.str(),
      "включение тактирования AFIO\n"
      "полный ремапинг USART3\n"
   );
   clear_stream();

   auto test1 = [&]() { gpio.init<mcu::PA10, mcu::PinMode::USART3_TX>(); };
   STATIC_ASSERTATION_REQUIRED ( test1(), "USART3_TX возможно только с PB10, PC10 или PD8");

   gpio.init<mcu::PB11, mcu::PinMode::USART3_RX>();
   BOOST_CHECK_EQUAL (cmsis.CRL,
        0
   );
   BOOST_CHECK_EQUAL (cmsis.CRH,
        GPIO_CRH_CNF8_1
      | GPIO_CRH_MODE8_1
      | GPIO_CRH_CNF10_1
      | GPIO_CRH_MODE10_1
      | GPIO_CRH_CNF11_1
      | GPIO_CRH_MODE11_1
   );
   BOOST_CHECK_EQUAL (process.str(),
      "включение тактирования AFIO\n"
   );
   clear_stream();

   gpio.init<mcu::PC11, mcu::PinMode::USART3_RX>();
   BOOST_CHECK_EQUAL (cmsis.CRL,
        0
   );
   BOOST_CHECK_EQUAL (cmsis.CRH,
        GPIO_CRH_CNF8_1
      | GPIO_CRH_MODE8_1
      | GPIO_CRH_CNF10_1
      | GPIO_CRH_MODE10_1
      | GPIO_CRH_CNF11_1
      | GPIO_CRH_MODE11_1
   );
   BOOST_CHECK_EQUAL (process.str(),
      "включение тактирования AFIO\n"
      "частичный ремапинг USART3\n"
   );
   clear_stream();

   gpio.init<mcu::PD9, mcu::PinMode::USART3_RX>();
   BOOST_CHECK_EQUAL (cmsis.CRL,
        0
   );
   BOOST_CHECK_EQUAL (cmsis.CRH,
        GPIO_CRH_CNF8_1
      | GPIO_CRH_MODE8_1
      | GPIO_CRH_CNF9_1
      | GPIO_CRH_MODE9_1
      | GPIO_CRH_CNF10_1
      | GPIO_CRH_MODE10_1
      | GPIO_CRH_CNF11_1
      | GPIO_CRH_MODE11_1
   );
   BOOST_CHECK_EQUAL (process.str(),
      "включение тактирования AFIO\n"
      "полный ремапинг USART3\n"
   );
   clear_stream();

   auto test2 = [&]() { gpio.init<mcu::PA9, mcu::PinMode::USART3_RX>(); };
   STATIC_ASSERTATION_REQUIRED ( test2(), "USART3_RX возможно только с PB11, PC11 или PD9");
}

BOOST_AUTO_TEST_CASE (init_jtag_pins)
{
   prepare_test();
   cmsis.CRL = 0;
   cmsis.CRH = 0;

   gpio.init<mcu::PA14, mcu::PinMode::Input>();
   BOOST_CHECK_EQUAL (process.str(),
      "включение тактирования AFIO\n"
      "выключение JTAG\n"
   );
   BOOST_CHECK_EQUAL (cmsis.CRL,
      0
   );
   BOOST_CHECK_EQUAL (cmsis.CRH,
      GPIO_CRH_CNF14_0
   );
   clear_stream();

   gpio.init<mcu::PA15, mcu::PinMode::Output>();
   BOOST_CHECK_EQUAL (process.str(),
      "включение тактирования AFIO\n"
      "выключение JTAG\n"
   );
   BOOST_CHECK_EQUAL (cmsis.CRL,
      0
   );
   BOOST_CHECK_EQUAL (cmsis.CRH,
        GPIO_CRH_CNF14_0
      | GPIO_CRH_MODE15_1
   );
   clear_stream();

   gpio.init<mcu::PB3, mcu::PinMode::Output>();
   BOOST_CHECK_EQUAL (process.str(),
      "включение тактирования AFIO\n"
      "выключение JTAG\n"
   );
   BOOST_CHECK_EQUAL (cmsis.CRL,
        GPIO_CRL_MODE3_1
   );
   BOOST_CHECK_EQUAL (cmsis.CRH,
        GPIO_CRH_CNF14_0
      | GPIO_CRH_MODE15_1
   );
   clear_stream();

   gpio.init<mcu::PB4, mcu::PinMode::Input>();
   BOOST_CHECK_EQUAL (process.str(),
      "включение тактирования AFIO\n"
      "выключение JTAG\n"
   );
   BOOST_CHECK_EQUAL (cmsis.CRL,
        GPIO_CRL_MODE3_1
      | GPIO_CRL_CNF4_0
   );
   BOOST_CHECK_EQUAL (cmsis.CRH,
        GPIO_CRH_CNF14_0
      | GPIO_CRH_MODE15_1
   );
   clear_stream();
}
#endif // #if defined(STM32F1)

BOOST_AUTO_TEST_SUITE_END()
