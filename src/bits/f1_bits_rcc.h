#pragma once

#include <cstdint>

namespace mcu::RCC_bits {

struct CR {
   bool     HSION       :1;  // Bit 0 HSION: Internal high-speed clock enable
   bool     HSIRDY      :1;  // Bit 1 HSIRDY: Internal high-speed clock ready flag
   uint32_t res1        :1;  // Bit 2 Reserved, must be kept at reset value.
   uint32_t HSITRIM     :5;  // Bits 7:3 HSITRIM[4:0]: Internal high-speed clock trimming
   uint32_t HSICAL      :8;  // Bits 15:8 HSICAL[7:0]: Internal high-speed clock calibration
   bool     HSEON       :1;  // Bit 16 HSEON: HSE clock enable
   bool     HSERDY      :1;  // Bit 17 HSERDY: HSE clock ready flag
   bool     HSEBYP      :1;  // Bit 18 HSEBYP: HSE clock bypass
   bool     CSSON       :1;  // Bit 19 CSSON: Clock security system enable
   uint32_t res2        :4;  // Bits 23:20 Reserved, must be kept at reset value.
   bool     PLLON       :1;  // Bit 24 PLLON: Main PLL (PLL) enable
   bool     PLLRDY      :1;  // Bit 25 PLLRDY: Main PLL (PLL) clock ready flag
   bool     PLL2ON      :1;  // Bit 26 PLL2ON: PLL2 enable
   bool     PLL2RDY     :1;  // Bit 27 PLL2RDY: PLL2 clock ready flag
   bool     PLL3ON      :1;  // Bit 28 PLL3ON: PLL3 enable
   bool     PLL3RDY     :1;  // Bit 29 PLL3RDY: PLL3 clock ready flag
   uint32_t res3        :2;  // Bits 31:30 Reserved, must be kept at reset value.
}__attribute__((packed));

struct CFGR {
   enum AHBprescaler { AHBnotdiv = 0, AHBdiv2 = 0b1000, AHBdiv4, AHBdiv8, AHBdiv16, AHBdiv64, AHBdiv128, AHBdiv256, AHBdiv512 };
   enum PLLmultiplier{ _2 = 0b0000, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16 };
   enum APBprescaler { APBnotdiv = 0, APBdiv2 = 0b100, APBdiv4, APBdiv8, APBdiv16 };
   enum ADCprescaler { ADCdiv2 = 0b0, ADCdiv4, ADCdiv6, ADCdiv8 };
   enum SystemClock  { CS_HSI = 0, CS_HSE, CS_PLL };
   enum PLLsource { HSIdiv2 = 0b0, HSE };
   SystemClock   SW      :2;  // Bits 1:0 SW: System clock switch
   SystemClock   SWS     :2;  // Bits 3:2 SWS: System clock switch status
   AHBprescaler  HPRE    :4;  // Bits 7:4 HPRE[3:0]: AHB prescaler
   APBprescaler  PPRE1   :3;  // Bits 10:8 PPRE1[2:0]: APB Low-speed prescaler (APB1)
   APBprescaler  PPRE2   :3;  // Bits 13:11 PPRE2[2:0]: APB high-speed prescaler (APB2)
   ADCprescaler  ADCPRE  :2;  // Bits 15:14 ADCPRE[1:0]: ADC prescaler
   PLLsource     PLLSRC  :1;  // Bit 16 PLLSRC: PLL entry clock source
   bool          PLLXTPRE:1;  // Bit 17 PLLXTPRE: LSB of division factor PREDIV1
   PLLmultiplier PLLMUL  :4;  // Bits 21:18 PLLMUL[3:0]: PLL multiplication factor
   bool          OTGFSPRE:1;  // Bit 22 OTGFSPRE: USB OTG FS prescaler
   uint32_t      res1    :1;  // Bits 23 Reserved, must be kept at reset value.
   uint32_t      MCO     :4;  // Bits 27:24 MCO[3:0]: Microcontroller clock output
   uint32_t      res2    :4;  // Bits 31:28 Reserved, must be kept at reset value.
}__attribute__((packed));

struct AHBENR {
   bool     DMA1EN      :1;  // Bit 0 DMA1EN: DMA1 clock enable
   bool     DMA2EN      :1;  // Bit 1 DMA2EN: DMA2 clock enable
   bool     SRAMEN      :1;  // Bit 2 SRAMEN: SRAM interface clock enable
   uint32_t res1        :1;  // Bit 3 Reserved, must be kept at reset value.
   bool     FLITFEN     :1;  // Bit 4 FLITFEN: FLITF clock enable
   uint32_t res2        :1;  // Bit 5 Reserved, must be kept at reset value.
   bool     CRCEN       :1;  // Bit 6 CRCEN: CRC clock enable
   uint32_t res3        :5;  // Bits 11:7 Reserved, must be kept at reset value.
   bool     OTGFSEN     :1;  // Bit 12 OTGFSEN: USB OTG FS clock enable
   uint32_t res4        :1;  // Bit 13 Reserved, must be kept at reset value.
   bool     ETHMACEN    :1;  // Bit 14 ETHMACEN: Ethernet MAC clock enable
   bool     ETHMACTXEN  :1;  // Bit 15 ETHMACTXEN: Ethernet MAC TX clock enable
   bool     ETHMACRXEN  :1;  // Bit 16 ETHMACRXEN: Ethernet MAC RX clock enable
   uint32_t res5        :15; // Bits 31:17 Reserved, must be kept at reset value.
}__attribute__((packed));

struct APB1ENR {
   bool     TIM2EN      :1;  // Bit 0 TIM2EN: Timer 2 clock enable
   bool     TIM3EN      :1;  // Bit 1 TIM3EN: Timer 3 clock enable
   bool     TIM4EN      :1;  // Bit 2 TIM4EN: Timer 4 clock enable
   bool     TIM5EN      :1;  // Bit 3 TIM5EN: Timer 5 clock enable
   bool     TIM6EN      :1;  // Bit 4 TIM6EN: Timer 6 clock enable
   bool     TIM7EN      :1;  // Bit 5 TIM7EN: Timer 7 clock enable
   uint32_t res1        :5;  // Bits 10:6 Reserved, must be kept at reset value.
   bool     WWDGEN      :1;  // Bit 11 WWDGEN: Window watchdog clock enable
   uint32_t res2        :2;  // Bits 13:12 Reserved, must be kept at reset value.
   bool     SPI2EN      :1;  // Bit 14 SPI2EN: SPI 2 clock enable
   bool     SPI3EN      :1;  // Bit 15 SPI3EN: SPI 3 clock enable
   uint32_t res3        :1;  // Bits 16 Reserved, must be kept at reset value.
   bool     USART2EN    :1;  // Bit 17 USART2EN: USART 2 clock enable
   bool     USART3EN    :1;  // Bit 18 USART3EN: USART 3 clock enable
   bool     UART4EN     :1;  // Bit 19 UART4EN: USART 4 clock enable
   bool     UART5EN     :1;  // Bit 20 UART5EN: USART 5 clock enable
   bool     I2C1EN      :1;  // Bit 21 I2C1EN: I2C 1 clock enable
   bool     I2C2EN      :1;  // Bit 22 I2C2EN: I2C 2 clock enable
   uint32_t res4        :2;  // Bits 24:23 Reserved, must be kept at reset value.
   bool     CAN1EN      :1;  // Bit 25 CAN1EN: CAN1 clock enable
   bool     CAN2EN      :1;  // Bit 26 CAN2EN: CAN2 clock enable
   bool     BKPEN       :1;  // Bit 27 BKPEN: Backup interface clock enable
   bool     PWREN       :1;  // Bit 28 PWREN: Power interface clock enable
   bool     DACEN       :1;  // Bit 29 DACEN: DAC interface clock enable
   uint32_t res5        :2;  // Bits 31:30 Reserved, must be kept at reset value.
}__attribute__((packed));

struct APB2ENR {
   bool     AFIOEN      :1;  // Bit 0 AFIOEN: Alternate function I/O clock enable
   uint32_t res1        :1;  // Bit 1 Reserved, must be kept at reset value.
   bool     IOPAEN      :1;  // Bit 2 IOPAEN: I/O port A clock enable
   bool     IOPBEN      :1;  // Bit 3 IOPBEN: I/O port B clock enable
   bool     IOPCEN      :1;  // Bit 4 IOPCEN: I/O port C clock enable
   bool     IOPDEN      :1;  // Bit 5 IOPDEN: I/O port D clock enable
   bool     IOPEEN      :1;  // Bit 6 IOPEEN: I/O port E clock enable
   uint32_t res2        :2;  // Bits 8:7 Reserved, must be kept at reset value.
   bool     ADC1EN      :1;  // Bit 9 ADC1EN: ADC 1 interface clock enable
   bool     ADC2EN      :1;  // Bit 10 ADC2EN: ADC 2 interface clock enable
   bool     TIM1EN      :1;  // Bit 11 TIM1EN: TIM1 Timer clock enable
   bool     SPI1EN      :1;  // Bit 12 SPI1EN: SPI 1 clock enable
   uint32_t res3        :1;  // Bit 13 Reserved, must be kept at reset value.
   bool     USART1EN    :1;  // Bit 14 USART1EN: USART1 clock enable
   uint32_t res4        :17; // Bits 31:15 Reserved, must be kept at reset value.
}__attribute__((packed));

} //namespace mcu::RCC_bits