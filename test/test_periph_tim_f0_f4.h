BOOST_AUTO_TEST_SUITE (test_suite_main)

mcu::TIM tim {};
auto& CMSIS = tim.like_CMSIS();

using mcu::TIM;

BOOST_AUTO_TEST_CASE (like_CMSIS)
{
   auto same = std::is_same_v<std::remove_reference_t<decltype(CMSIS)>, TIM_TypeDef>;
   auto address_tim = reinterpret_cast<size_t>(&tim);
   auto address_CMSIS = reinterpret_cast<size_t>(&CMSIS);
   BOOST_CHECK_EQUAL (address_tim, address_CMSIS);
   BOOST_CHECK_EQUAL (same, true);
}

BOOST_AUTO_TEST_CASE (counter_enable_disable)
{
   CMSIS.CR1 = 0;
   tim.counter_enable();
   BOOST_CHECK_EQUAL (CMSIS.CR1, TIM_CR1_CEN_Msk);
   tim.counter_disable();
   BOOST_CHECK_EQUAL (CMSIS.CR1, not TIM_CR1_CEN_Msk);
}

BOOST_AUTO_TEST_CASE (set_clear_counter)
{
   CMSIS.CNT = 0;
   tim.set_counter(0xFFFF);
   BOOST_CHECK_EQUAL (CMSIS.CNT, 0xFFFF);

   tim.clear_counter();
   BOOST_CHECK_EQUAL (CMSIS.CNT, 0);
   tim.set_counter(256);
   BOOST_CHECK_EQUAL (CMSIS.CNT, 256);
}

BOOST_AUTO_TEST_CASE (ext_clock_enable_disable)
{
   CMSIS.SMCR = 0;
   tim.ext_clock_enable();
   BOOST_CHECK_EQUAL (CMSIS.SMCR, TIM_SMCR_ECE_Msk);

   tim.ext_clock_disable();
   BOOST_CHECK_EQUAL (CMSIS.SMCR, not TIM_SMCR_ECE_Msk);
}

BOOST_AUTO_TEST_CASE (set_prescaller)
{
   CMSIS.PSC = 0;
   tim.set_prescaller(0xFFFF);
   BOOST_CHECK_EQUAL (CMSIS.PSC, 0xFFFF);

   tim.set_prescaller(0);
   BOOST_CHECK_EQUAL (CMSIS.PSC, 0);

   tim.set_prescaller(4585);
   BOOST_CHECK_EQUAL (CMSIS.PSC, 4585);
}

BOOST_AUTO_TEST_CASE (auto_reload_enable)
{
   CMSIS.CR1 = 0;
   tim.auto_reload_enable();
   BOOST_CHECK_EQUAL (CMSIS.CR1, TIM_CR1_ARPE_Msk);
}

BOOST_AUTO_TEST_CASE (set_auto_reload)
{
   CMSIS.ARR = 0;
   tim.set_auto_reload(0xFFFF);
   BOOST_CHECK_EQUAL (CMSIS.ARR, 0xFFFF);

   tim.set_auto_reload(0);
   BOOST_CHECK_EQUAL (CMSIS.ARR, 0);

   tim.set_auto_reload(12569);
   BOOST_CHECK_EQUAL (CMSIS.ARR, 12569);
}

BOOST_AUTO_TEST_CASE (set_SlaveMode)
{
   CMSIS.SMCR = 0;
   tim.set(TIM::SlaveMode::ExternalClock);
   BOOST_CHECK_EQUAL (CMSIS.SMCR, TIM_SMCR_SMS_Msk);

   tim.set(TIM::SlaveMode::TriggerMode);
   BOOST_CHECK_EQUAL (CMSIS.SMCR, not TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1 | TIM_SMCR_SMS_2);

   tim.set(TIM::SlaveMode::Gated);
   BOOST_CHECK_EQUAL (CMSIS.SMCR, TIM_SMCR_SMS_0 | not TIM_SMCR_SMS_1 | TIM_SMCR_SMS_2);

   tim.set(TIM::SlaveMode::Reset);
   BOOST_CHECK_EQUAL (CMSIS.SMCR, not TIM_SMCR_SMS_0 | not TIM_SMCR_SMS_1 | TIM_SMCR_SMS_2);

   tim.set(TIM::SlaveMode::Encoder3);
   BOOST_CHECK_EQUAL (CMSIS.SMCR, TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1 | not TIM_SMCR_SMS_2);

   tim.set(TIM::SlaveMode::Encoder2);
   BOOST_CHECK_EQUAL (CMSIS.SMCR, not TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1 | not TIM_SMCR_SMS_2);

   tim.set(TIM::SlaveMode::Encoder1);
   BOOST_CHECK_EQUAL (CMSIS.SMCR, TIM_SMCR_SMS_0 | not TIM_SMCR_SMS_1 | not TIM_SMCR_SMS_2);

   tim.set(TIM::SlaveMode::SMDisabled);
   BOOST_CHECK_EQUAL (CMSIS.SMCR, not TIM_SMCR_SMS_Msk);
}

