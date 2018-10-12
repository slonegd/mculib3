#pragma once

#include <cstdint>

namespace mcu::RCC_bits {

struct CR {
   bool     HSION   :1; // Bit 0 HSION: Internal high-speed clock enable
   bool     HSIRDY  :1; // Bit 1 HSIRDY: Internal high-speed clock ready flag
   uint32_t res1    :1; // Bit 2 Reserved, must be kept at reset value.
   uint32_t HSITRIM :5; // Bits 7:3 HSITRIM[4:0]: Internal high-speed clock trimming
   uint32_t HSICAL  :8; // Bits 15:8 HSICAL[7:0]: Internal high-speed clock calibration
   bool     HSEON   :1; // Bit 16 HSEON: HSE clock enable
   bool     HSERDY  :1; // Bit 17 HSERDY: HSE clock ready flag
   bool     HSEBYP  :1; // Bit 18 HSEBYP: HSE clock bypass
   bool     CSSON   :1; // Bit 19 CSSON: Clock security system enable
   uint32_t res2    :4; // Bits 23:20 Reserved, must be kept at reset value.
   bool     PLLON   :1; // Bit 24 PLLON: Main PLL (PLL) enable
   bool     PLLRDY  :1; // Bit 25 PLLRDY: Main PLL (PLL) clock ready flag
   uint32_t res3    :6;
}__attribute__((packed));

struct CFGR {
   enum PLLdivMCO { PLLdiv1 = 0b0, PLLdiv2 };
   enum PLLmultiplier { _2 = 0b0000, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16 };
   enum MCOprescale { MCOdiv1 = 0b000, MCOdiv2, MCOdiv4, MCOdiv8, MCOdiv16, MCOdiv32, MCOdiv64, MCOdiv128 };
   enum AHBprescaler { AHBnotdiv = 0, AHBdiv2 = 0b1000, AHBdiv4, AHBdiv8, AHBdiv16, AHBdiv64,AHBdiv128, AHBdiv256, AHBdiv512 };
   enum APBprescaler { APBnotdiv = 0, APBdiv2 = 0b100, APBdiv4   = 0b101, APBdiv8 = 0b110, APBdiv16  = 0b111 };
   enum SystemClock { CS_HSI = 0, CS_HSE, CS_PLL };
   enum PLLsource { HSIdiv2 = 0b0, HSE };
   SystemClock   SW       :2; // Bits 1:0 SW: System clock switch
   SystemClock   SWS      :2; // Bits 3:2 SWS: System clock switch status
   AHBprescaler  HPRE     :4; // Bits 7:4 HPRE: AHB prescaler
   APBprescaler  PPRE     :3; // Bits 10:8 PPRE: PCLK prescaler
   uint32_t      res1     :3; // Bits 13:11 Reserved, must be kept at reset value
   bool          ADCPRE   :1; // Bit 14 ADC prescaler
   uint32_t      res2     :1; // Bit 15 Reserved, must be kept at reset value
   PLLsource     PLLSRC   :1; // Bit 16 PLL entry clock source
   bool          PLLXTPRE :1; // Bit 17 HSE divider for PLL input clock
   PLLmultiplier PLLMUL   :4; // Bits 21:18 PLL  multiplication factor
   uint32_t      res3     :2; // Bits 23:22 Reserved, must be kept at reset value
   uint32_t      MCO      :4; // Bits 27:24 MCO Microcontroller clock output
   MCOprescale   MCOPRE   :3; // Bits 30:28 MCOPRE Microcontroller Clock Output Prescaler
   PLLdivMCO     PLLNOdiv :1; // Bit 31 PLLNODIV PLL clock not divided for MCO
}__attribute__((packed));

struct AHBENR {
   bool     DMAEN   :1; // Bit 0 DMAEN: DMA clock enable
   uint32_t res1    :1; // Bit 1 Reserved, must be kept at reset value.
   bool     SRAMEN  :1; // Bit 2 SRAMEN: SRAM interface clock enable
   uint32_t res2    :1; // Bit 3 Reserved, must be kept at reset value.
   bool     FLITFEN :1; // Bit 4 FLITFEN: FLITF clock enable
   uint32_t res3    :1; // Bit 5 Reserved, must be kept at reset value.
   bool     CRCEN   :1; // Bit 6 CRCEN: CRC clock enable
   uint32_t res4    :10; // Bits 16:7 Reserved, must be kept at reset value.
   bool     IOPAEN  :1; // Bit 17 IOPAEN: I/O port A clock enable
   bool     IOPBEN  :1; // Bit 18 IOPBEN: I/O port B clock enable
   bool     IOPCEN  :1; // Bit 19 IOPCEN: I/O port C clock enable
   bool     IOPDEN  :1; // Bit 20 IOPDEN: I/O port D clock enable
   uint32_t res5    :1; // Reserved, must be kept at reset value.
   bool     IOPFEN  :1; // Bit 22 IOPFEN: I/O port F clock enable
   uint32_t res6    :9; // Bits 31:23 Reserved, must be kept at reset value.
}__attribute__((packed));

struct APB2ENR {
   bool     SYSCFGEN :1; // Bit 0 SYSCFGEN: SYSCFG clock enable
   uint32_t res1     :4; // Bits 4:1 Reserved, must be kept at reset value.
   bool     USART6EN :1; // Bit 5 USART6EN: USART6 clock enable
   uint32_t res2     :3; // Bits 8:6 Reserved, must be kept at reset value.
   bool     ADC1EN   :1; // Bit 9 ADCEN: ADC interface clock enable
   uint32_t res3     :1; // Bit 10 Reserved, must be kept at reset value.
   bool     TIM1EN   :1; // Bit 11 TIM1EN: TIM1 timer clock enable
   bool     SPI1EN   :1; // Bit 12 SPI1EN: SPI1 clock enable
   uint32_t res4     :1; // Bit 13 Reserved, must be kept at reset value.
   bool     USART1EN :1; // Bit 14 USART1EN: USART1 clock enable
   uint32_t res5     :1; // Bit 15 Reserved, must be kept at reset value.
   bool     TIM15EN  :1; // Bit 16 TIM15EN: TIM15 timer clock enable
   bool     TIM16EN  :1; // Bit 17 TIM16EN: TIM16 timer clock enable
   bool     TIM17EN  :1; // Bit 18 TIM17EN: TIM17 timer clock enable
   uint32_t res6     :3; // Bits 21:19 Reserved, must be kept at reset value.
   bool     DBGMCUEN :1; // Bit 22 DBGMCUEN MCU debug module clock enable
   uint32_t res7     :9; // Bits 31:23 Reserved, must be kept at reset value.
}__attribute__((packed));

struct APB1ENR {
   uint32_t res1     :1;
   bool     TIM3EN   :1;
   uint32_t res2     :2;
   bool     TIM6EN   :1;
   bool     TIM7EN   :1;
   bool     res3     :2;
   bool     TIM14EN  :1;
   uint32_t res4     :2;
   bool     WWDGEN   :1;
   uint32_t res5     :2;
   bool     SPI2EN   :1;
   uint32_t res6     :2;
   bool     USART2EN :1;
   bool     USART3EN :1;
   bool     UART4EN  :1;
   bool     UART5EN  :1;
   bool     I2C1EN   :1;
   bool     I2C2EN   :1;
   bool     USBEN    :1;
   uint32_t res7     :3;
   bool     PWREN    :1;
   uint32_t res8     :3;
}__attribute__((packed));

} // namespace mcu::RCC_bits {

