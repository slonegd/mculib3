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

BOOST_AUTO_TEST_SUITE_END()