BOOST_AUTO_TEST_CASE (compare_enable_disable)
{
   CMSIS.CCER = 0;
   tim.compare_enable(TIM::EnableMask::ch1);
   BOOST_CHECK_EQUAL (CMSIS.CCER, TIM_CCER_CC1E_Msk);

   tim.compare_disable(TIM::EnableMask::ch1);
   BOOST_CHECK_EQUAL (CMSIS.CCER, not TIM_CCER_CC1E_Msk);

   tim.compare_enable(TIM::EnableMask::ch2);
   BOOST_CHECK_EQUAL (CMSIS.CCER, TIM_CCER_CC2E_Msk);

   tim.compare_disable(TIM::EnableMask::ch2);
   BOOST_CHECK_EQUAL (CMSIS.CCER, not TIM_CCER_CC2E_Msk);

   tim.compare_enable(TIM::EnableMask::ch3);
   BOOST_CHECK_EQUAL (CMSIS.CCER, TIM_CCER_CC3E_Msk);

   tim.compare_disable(TIM::EnableMask::ch3);
   BOOST_CHECK_EQUAL (CMSIS.CCER, not TIM_CCER_CC3E_Msk);

   tim.compare_enable(TIM::EnableMask::ch4);
   BOOST_CHECK_EQUAL (CMSIS.CCER, TIM_CCER_CC4E_Msk);

   tim.compare_disable(TIM::EnableMask::ch4);
   BOOST_CHECK_EQUAL (CMSIS.CCER, not TIM_CCER_CC4E_Msk);
}

BOOST_AUTO_TEST_CASE (set_Polarity)
{
   CMSIS.CCER = 0;
   tim.set<TIM::Channel::_1>(TIM::Polarity::both);
   BOOST_CHECK_EQUAL(CMSIS.CCER, TIM_CCER_CC1P_Msk | TIM_CCER_CC1NP_Msk);

   CMSIS.CCER = 0;
   tim.set<TIM::Channel::_1>(TIM::Polarity::falling);
   BOOST_CHECK_EQUAL(CMSIS.CCER, TIM_CCER_CC1P_Msk | not TIM_CCER_CC1NP_Msk);

   CMSIS.CCER = 0;
   tim.set<TIM::Channel::_1>(TIM::Polarity::rising);
   BOOST_CHECK_EQUAL(CMSIS.CCER, not TIM_CCER_CC1P_Msk | not TIM_CCER_CC1NP_Msk);

   CMSIS.CCER = 0;
   tim.set<TIM::Channel::_2>(TIM::Polarity::both);
   BOOST_CHECK_EQUAL(CMSIS.CCER, TIM_CCER_CC2P_Msk | TIM_CCER_CC2NP_Msk);

   CMSIS.CCER = 0;
   tim.set<TIM::Channel::_2>(TIM::Polarity::falling);
   BOOST_CHECK_EQUAL(CMSIS.CCER, TIM_CCER_CC2P_Msk | not TIM_CCER_CC2NP_Msk);

   CMSIS.CCER = 0;
   tim.set<TIM::Channel::_2>(TIM::Polarity::rising);
   BOOST_CHECK_EQUAL(CMSIS.CCER, not TIM_CCER_CC2P_Msk | not TIM_CCER_CC2NP_Msk);

   CMSIS.CCER = 0;
   tim.set<TIM::Channel::_3>(TIM::Polarity::both);
   BOOST_CHECK_EQUAL(CMSIS.CCER, TIM_CCER_CC3P_Msk | TIM_CCER_CC3NP_Msk);

   CMSIS.CCER = 0;
   tim.set<TIM::Channel::_3>(TIM::Polarity::falling);
   BOOST_CHECK_EQUAL(CMSIS.CCER, TIM_CCER_CC3P_Msk | not TIM_CCER_CC3NP_Msk);

   CMSIS.CCER = 0;
   tim.set<TIM::Channel::_3>(TIM::Polarity::rising);
   BOOST_CHECK_EQUAL(CMSIS.CCER, not TIM_CCER_CC3P_Msk | not TIM_CCER_CC3NP_Msk);

   CMSIS.CCER = 0;
   tim.set<TIM::Channel::_4>(TIM::Polarity::both);
   BOOST_CHECK_EQUAL(CMSIS.CCER, TIM_CCER_CC4P_Msk | TIM_CCER_CC4NP_Msk);

   CMSIS.CCER = 0;
   tim.set<TIM::Channel::_4>(TIM::Polarity::falling);
   BOOST_CHECK_EQUAL(CMSIS.CCER, TIM_CCER_CC4P_Msk | not TIM_CCER_CC4NP_Msk);

   CMSIS.CCER = 0;
   tim.set<TIM::Channel::_4>(TIM::Polarity::rising);
   BOOST_CHECK_EQUAL(CMSIS.CCER, not TIM_CCER_CC4P_Msk | not TIM_CCER_CC4NP_Msk);
}

