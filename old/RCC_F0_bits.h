#pragma once

#include "registr.h"


namespace RCC_ral {

struct CRbits {
   __IO bool     HSION   :1; // Bit 0 HSION: Internal high-speed clock enable
   __IO bool     HSIRDY  :1; // Bit 1 HSIRDY: Internal high-speed clock ready flag
   __IO uint32_t res1    :1; // Bit 2 Reserved, must be kept at reset value.
   __IO uint32_t HSITRIM :5; // Bits 7:3 HSITRIM[4:0]: Internal high-speed clock trimming
   __IO uint32_t HSICAL  :8; // Bits 15:8 HSICAL[7:0]: Internal high-speed clock calibration
   __IO bool     HSEON   :1; // Bit 16 HSEON: HSE clock enable
   __IO bool     HSERDY  :1; // Bit 17 HSERDY: HSE clock ready flag
   __IO bool     HSEBYP  :1; // Bit 18 HSEBYP: HSE clock bypass
   __IO bool     CSSON   :1; // Bit 19 CSSON: Clock security system enable
   __IO uint32_t res2    :4; // Bits 23:20 Reserved, must be kept at reset value.
   __IO bool     PLLON   :1; // Bit 24 PLLON: Main PLL (PLL) enable
   __IO bool     PLLRDY  :1; // Bit 25 PLLRDY: Main PLL (PLL) clock ready flag
   __IO uint32_t res3    :6;
};

struct CRposition {
   enum { HSION = 0, HSIRDY, HSITRIM = 3, HSICAL = 8, HSEON = 16, HSERDY,
      HSEBYP, CSSON, PLLON = 24, PLLRDY
   };
};

enum PLLdivMCO { PLLdiv1 = 0b0, PLLdiv2 };
enum PLLmultiplier { _2 = 0b0000, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16 };
enum MCOprescale { MCOdiv1 = 0b000, MCOdiv2, MCOdiv4, MCOdiv8, MCOdiv16, MCOdiv32, MCOdiv64, MCOdiv128 };
enum AHBprescaler { AHBnotdiv = 0, AHBdiv2 = 0b1000, AHBdiv4, AHBdiv8, AHBdiv16, AHBdiv64,AHBdiv128, AHBdiv256, AHBdiv512 };
enum APBprescaler { APBnotdiv = 0, APBdiv2 = 0b100, APBdiv4   = 0b101, APBdiv8 = 0b110, APBdiv16  = 0b111 };
enum SystemClockSwitch { CS_HSI = 0, CS_HSE, CS_PLL };
enum PLLsource { HSIdiv2 = 0b0, HSE };

struct CFGRbits {
   __IO SystemClockSwitch SW       :2; // Bits 1:0 SW: System clock switch
   __IO SystemClockSwitch SWS      :2; // Bits 3:2 SWS: System clock switch status
   __IO AHBprescaler      HPRE     :4; // Bits 7:4 HPRE: AHB prescaler
   __IO APBprescaler      PPRE     :3; // Bits 10:8 PPRE: PCLK prescaler
   __IO uint32_t          res1     :3; // Bits 13:11 Reserved, must be kept at reset value
   __IO bool              ADCPRE   :1; // Bit 14 ADC prescaler
   __IO uint32_t          res2     :1; // Bit 15 Reserved, must be kept at reset value
   __IO PLLsource         PLLSRC   :1; // Bit 16 PLL entry clock source
   __IO bool              PLLXTPRE :1; // Bit 17 HSE divider for PLL input clock
   __IO PLLmultiplier     PLLMUL   :4; // Bits 21:18 PLL  multiplication factor
   __IO uint32_t          res3     :2; // Bits 23:22 Reserved, must be kept at reset value
   __IO uint32_t          MCO      :4; // Bits 27:24 MCO Microcontroller clock output
   __IO MCOprescale       MCOPRE   :3; // Bits 30:28 MCOPRE Microcontroller Clock Output Prescaler
   __IO PLLdivMCO         PLLNOdiv :1; // Bit 31 PLLNODIV PLL clock not divided for MCO
};

struct CFGRposition {
   enum { SW = 0, SWS = 2, HPRE = 4, PPRE = 8, ADCPRE = 14, PLLSRC = 16,
      PLLXTPRE, PLLMUL, MCO = 24, MCOPRE = 28, PLLNOdiv = 31
   };
};

struct AHBENRbits {
   __IO bool     DMAEN   :1; // Bit 0 DMAEN: DMA clock enable
   __IO uint32_t res1    :1; // Bit 1 Reserved, must be kept at reset value.
   __IO bool     SRAMEN  :1; // Bit 2 SRAMEN: SRAM interface clock enable
   __IO uint32_t res2    :1; // Bit 3 Reserved, must be kept at reset value.
   __IO bool     FLITFEN :1; // Bit 4 FLITFEN: FLITF clock enable
   __IO uint32_t res3    :1; // Bit 5 Reserved, must be kept at reset value.
   __IO bool     CRCEN   :1; // Bit 6 CRCEN: CRC clock enable
   __IO uint32_t res4    :10; // Bits 16:7 Reserved, must be kept at reset value.
   __IO bool     IOPAEN  :1; // Bit 17 IOPAEN: I/O port A clock enable
   __IO bool     IOPBEN  :1; // Bit 18 IOPBEN: I/O port B clock enable
   __IO bool     IOPCEN  :1; // Bit 19 IOPCEN: I/O port C clock enable
   __IO bool     IOPDEN  :1; // Bit 20 IOPDEN: I/O port D clock enable
   __IO uint32_t res5    :1; // Reserved, must be kept at reset value.
   __IO bool     IOPFEN  :1; // Bit 22 IOPFEN: I/O port F clock enable
   __IO uint32_t res6    :9; // Bits 31:23 Reserved, must be kept at reset value.
};

struct AHBENRposition {
   enum { DMAEN = 0, SRAMEN = 2, FLITFEN = 4, CRCEN = 6, IOPAEN = 17, IOPBEN, IOPCEN, IOPDEN, IOPFEN = 22 };
};

struct APB2ENRbits {
   __IO bool     SYSCFGEN :1; // Bit 0 SYSCFGEN: SYSCFG clock enable
   __IO uint32_t res1     :4; // Bits 4:1 Reserved, must be kept at reset value.
   __IO bool     USART6EN :1; // Bit 5 USART6EN: USART6 clock enable
   __IO uint32_t res2     :3; // Bits 8:6 Reserved, must be kept at reset value.
   __IO bool     ADC1EN   :1; // Bit 9 ADCEN: ADC interface clock enable
   __IO uint32_t res3     :1; // Bit 10 Reserved, must be kept at reset value.
   __IO bool     TIM1EN   :1; // Bit 11 TIM1EN: TIM1 timer clock enable
   __IO bool     SPI1EN   :1; // Bit 12 SPI1EN: SPI1 clock enable
   __IO uint32_t res4     :1; // Bit 13 Reserved, must be kept at reset value.
   __IO bool     USART1EN :1; // Bit 14 USART1EN: USART1 clock enable
   __IO uint32_t res5     :1; // Bit 15 Reserved, must be kept at reset value.
   __IO bool     TIM15EN  :1; // Bit 16 TIM15EN: TIM15 timer clock enable
   __IO bool     TIM16EN  :1; // Bit 17 TIM16EN: TIM16 timer clock enable
   __IO bool     TIM17EN  :1; // Bit 18 TIM17EN: TIM17 timer clock enable
   __IO uint32_t res6     :3; // Bits 21:19 Reserved, must be kept at reset value.
   __IO bool     DBGMCUEN :1; // Bit 22 DBGMCUEN MCU debug module clock enable
   __IO uint32_t res7     :9; // Bits 31:23 Reserved, must be kept at reset value.
};

struct APB2ENRposition {
   enum { SYSCFGEN = 0, USART6EN = 5, ADC1EN = 9, TIM1EN = 11, SPI1EN, USART1EN = 14, TIM15EN = 16,
      TIM16EN, TIM17EN, DBGMCUEN = 22
   };
};

struct APB1ENRbits {
   __IO bool res1      :1;
   __IO bool TIM3EN    :1;
   __IO bool res2      :2;
   __IO bool TIM6EN    :1;
   __IO bool TIM7EN    :1;
   __IO bool res3      :2;
   __IO bool TIM14EN   :1;
   __IO uint32_t res4           :2;
   __IO bool WWDGEN    :1;
   __IO uint32_t res5           :2;
   __IO bool SPI2EN    :1;
   __IO uint32_t res6           :2;
   __IO bool USART2EN  :1;
   __IO bool USART3EN  :1;
   __IO bool UART4EN   :1;
   __IO bool UART5EN   :1;
   __IO bool I2C1EN    :1;
   __IO bool I2C2EN    :1;
   __IO bool USBEN     :1;
   __IO uint32_t res7           :3;
   __IO bool PWREN     :1;
   __IO uint32_t res8           :3;
};

struct CR_t       : BitsRegistr<CRbits>     , Offset_t<0x00>, CRposition      {};
struct CFGR_t     : BitsRegistr<CFGRbits>   , Offset_t<0x04>, CFGRposition    {};
struct CIR_t      : DataRegistr             , Offset_t<0x08>                  {};
struct APB2RSTR_t : DataRegistr             , Offset_t<0x0C>                  {};
struct APB1RSTR_t : DataRegistr             , Offset_t<0x10>                  {};
struct AHBENR_t   : BitsRegistr<AHBENRbits> , Offset_t<0x14>, AHBENRposition  {};
struct APB2ENR_t  : BitsRegistr<APB2ENRbits>, Offset_t<0x18>, APB2ENRposition {};
struct APB1ENR_t  : BitsRegistr<APB1ENRbits>, Offset_t<0x1C>                  {};
struct BDCR_t     : DataRegistr             , Offset_t<0x20>                  {};
struct CSR_t      : DataRegistr             , Offset_t<0x24>                  {};
struct AHBRSTR_t  : DataRegistr             , Offset_t<0x28>                  {};
struct CFGR2_t    : DataRegistr             , Offset_t<0x2C>                  {};
struct CFGR3_t    : DataRegistr             , Offset_t<0x30>                  {};
struct CR2_t      : DataRegistr             , Offset_t<0x34>                  {};


} // namespace RCC_ral