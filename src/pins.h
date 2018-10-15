// Хелперы для пинов

#pragma once

#include "gpio.h"

namespace mcu {

template <Periph p, int n_>
struct Pin_ {
   static constexpr Periph periph {p};
   static constexpr int    n      {n_};
};

using PA0  = Pin_<Periph::GPIOA,  0>;
using PA1  = Pin_<Periph::GPIOA,  1>;
using PA2  = Pin_<Periph::GPIOA,  2>;
using PA3  = Pin_<Periph::GPIOA,  3>;
using PA4  = Pin_<Periph::GPIOA,  4>;
using PA5  = Pin_<Periph::GPIOA,  5>;
using PA6  = Pin_<Periph::GPIOA,  6>;
using PA7  = Pin_<Periph::GPIOA,  7>;
using PA8  = Pin_<Periph::GPIOA,  8>;
using PA9  = Pin_<Periph::GPIOA,  9>;
using PA10 = Pin_<Periph::GPIOA, 10>;
using PA11 = Pin_<Periph::GPIOA, 11>;
using PA12 = Pin_<Periph::GPIOA, 12>;
using PA13 = Pin_<Periph::GPIOA, 13>;
using PA14 = Pin_<Periph::GPIOA, 14>;
using PA15 = Pin_<Periph::GPIOA, 15>;

using PB0  = Pin_<Periph::GPIOB,  0>;
using PB1  = Pin_<Periph::GPIOB,  1>;
using PB2  = Pin_<Periph::GPIOB,  2>;
using PB3  = Pin_<Periph::GPIOB,  3>;
using PB4  = Pin_<Periph::GPIOB,  4>;
using PB5  = Pin_<Periph::GPIOB,  5>;
using PB6  = Pin_<Periph::GPIOB,  6>;
using PB7  = Pin_<Periph::GPIOB,  7>;
using PB8  = Pin_<Periph::GPIOB,  8>;
using PB9  = Pin_<Periph::GPIOB,  9>;
using PB10 = Pin_<Periph::GPIOB, 10>;
using PB11 = Pin_<Periph::GPIOB, 11>;
using PB12 = Pin_<Periph::GPIOB, 12>;
using PB13 = Pin_<Periph::GPIOB, 13>;
using PB14 = Pin_<Periph::GPIOB, 14>;
using PB15 = Pin_<Periph::GPIOB, 15>;

using PC0  = Pin_<Periph::GPIOC,  0>;
using PC1  = Pin_<Periph::GPIOC,  1>;
using PC2  = Pin_<Periph::GPIOC,  2>;
using PC3  = Pin_<Periph::GPIOC,  3>;
using PC4  = Pin_<Periph::GPIOC,  4>;
using PC5  = Pin_<Periph::GPIOC,  5>;
using PC6  = Pin_<Periph::GPIOC,  6>;
using PC7  = Pin_<Periph::GPIOC,  7>;
using PC8  = Pin_<Periph::GPIOC,  8>;
using PC9  = Pin_<Periph::GPIOC,  9>;
using PC10 = Pin_<Periph::GPIOC, 10>;
using PC11 = Pin_<Periph::GPIOC, 11>;
using PC12 = Pin_<Periph::GPIOC, 12>;
using PC13 = Pin_<Periph::GPIOC, 13>;
using PC14 = Pin_<Periph::GPIOC, 14>;
using PC15 = Pin_<Periph::GPIOC, 15>;

using PD0  = Pin_<Periph::GPIOD,  0>;
using PD1  = Pin_<Periph::GPIOD,  1>;
using PD2  = Pin_<Periph::GPIOD,  2>;
using PD3  = Pin_<Periph::GPIOD,  3>;
using PD4  = Pin_<Periph::GPIOD,  4>;
using PD5  = Pin_<Periph::GPIOD,  5>;
using PD6  = Pin_<Periph::GPIOD,  6>;
using PD7  = Pin_<Periph::GPIOD,  7>;
using PD8  = Pin_<Periph::GPIOD,  8>;
using PD9  = Pin_<Periph::GPIOD,  9>;
using PD10 = Pin_<Periph::GPIOD, 10>;
using PD11 = Pin_<Periph::GPIOD, 11>;
using PD12 = Pin_<Periph::GPIOD, 12>;
using PD13 = Pin_<Periph::GPIOD, 13>;
using PD14 = Pin_<Periph::GPIOD, 14>;
using PD15 = Pin_<Periph::GPIOD, 15>;

using PF0  = Pin_<Periph::GPIOF,  0>;
using PF1  = Pin_<Periph::GPIOF,  1>;
using PF2  = Pin_<Periph::GPIOF,  2>;
using PF3  = Pin_<Periph::GPIOF,  3>;
using PF4  = Pin_<Periph::GPIOF,  4>;
using PF5  = Pin_<Periph::GPIOF,  5>;
using PF6  = Pin_<Periph::GPIOF,  6>;
using PF7  = Pin_<Periph::GPIOF,  7>;
using PF8  = Pin_<Periph::GPIOF,  8>;
using PF9  = Pin_<Periph::GPIOF,  9>;
using PF10 = Pin_<Periph::GPIOF, 10>;
using PF11 = Pin_<Periph::GPIOF, 11>;
using PF12 = Pin_<Periph::GPIOF, 12>;
using PF13 = Pin_<Periph::GPIOF, 13>;
using PF14 = Pin_<Periph::GPIOF, 14>;
using PF15 = Pin_<Periph::GPIOF, 15>;

#if defined (STM32F4)

using PE0  = Pin_<Periph::GPIOE,  0>;
using PE1  = Pin_<Periph::GPIOE,  1>;
using PE2  = Pin_<Periph::GPIOE,  2>;
using PE3  = Pin_<Periph::GPIOE,  3>;
using PE4  = Pin_<Periph::GPIOE,  4>;
using PE5  = Pin_<Periph::GPIOE,  5>;
using PE6  = Pin_<Periph::GPIOE,  6>;
using PE7  = Pin_<Periph::GPIOE,  7>;
using PE8  = Pin_<Periph::GPIOE,  8>;
using PE9  = Pin_<Periph::GPIOE,  9>;
using PE10 = Pin_<Periph::GPIOE, 10>;
using PE11 = Pin_<Periph::GPIOE, 11>;
using PE12 = Pin_<Periph::GPIOE, 12>;
using PE13 = Pin_<Periph::GPIOE, 13>;
using PE14 = Pin_<Periph::GPIOE, 14>;
using PE15 = Pin_<Periph::GPIOE, 15>;

using PG0  = Pin_<Periph::GPIOG,  0>;
using PG1  = Pin_<Periph::GPIOG,  1>;
using PG2  = Pin_<Periph::GPIOG,  2>;
using PG3  = Pin_<Periph::GPIOG,  3>;
using PG4  = Pin_<Periph::GPIOG,  4>;
using PG5  = Pin_<Periph::GPIOG,  5>;
using PG6  = Pin_<Periph::GPIOG,  6>;
using PG7  = Pin_<Periph::GPIOG,  7>;
using PG8  = Pin_<Periph::GPIOG,  8>;
using PG9  = Pin_<Periph::GPIOG,  9>;
using PG10 = Pin_<Periph::GPIOG, 10>;
using PG11 = Pin_<Periph::GPIOG, 11>;
using PG12 = Pin_<Periph::GPIOG, 12>;
using PG13 = Pin_<Periph::GPIOG, 13>;
using PG14 = Pin_<Periph::GPIOG, 14>;
using PG15 = Pin_<Periph::GPIOG, 15>;

using PH0  = Pin_<Periph::GPIOH,  0>;
using PH1  = Pin_<Periph::GPIOH,  1>;
using PH2  = Pin_<Periph::GPIOH,  2>;
using PH3  = Pin_<Periph::GPIOH,  3>;
using PH4  = Pin_<Periph::GPIOH,  4>;
using PH5  = Pin_<Periph::GPIOH,  5>;
using PH6  = Pin_<Periph::GPIOH,  6>;
using PH7  = Pin_<Periph::GPIOH,  7>;
using PH8  = Pin_<Periph::GPIOH,  8>;
using PH9  = Pin_<Periph::GPIOH,  9>;
using PH10 = Pin_<Periph::GPIOH, 10>;
using PH11 = Pin_<Periph::GPIOH, 11>;
using PH12 = Pin_<Periph::GPIOH, 12>;
using PH13 = Pin_<Periph::GPIOH, 13>;
using PH14 = Pin_<Periph::GPIOH, 14>;
using PH15 = Pin_<Periph::GPIOH, 15>;

using PI0  = Pin_<Periph::GPIOI,  0>;
using PI1  = Pin_<Periph::GPIOI,  1>;
using PI2  = Pin_<Periph::GPIOI,  2>;
using PI3  = Pin_<Periph::GPIOI,  3>;
using PI4  = Pin_<Periph::GPIOI,  4>;
using PI5  = Pin_<Periph::GPIOI,  5>;
using PI6  = Pin_<Periph::GPIOI,  6>;
using PI7  = Pin_<Periph::GPIOI,  7>;
using PI8  = Pin_<Periph::GPIOI,  8>;
using PI9  = Pin_<Periph::GPIOI,  9>;
using PI10 = Pin_<Periph::GPIOI, 10>;
using PI11 = Pin_<Periph::GPIOI, 11>;
using PI12 = Pin_<Periph::GPIOI, 12>;
using PI13 = Pin_<Periph::GPIOI, 13>;
using PI14 = Pin_<Periph::GPIOI, 14>;
using PI15 = Pin_<Periph::GPIOI, 15>;

#endif

} // namespace mcu {