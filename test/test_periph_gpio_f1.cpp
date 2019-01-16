#define BOOST_TEST_MODULE f1_test_periph_gpio
#include <boost/test/unit_test.hpp>

#define F_CPU   72'000'000UL
#define STM32F103xB

#include "mock_rcc.h"
#include "mock_afio.h"
#include "catch_static_assert.h"
#include "periph_gpio.h"
#undef static_assert
#include <iostream>
#include <type_traits>
#include <thread>
#include "timeout.h"

// отсутсвуют в cmsis f1
#define GPIO_BSRR_BS_0  GPIO_BSRR_BS0
#define GPIO_BSRR_BS_1  GPIO_BSRR_BS1
#define GPIO_BSRR_BS_2  GPIO_BSRR_BS2
#define GPIO_BSRR_BS_3  GPIO_BSRR_BS3
#define GPIO_BSRR_BS_4  GPIO_BSRR_BS4
#define GPIO_BSRR_BS_5  GPIO_BSRR_BS5
#define GPIO_BSRR_BS_6  GPIO_BSRR_BS6
#define GPIO_BSRR_BS_7  GPIO_BSRR_BS7
#define GPIO_BSRR_BS_8  GPIO_BSRR_BS8
#define GPIO_BSRR_BS_9  GPIO_BSRR_BS9
#define GPIO_BSRR_BS_10 GPIO_BSRR_BS10
#define GPIO_BSRR_BS_11 GPIO_BSRR_BS11
#define GPIO_BSRR_BS_12 GPIO_BSRR_BS12
#define GPIO_BSRR_BS_13 GPIO_BSRR_BS13
#define GPIO_BSRR_BS_14 GPIO_BSRR_BS14
#define GPIO_BSRR_BS_15 GPIO_BSRR_BS15

#define GPIO_BSRR_BR_0  GPIO_BSRR_BR0
#define GPIO_BSRR_BR_1  GPIO_BSRR_BR1
#define GPIO_BSRR_BR_2  GPIO_BSRR_BR2
#define GPIO_BSRR_BR_3  GPIO_BSRR_BR3
#define GPIO_BSRR_BR_4  GPIO_BSRR_BR4
#define GPIO_BSRR_BR_5  GPIO_BSRR_BR5
#define GPIO_BSRR_BR_6  GPIO_BSRR_BR6
#define GPIO_BSRR_BR_7  GPIO_BSRR_BR7
#define GPIO_BSRR_BR_8  GPIO_BSRR_BR8
#define GPIO_BSRR_BR_9  GPIO_BSRR_BR9
#define GPIO_BSRR_BR_10 GPIO_BSRR_BR10
#define GPIO_BSRR_BR_11 GPIO_BSRR_BR11
#define GPIO_BSRR_BR_12 GPIO_BSRR_BR12
#define GPIO_BSRR_BR_13 GPIO_BSRR_BR13
#define GPIO_BSRR_BR_14 GPIO_BSRR_BR14
#define GPIO_BSRR_BR_15 GPIO_BSRR_BR15



#include "test_periph_gpio_f0_f1_f4_f7.h"