BOOST_AUTO_TEST_CASE (set_CompareMode)
{
   // channel 1
   CMSIS.CCMR1 = 0;
   tim.set<TIM::Channel::_1>(TIM::CompareMode::InvertedPWMmode);
   BOOST_CHECK_EQUAL (CMSIS.CCMR1, TIM_CCMR1_OC1M_Msk);

   CMSIS.CCMR1 = 0;
   tim.set<TIM::Channel::_1>(TIM::CompareMode::PWMmode);
   BOOST_CHECK_EQUAL (CMSIS.CCMR1, TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | not TIM_CCMR1_OC1M_0);

   CMSIS.CCMR1 = 0;
   tim.set<TIM::Channel::_1>(TIM::CompareMode::ForceActive);
   BOOST_CHECK_EQUAL (CMSIS.CCMR1, TIM_CCMR1_OC1M_2 | not TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_0);

   CMSIS.CCMR1 = 0;
   tim.set<TIM::Channel::_1>(TIM::CompareMode::ForceInactive);
   BOOST_CHECK_EQUAL (CMSIS.CCMR1, TIM_CCMR1_OC1M_2 | not TIM_CCMR1_OC1M_1 | not TIM_CCMR1_OC1M_0);

   CMSIS.CCMR1 = 0;
   tim.set<TIM::Channel::_1>(TIM::CompareMode::ToggleOnMatch);
   BOOST_CHECK_EQUAL (CMSIS.CCMR1, not TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_0);

   CMSIS.CCMR1 = 0;
   tim.set<TIM::Channel::_1>(TIM::CompareMode::InactiveOnMatch);
   BOOST_CHECK_EQUAL (CMSIS.CCMR1, not TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | not TIM_CCMR1_OC1M_0);

   CMSIS.CCMR1 = 0;
   tim.set<TIM::Channel::_1>(TIM::CompareMode::ActiveOnMatch);
   BOOST_CHECK_EQUAL (CMSIS.CCMR1, not TIM_CCMR1_OC1M_2 | not TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_0);

   CMSIS.CCMR1 = 0;
   tim.set<TIM::Channel::_1>(TIM::CompareMode::Off);
   BOOST_CHECK_EQUAL (CMSIS.CCMR1, not TIM_CCMR1_OC1M_Msk);

   // channel 2
   CMSIS.CCMR1 = 0;
   tim.set<TIM::Channel::_2>(TIM::CompareMode::InvertedPWMmode);
   BOOST_CHECK_EQUAL (CMSIS.CCMR1, TIM_CCMR1_OC2M_Msk);

   CMSIS.CCMR1 = 0;
   tim.set<TIM::Channel::_2>(TIM::CompareMode::PWMmode);
   BOOST_CHECK_EQUAL (CMSIS.CCMR1, TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 | not TIM_CCMR1_OC2M_0);

   CMSIS.CCMR1 = 0;
   tim.set<TIM::Channel::_2>(TIM::CompareMode::ForceActive);
   BOOST_CHECK_EQUAL (CMSIS.CCMR1, TIM_CCMR1_OC2M_2 | not TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_0);

   CMSIS.CCMR1 = 0;
   tim.set<TIM::Channel::_2>(TIM::CompareMode::ForceInactive);
   BOOST_CHECK_EQUAL (CMSIS.CCMR1, TIM_CCMR1_OC2M_2 | not TIM_CCMR1_OC2M_1 | not TIM_CCMR1_OC2M_0);

   CMSIS.CCMR1 = 0;
   tim.set<TIM::Channel::_2>(TIM::CompareMode::ToggleOnMatch);
   BOOST_CHECK_EQUAL (CMSIS.CCMR1, not TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_0);

   CMSIS.CCMR1 = 0;
   tim.set<TIM::Channel::_2>(TIM::CompareMode::InactiveOnMatch);
   BOOST_CHECK_EQUAL (CMSIS.CCMR1, not TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 | not TIM_CCMR1_OC2M_0);

   CMSIS.CCMR1 = 0;
   tim.set<TIM::Channel::_2>(TIM::CompareMode::ActiveOnMatch);
   BOOST_CHECK_EQUAL (CMSIS.CCMR1, not TIM_CCMR1_OC2M_2 | not TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_0);

   CMSIS.CCMR1 = 0;
   tim.set<TIM::Channel::_2>(TIM::CompareMode::Off);
   BOOST_CHECK_EQUAL (CMSIS.CCMR1, not TIM_CCMR1_OC2M_Msk);

   // channel 3
   CMSIS.CCMR2 = 0;
   tim.set<TIM::Channel::_3>(TIM::CompareMode::InvertedPWMmode);
   BOOST_CHECK_EQUAL (CMSIS.CCMR2, TIM_CCMR2_OC3M_Msk);

   CMSIS.CCMR2 = 0;
   tim.set<TIM::Channel::_3>(TIM::CompareMode::PWMmode);
   BOOST_CHECK_EQUAL (CMSIS.CCMR2, TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 | not TIM_CCMR2_OC3M_0);

   CMSIS.CCMR2 = 0;
   tim.set<TIM::Channel::_3>(TIM::CompareMode::ForceActive);
   BOOST_CHECK_EQUAL (CMSIS.CCMR2, TIM_CCMR2_OC3M_2 | not TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_0);

   CMSIS.CCMR2 = 0;
   tim.set<TIM::Channel::_3>(TIM::CompareMode::ForceInactive);
   BOOST_CHECK_EQUAL (CMSIS.CCMR2, TIM_CCMR2_OC3M_2 | not TIM_CCMR2_OC3M_1 | not TIM_CCMR2_OC3M_0);

   CMSIS.CCMR2 = 0;
   tim.set<TIM::Channel::_3>(TIM::CompareMode::ToggleOnMatch);
   BOOST_CHECK_EQUAL (CMSIS.CCMR2, not TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_0);

   CMSIS.CCMR2 = 0;
   tim.set<TIM::Channel::_3>(TIM::CompareMode::InactiveOnMatch);
   BOOST_CHECK_EQUAL (CMSIS.CCMR2, not TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 | not TIM_CCMR2_OC3M_0);

   CMSIS.CCMR2 = 0;
   tim.set<TIM::Channel::_3>(TIM::CompareMode::ActiveOnMatch);
   BOOST_CHECK_EQUAL (CMSIS.CCMR2, not TIM_CCMR2_OC3M_2 | not TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_0);

   CMSIS.CCMR2 = 0;
   tim.set<TIM::Channel::_3>(TIM::CompareMode::Off);
   BOOST_CHECK_EQUAL (CMSIS.CCMR2, not TIM_CCMR2_OC3M_Msk);

   // channel 4
   CMSIS.CCMR2 = 0;
   tim.set<TIM::Channel::_4>(TIM::CompareMode::InvertedPWMmode);
   BOOST_CHECK_EQUAL (CMSIS.CCMR2, TIM_CCMR2_OC4M_Msk);

   CMSIS.CCMR2 = 0;
   tim.set<TIM::Channel::_4>(TIM::CompareMode::PWMmode);
   BOOST_CHECK_EQUAL (CMSIS.CCMR2, TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1 | not TIM_CCMR2_OC4M_0);

   CMSIS.CCMR2 = 0;
   tim.set<TIM::Channel::_4>(TIM::CompareMode::ForceActive);
   BOOST_CHECK_EQUAL (CMSIS.CCMR2, TIM_CCMR2_OC4M_2 | not TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_0);

   CMSIS.CCMR2 = 0;
   tim.set<TIM::Channel::_4>(TIM::CompareMode::ForceInactive);
   BOOST_CHECK_EQUAL (CMSIS.CCMR2, TIM_CCMR2_OC4M_2 | not TIM_CCMR2_OC4M_1 | not TIM_CCMR2_OC4M_0);

   CMSIS.CCMR2 = 0;
   tim.set<TIM::Channel::_4>(TIM::CompareMode::ToggleOnMatch);
   BOOST_CHECK_EQUAL (CMSIS.CCMR2, not TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_0);

   CMSIS.CCMR2 = 0;
   tim.set<TIM::Channel::_4>(TIM::CompareMode::InactiveOnMatch);
   BOOST_CHECK_EQUAL (CMSIS.CCMR2, not TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1 | not TIM_CCMR2_OC4M_0);

   CMSIS.CCMR2 = 0;
   tim.set<TIM::Channel::_4>(TIM::CompareMode::ActiveOnMatch);
   BOOST_CHECK_EQUAL (CMSIS.CCMR2, not TIM_CCMR2_OC4M_2 | not TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_0);

   CMSIS.CCMR2 = 0;
   tim.set<TIM::Channel::_4>(TIM::CompareMode::Off);
   BOOST_CHECK_EQUAL (CMSIS.CCMR2, not TIM_CCMR2_OC4M_Msk);
}

BOOST_AUTO_TEST_CASE (SelectionCompareMode)
{
   //channel 1
   CMSIS.CCMR1 = 0;
   tim.set<TIM::Channel::_1>(TIM::SelectionCompareMode::InputTRC);
   BOOST_CHECK_EQUAL (CMSIS.CCMR1, TIM_CCMR1_CC1S_Msk);

   CMSIS.CCMR1 = 0;
   tim.set<TIM::Channel::_1>(TIM::SelectionCompareMode::InputALT);
   BOOST_CHECK_EQUAL (CMSIS.CCMR1, TIM_CCMR1_CC1S_1 | not TIM_CCMR1_CC1S_0);

   CMSIS.CCMR1 = 0;
   tim.set<TIM::Channel::_1>(TIM::SelectionCompareMode::Input);
   BOOST_CHECK_EQUAL (CMSIS.CCMR1, not TIM_CCMR1_CC1S_1 | TIM_CCMR1_CC1S_0);

   CMSIS.CCMR1 = 0;
   tim.set<TIM::Channel::_1>(TIM::SelectionCompareMode::Output);
   BOOST_CHECK_EQUAL (CMSIS.CCMR1, not TIM_CCMR1_CC1S_Msk);

   //channel 2
   CMSIS.CCMR1 = 0;
   tim.set<TIM::Channel::_2>(TIM::SelectionCompareMode::InputTRC);
   BOOST_CHECK_EQUAL (CMSIS.CCMR1, TIM_CCMR1_CC2S_Msk);

   CMSIS.CCMR1 = 0;
   tim.set<TIM::Channel::_2>(TIM::SelectionCompareMode::InputALT);
   BOOST_CHECK_EQUAL (CMSIS.CCMR1, TIM_CCMR1_CC2S_1 | not TIM_CCMR1_CC2S_0);

   CMSIS.CCMR1 = 0;
   tim.set<TIM::Channel::_2>(TIM::SelectionCompareMode::Input);
   BOOST_CHECK_EQUAL (CMSIS.CCMR1, not TIM_CCMR1_CC2S_1 | TIM_CCMR1_CC2S_0);

   CMSIS.CCMR1 = 0;
   tim.set<TIM::Channel::_2>(TIM::SelectionCompareMode::Output);
   BOOST_CHECK_EQUAL (CMSIS.CCMR1, not TIM_CCMR1_CC2S_Msk);

   //channel 3
   CMSIS.CCMR2 = 0;
   tim.set<TIM::Channel::_3>(TIM::SelectionCompareMode::InputTRC);
   BOOST_CHECK_EQUAL (CMSIS.CCMR2, TIM_CCMR2_CC3S_Msk);

   CMSIS.CCMR2 = 0;
   tim.set<TIM::Channel::_3>(TIM::SelectionCompareMode::InputALT);
   BOOST_CHECK_EQUAL (CMSIS.CCMR2, TIM_CCMR2_CC3S_1 | not TIM_CCMR2_CC3S_0);

   CMSIS.CCMR2 = 0;
   tim.set<TIM::Channel::_3>(TIM::SelectionCompareMode::Input);
   BOOST_CHECK_EQUAL (CMSIS.CCMR2, not TIM_CCMR2_CC3S_1 | TIM_CCMR2_CC3S_0);

   CMSIS.CCMR2 = 0;
   tim.set<TIM::Channel::_3>(TIM::SelectionCompareMode::Output);
   BOOST_CHECK_EQUAL (CMSIS.CCMR2, not TIM_CCMR2_CC3S_Msk);

   //channel 4
   CMSIS.CCMR2 = 0;
   tim.set<TIM::Channel::_4>(TIM::SelectionCompareMode::InputTRC);
   BOOST_CHECK_EQUAL (CMSIS.CCMR2, TIM_CCMR2_CC4S_Msk);

   CMSIS.CCMR2 = 0;
   tim.set<TIM::Channel::_4>(TIM::SelectionCompareMode::InputALT);
   BOOST_CHECK_EQUAL (CMSIS.CCMR2, TIM_CCMR2_CC4S_1 | not TIM_CCMR2_CC4S_0);

   CMSIS.CCMR2 = 0;
   tim.set<TIM::Channel::_4>(TIM::SelectionCompareMode::Input);
   BOOST_CHECK_EQUAL (CMSIS.CCMR2, not TIM_CCMR2_CC4S_1 | TIM_CCMR2_CC4S_0);

   CMSIS.CCMR2 = 0;
   tim.set<TIM::Channel::_4>(TIM::SelectionCompareMode::Output);
   BOOST_CHECK_EQUAL (CMSIS.CCMR2, not TIM_CCMR2_CC4S_Msk);
}

BOOST_AUTO_TEST_CASE (preload_enable_disable)
{
   CMSIS.CCMR1 = 0;
   tim.preload_enable<TIM::Channel::_1>();
   BOOST_CHECK_EQUAL(CMSIS.CCMR1, TIM_CCMR1_OC1PE_Msk);

   tim.preload_disable<TIM::Channel::_1>();
   BOOST_CHECK_EQUAL(CMSIS.CCMR1, not TIM_CCMR1_OC1PE_Msk);

   tim.preload_enable<TIM::Channel::_2>();
   BOOST_CHECK_EQUAL(CMSIS.CCMR1, TIM_CCMR1_OC2PE_Msk);

   tim.preload_disable<TIM::Channel::_2>();
   BOOST_CHECK_EQUAL(CMSIS.CCMR1, not TIM_CCMR1_OC2PE_Msk);

   tim.preload_enable<TIM::Channel::_3>();
   BOOST_CHECK_EQUAL(CMSIS.CCMR2, TIM_CCMR2_OC3PE_Msk);

   tim.preload_disable<TIM::Channel::_3>();
   BOOST_CHECK_EQUAL(CMSIS.CCMR2, not TIM_CCMR2_OC3PE_Msk);

   tim.preload_enable<TIM::Channel::_4>();
   BOOST_CHECK_EQUAL(CMSIS.CCMR2, TIM_CCMR2_OC4PE_Msk);

   tim.preload_disable<TIM::Channel::_4>();
   BOOST_CHECK_EQUAL(CMSIS.CCMR2, not TIM_CCMR2_OC4PE_Msk);
}

BOOST_AUTO_TEST_CASE (compare_enable_disable_)
{
   CMSIS.CCER = 0;
   tim.compare_enable<TIM::Channel::_1>();
   BOOST_CHECK_EQUAL (CMSIS.CCER, TIM_CCER_CC1E_Msk);

   tim.compare_disable<TIM::Channel::_1>();
   BOOST_CHECK_EQUAL (CMSIS.CCER, not TIM_CCER_CC1E_Msk);

   tim.compare_enable<TIM::Channel::_2>();
   BOOST_CHECK_EQUAL (CMSIS.CCER, TIM_CCER_CC2E_Msk);

   tim.compare_disable<TIM::Channel::_2>();
   BOOST_CHECK_EQUAL (CMSIS.CCER, not TIM_CCER_CC2E_Msk);

   tim.compare_enable<TIM::Channel::_3>();
   BOOST_CHECK_EQUAL (CMSIS.CCER, TIM_CCER_CC3E_Msk);

   tim.compare_disable<TIM::Channel::_3>();
   BOOST_CHECK_EQUAL (CMSIS.CCER, not TIM_CCER_CC3E_Msk);

   tim.compare_enable<TIM::Channel::_4>();
   BOOST_CHECK_EQUAL (CMSIS.CCER, TIM_CCER_CC4E_Msk);

   tim.compare_disable<TIM::Channel::_4>();
   BOOST_CHECK_EQUAL (CMSIS.CCER, not TIM_CCER_CC4E_Msk);
}

BOOST_AUTO_TEST_CASE (set_compare)
{
   // channel 1
   CMSIS.CCR1 = 0;
   tim.set_compare(TIM::Channel::_1, 0xFFFF);
   BOOST_CHECK_EQUAL (CMSIS.CCR1, 0xFFFF);

   tim.set_compare(TIM::Channel::_1, 0);
   BOOST_CHECK_EQUAL (CMSIS.CCR1, 0);

   tim.set_compare(TIM::Channel::_1, 147);
   BOOST_CHECK_EQUAL (CMSIS.CCR1, 147);

   // channel 2
   CMSIS.CCR2 = 0;
   tim.set_compare(TIM::Channel::_2, 0xFFFF);
   BOOST_CHECK_EQUAL (CMSIS.CCR2, 0xFFFF);

   tim.set_compare(TIM::Channel::_2, 0);
   BOOST_CHECK_EQUAL (CMSIS.CCR2, 0);

   tim.set_compare(TIM::Channel::_2, 147);
   BOOST_CHECK_EQUAL (CMSIS.CCR2, 147);

   // channel 3
   CMSIS.CCR3 = 0;
   tim.set_compare(TIM::Channel::_3, 0xFFFF);
   BOOST_CHECK_EQUAL (CMSIS.CCR3, 0xFFFF);

   tim.set_compare(TIM::Channel::_3, 0);
   BOOST_CHECK_EQUAL (CMSIS.CCR3, 0);

   tim.set_compare(TIM::Channel::_3, 147);
   BOOST_CHECK_EQUAL (CMSIS.CCR3, 147);

   // channel 4
   CMSIS.CCR4 = 0;
   tim.set_compare(TIM::Channel::_4, 0xFFFF);
   BOOST_CHECK_EQUAL (CMSIS.CCR4, 0xFFFF);

   tim.set_compare(TIM::Channel::_4, 0);
   BOOST_CHECK_EQUAL (CMSIS.CCR4, 0);

   tim.set_compare(TIM::Channel::_4, 147);
   BOOST_CHECK_EQUAL (CMSIS.CCR4, 147);
}

BOOST_AUTO_TEST_CASE (get_compare_reference)
{
   BOOST_CHECK_EQUAL(tim.get_compare_reference<TIM::Channel::_1>(), CMSIS.CCR1);

   BOOST_CHECK_EQUAL(tim.get_compare_reference<TIM::Channel::_2>(), CMSIS.CCR2);

   BOOST_CHECK_EQUAL(tim.get_compare_reference<TIM::Channel::_3>(), CMSIS.CCR3);

   BOOST_CHECK_EQUAL(tim.get_compare_reference<TIM::Channel::_4>(), CMSIS.CCR4);
}

BOOST_AUTO_TEST_CASE (pin_mode)
{
#if defined(STM32F4)
   // TIM2
   auto mode = tim.pin_mode<mcu::Periph::TIM2, mcu::PA0>();
   BOOST_CHECK(mode == mcu::PinMode::Alternate_1);
   
   mode = tim.pin_mode<mcu::Periph::TIM2, mcu::PA1>();
   BOOST_CHECK(mode == mcu::PinMode::Alternate_1);

   mode = tim.pin_mode<mcu::Periph::TIM2, mcu::PA2>();
   BOOST_CHECK(mode == mcu::PinMode::Alternate_1);

   mode = tim.pin_mode<mcu::Periph::TIM2, mcu::PA3>();
   BOOST_CHECK(mode == mcu::PinMode::Alternate_1);

   mode = tim.pin_mode<mcu::Periph::TIM2, mcu::PA15>();
   BOOST_CHECK(mode == mcu::PinMode::Alternate_1);

   mode = tim.pin_mode<mcu::Periph::TIM2, mcu::PB3>();
   BOOST_CHECK(mode == mcu::PinMode::Alternate_1);

   mode = tim.pin_mode<mcu::Periph::TIM2, mcu::PB10>();
   BOOST_CHECK(mode == mcu::PinMode::Alternate_1);

   mode = tim.pin_mode<mcu::Periph::TIM2, mcu::PB11>();
   BOOST_CHECK(mode == mcu::PinMode::Alternate_1);

   mode = tim.pin_mode<mcu::Periph::TIM2, mcu::PC6>();
   BOOST_CHECK(mode != mcu::PinMode::Alternate_1);
   BOOST_CHECK(mode == mcu::PinMode::Input);

   // TIM3
   mode = tim.pin_mode<mcu::Periph::TIM3, mcu::PA6>();
   BOOST_CHECK(mode == mcu::PinMode::Alternate_2);

   mode = tim.pin_mode<mcu::Periph::TIM3, mcu::PA7>();
   BOOST_CHECK(mode == mcu::PinMode::Alternate_2);

   mode = tim.pin_mode<mcu::Periph::TIM3, mcu::PB0>();
   BOOST_CHECK(mode == mcu::PinMode::Alternate_2);

   mode = tim.pin_mode<mcu::Periph::TIM3, mcu::PB1>();
   BOOST_CHECK(mode == mcu::PinMode::Alternate_2);

   mode = tim.pin_mode<mcu::Periph::TIM3, mcu::PB4>();
   BOOST_CHECK(mode == mcu::PinMode::Alternate_2);

   mode = tim.pin_mode<mcu::Periph::TIM3, mcu::PB5>();
   BOOST_CHECK(mode == mcu::PinMode::Alternate_2);

   mode = tim.pin_mode<mcu::Periph::TIM3, mcu::PC6>();
   BOOST_CHECK(mode == mcu::PinMode::Alternate_2);

   mode = tim.pin_mode<mcu::Periph::TIM3, mcu::PC7>();
   BOOST_CHECK(mode == mcu::PinMode::Alternate_2);

   mode = tim.pin_mode<mcu::Periph::TIM3, mcu::PC8>();
   BOOST_CHECK(mode == mcu::PinMode::Alternate_2);

   mode = tim.pin_mode<mcu::Periph::TIM3, mcu::PC9>();
   BOOST_CHECK(mode == mcu::PinMode::Alternate_2);

   mode = tim.pin_mode<mcu::Periph::TIM3, mcu::PD2>();
   BOOST_CHECK(mode == mcu::PinMode::Alternate_2);

   mode = tim.pin_mode<mcu::Periph::TIM3, mcu::PA1>();
   BOOST_CHECK(mode != mcu::PinMode::Alternate_2);
   BOOST_CHECK(mode == mcu::PinMode::Input);
#endif
}

BOOST_AUTO_TEST_CASE (channel)
{
#if defined(STM32F4)
   // TIM2
   auto channel = tim.channel<mcu::Periph::TIM2, mcu::PA0>();
   BOOST_CHECK(channel == TIM::Channel::_1);

   channel = tim.channel<mcu::Periph::TIM2, mcu::PA1>();
   BOOST_CHECK(channel == TIM::Channel::_2);

   channel = tim.channel<mcu::Periph::TIM2, mcu::PA2>();
   BOOST_CHECK(channel == TIM::Channel::_3);

   channel = tim.channel<mcu::Periph::TIM2, mcu::PA3>();
   BOOST_CHECK(channel == TIM::Channel::_4);

   channel = tim.channel<mcu::Periph::TIM2, mcu::PA5>();
   BOOST_CHECK(channel == TIM::Channel::_1);

   channel = tim.channel<mcu::Periph::TIM2, mcu::PA15>();
   BOOST_CHECK(channel == TIM::Channel::_1);

   channel = tim.channel<mcu::Periph::TIM2, mcu::PB3>();
   BOOST_CHECK(channel == TIM::Channel::_2);

   channel = tim.channel<mcu::Periph::TIM2, mcu::PB10>();
   BOOST_CHECK(channel == TIM::Channel::_3);

   channel = tim.channel<mcu::Periph::TIM2, mcu::PB11>();
   BOOST_CHECK(channel == TIM::Channel::_4);

   channel = tim.channel<mcu::Periph::TIM2, mcu::PA4>();
   BOOST_CHECK(channel == TIM::Channel::error);

   // TIM3
   channel = tim.channel<mcu::Periph::TIM3, mcu::PA6>();
   BOOST_CHECK(channel == TIM::Channel::_1);

   channel = tim.channel<mcu::Periph::TIM3, mcu::PA7>();
   BOOST_CHECK(channel == TIM::Channel::_2);

   channel = tim.channel<mcu::Periph::TIM3, mcu::PB0>();
   BOOST_CHECK(channel == TIM::Channel::_3);

   channel = tim.channel<mcu::Periph::TIM3, mcu::PB1>();
   BOOST_CHECK(channel == TIM::Channel::_4);

   channel = tim.channel<mcu::Periph::TIM3, mcu::PB4>();
   BOOST_CHECK(channel == TIM::Channel::_1);

   channel = tim.channel<mcu::Periph::TIM3, mcu::PB5>();
   BOOST_CHECK(channel == TIM::Channel::_2);

   channel = tim.channel<mcu::Periph::TIM3, mcu::PC6>();
   BOOST_CHECK(channel == TIM::Channel::_1);

   channel = tim.channel<mcu::Periph::TIM3, mcu::PC7>();
   BOOST_CHECK(channel == TIM::Channel::_2);

   channel = tim.channel<mcu::Periph::TIM3, mcu::PC8>();
   BOOST_CHECK(channel == TIM::Channel::_3);

   channel = tim.channel<mcu::Periph::TIM3, mcu::PC9>();
   BOOST_CHECK(channel == TIM::Channel::_4);
#endif
}

BOOST_AUTO_TEST_SUITE_END()