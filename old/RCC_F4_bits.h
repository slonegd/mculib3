#pragma once

#include "registr.h"


namespace RCC_ral {

struct CRbits {
   bool     HSION     :1; // Bit 0 HSION: Internal high-speed clock enable
   bool     HSIRDY    :1; // Bit 1 HSIRDY: Internal high-speed clock ready flag
   uint32_t res1      :1; // Bit 2 Reserved, must be kept at reset value.
   uint32_t HSITRIM   :5; // Bits 7:3 HSITRIM[4:0]: Internal high-speed clock trimming
   uint32_t HSICAL    :8; // Bits 15:8 HSICAL[7:0]: Internal high-speed clock calibration
   bool     HSEON     :1; // Bit 16 HSEON: HSE clock enable
   bool     HSERDY    :1; // Bit 17 HSERDY: HSE clock ready flag
   bool     HSEBYP    :1; // Bit 18 HSEBYP: HSE clock bypass
   bool     CSSON     :1; // Bit 19 CSSON: Clock security system enable
   uint32_t res2      :4; // Bits 23:20 Reserved, must be kept at reset value.
   bool     PLLON     :1; // Bit 24 PLLON: Main PLL (PLL) enable
   bool     PLLRDY    :1; // Bit 25 PLLRDY: Main PLL (PLL) clock ready flag
   bool     PLLI2SON  :1; // Bit 26 PLLI2SON: PLLI2S enable
   bool     PLLI2SRDY :1; // Bit 27 PLLI2SRDY: PLLI2S clock ready flag
   uint32_t res3      :4; // Bits 31:28 Reserved, must be kept at reset value.
};

struct CRposition {
   enum { HSION = 0, HSIRDY, HSITRIM = 3, HSICAL = 8, HSEON = 16, HSERDY,
      HSEBYP, CSSON, PLLON = 24, PLLRDY, PLLI2SON, PLLI2SRDY
   };
};

enum PLLPdiv { PLLdiv2 = 0b00, PLLdiv4, PLLdiv6,  PLLdiv8 };
enum PLLsource { sHSI = 0b0, sHSE = 0b1 };

struct PLLCFGRbits {
   uint32_t  PLLM   :6; // Bits 5:0 2≤PLLM≤63: Division factor for the main PLL (PLL) and audio PLL (PLLI2S) input clock
   uint32_t  PLLN   :9; // Bits 14:6 50≤PLLN≤432: Main PLL (PLL) multiplication factor for VCO
   uint32_t  dcb4   :1; // Bit 15 Reserved, must be kept at reset value.
   PLLPdiv   PLLP   :2; // Bits 17:16 PLLP: Main PLL (PLL) division factor for main system clock
   uint32_t  dcb1   :4; // Bits 21:18 Reserved, must be kept at reset value.
   PLLsource PLLSRC :1; // Bit 22 PLLSRC: Main PLL(PLL) and audio PLL (PLLI2S) entry clock source
   uint32_t  dcb2   :1; // Bit 23 Reserved, must be kept at reset value.
   uint32_t  PLLQ   :4; // Bits 27:24 2≤PLLQ≤15: Main PLL (PLL) division factor for USB OTG FS, SDIO and random number generator
   uint32_t  dcb3   :4; // Bits 31:28 Reserved, must be kept at reset value.
};

struct PLLCFGRposition {
   enum { PLLM = 0, PLLN = 6, PLLP = 16, PLLSRC = 22, PLLQ = 24 };
};

enum AHBprescaler { AHBnotdiv = 0, AHBdiv2 = 0b1000, AHBdiv4, AHBdiv8, AHBdiv16, AHBdiv64, AHBdiv128, AHBdiv256, AHBdiv512 };
enum  APBprescaler { APBnotdiv = 0, APBdiv2 = 0b100, APBdiv4, APBdiv8, APBdiv16 };
enum  SystemClockSwitch { CS_HSI = 0, CS_HSE, CS_PLL };

struct CFGRbits {
   SystemClockSwitch SW      :2; // Bits 1:0 SW: System clock switch
   SystemClockSwitch SWS     :2; // Bits 3:2 SWS: System clock switch status
   AHBprescaler      HPRE    :4; // Bits 7:4 HPRE: AHB prescaler
   uint32_t          dcb1    :2; // Bits 9:8 Reserved, must be kept at reset value.
   APBprescaler      PPRE1   :3; // Bits 12:10 PPRE1: APB Low speed prescaler (APB1)
   APBprescaler      PPRE2   :3; // Bits 15:13 PPRE2: APB high-speed prescaler (APB2)
   uint32_t          RTCPRE  :5; // Bits 20:16 2≤RTCPRE≤31: HSE division factor for RTC clock
   uint32_t          MCO1    :2; // Bits 22:21 MCO1: Microcontroller clock output 1
   uint32_t          I2SSRC  :1; // Bit 23 I2SSRC: I2S clock selection
   uint32_t          MCO1PRE :3; // Bits 24:26 MCO1PRE: MCO1 prescaler
   uint32_t          MCO2PRE :3; // Bits 27:29 MCO2PRE: MCO2 prescaler
   uint32_t          MCO2    :2; // Bits 31:30 MCO2[1:0]: Microcontroller clock output 2
};

struct CFGRposition {
   enum { SW = 0, SWS = 2, HPRE = 4, PPRE1 = 10, PPRE2 = 13, RTCPRE = 16, I2SSRC = 23, MCO1PRE, MCO2PRE = 27, MCO2 = 30 };
};

struct AHB1ENbits {
   bool     GPIOAEN      :1; // Bit 0 GPIOAEN: IO port A clock enable
   bool     GPIOBEN      :1; // Bit 1 GPIOBEN: IO port B clock enable
   bool     GPIOCEN      :1; // Bit 2 GPIOCEN: IO port C clock enable
   bool     GPIODEN      :1; // Bit 3 GPIODEN: IO port D clock enable
   bool     GPIOEEN      :1; // Bit 4 GPIOEEN: IO port E clock enable
   bool     GPIOFEN      :1; // Bit 5 GPIOFEN: IO port F clock enable
   bool     GPIOGEN      :1; // Bit 6 GPIOGEN: IO port G clock enable
   bool     GPIOHEN      :1; // Bit 7 GPIOHEN: IO port H clock enable
   bool     GPIOIEN      :1; // Bit 8 GPIOIEN: IO port I clock enable
   uint32_t res1         :3; // Bits 11:9 Reserved, must be kept at reset value.
   bool     CRCEN        :1; // Bit 12 CRCEN: CRC clock enable
   uint32_t res2         :5; // Bits 17:13 Reserved, must be kept at reset value.
   bool     BKPSRAMEN    :1; // Bit 18 BKPSRAMEN: Backup SRAM interface clock enable
   uint32_t res3         :1; // Bit 19 Reserved, must be kept at reset value.
   bool     CCMDATARAMEN :1; // Bit 20 CCMDATARAMEN: CCM data RAM clock enable
   bool     DMA1EN       :1; // Bit 21 DMA1EN: DMA1 clock enable
   bool     DMA2EN       :1; // Bit 22 DMA2EN: DMA2 clock enable
   uint32_t res4         :2; // Bits 24:23 Reserved, must be kept at reset value.
   bool     ETHMACEN     :1; // Bit 25 ETHMACEN: Ethernet MAC clock enable
   bool     ETHMACTXEN   :1; // Bit 26 ETHMACTXEN: Ethernet Transmission clock enable
   bool     ETHMACRXEN   :1; // Bit 27 ETHMACRXEN: Ethernet Reception clock enable
   bool     ETHMACPTPEN  :1; // Bit 28 ETHMACPTPEN: Ethernet PTP clock enable
   bool     OTGHSEN      :1; // Bit 29 OTGHSEN: USB OTG HS clock enable
   bool     OTGHSULPIEN  :1; // Bit 30 OTGHSULPIEN: USB OTG HSULPI clock enable
   uint32_t res5         :1; // Bit 31 Reserved, must be kept at reset value.
};

struct AHB1ENposition {
   enum { GPIOAEN = 0, GPIOBEN, GPIOCEN, GPIODEN, GPIOEEN, GPIOFEN, GPIOGEN, GPIOHEN, GPIOIEN,
      CRCEN = 12, BKPSRAMEN = 18, CCMDATARAMEN = 20, DMA1EN, DMA2EN, ETHMACEN = 25, ETHMACTXEN,
      ETHMACRXEN, ETHMACPTPEN, OTGHSEN, OTGHSULPIEN
   };
};

struct APB1ENRbits {
   bool     TIM2EN   :1; // Bit 0 TIM2EN: TIM2 clock enable
   bool     TIM3EN   :1; // Bit 1 TIM3EN: TIM3 clock enable
   bool     TIM4EN   :1; // Bit 2 TIM4EN: TIM4 clock enable
   bool     TIM5EN   :1; // Bit 3 TIM5EN: TIM5 clock enable
   bool     TIM6EN   :1; // Bit 4 TIM6EN: TIM6 clock enable
   bool     TIM7EN   :1; // Bit 5 TIM7EN: TIM7 clock enable
   bool     TIM12EN  :1; // Bit 6 TIM12EN: TIM12 clock enable
   bool     TIM13EN  :1; // Bit 7 TIM13EN: TIM13 clock enable
   bool     TIM14EN  :1; // Bit 8 TIM14EN: TIM14 clock enable
   uint32_t res1     :2; // Bits 10:9 Reserved, must be kept at reset value.
   bool     WWDGEN   :1; // Bit 11 WWDGEN: Window watchdog clock enable
   uint32_t res2     :2; // Bits 13:12 Reserved, must be kept at reset value.
   bool     SPI2EN   :1; // Bit 14 SPI2EN: SPI2 clock enable
   bool     SPI3EN   :1; // Bit 15 SPI3EN: SPI3 clock enable
   uint32_t res3     :1; // Bit 16 Reserved, must be kept at reset value.
   bool     USART2EN :1; // Bit 17 USART2EN: USART2 clock enable
   bool     USART3EN :1; // Bit 18 USART3EN: USART3 clock enable
   bool     UART4EN  :1; // Bit 19 UART4EN: UART4 clock enable
   bool     UART5EN  :1; // Bit 20 UART5EN: UART5 clock enable
   bool     I2C1EN   :1; // Bit 21 I2C1EN: I2C1 clock enable
   bool     I2C2EN   :1; // Bit 22 I2C2EN: I2C2 clock enable
   bool     I2C3EN   :1; // Bit 23 I2C3EN: I2C3 clock enable
   uint32_t res4     :1; // Bit 24 Reserved, must be kept at reset value.
   bool     CAN1EN   :1; // Bit 25 CAN1EN: CAN 1 clock enable
   bool     CAN2EN   :1; // Bit 26 CAN2EN: CAN 2 clock enable
   uint32_t res5     :1; // Bit 27 Reserved, must be kept at reset value.
   bool     PWREN    :1; // Bit 28 PWREN: Power interface clock enable
   bool     DACEN    :1; // Bit 29 DACEN: DAC interface clock enable
   uint32_t res6     :2; // Bits 31:30 Reserved, must be kept at reset value.
};

struct APB1ENRposition {
   enum { TIM2EN = 0, TIM3EN, TIM4EN, TIM5EN, TIM6EN, TIM7EN, TIM12EN, TIM13EN, TIM14EN,
      WWDGEN = 11, SPI2EN = 14, SPI3EN, USART2EN = 17, USART3EN, UART4EN, UART5EN, I2C1EN,
      I2C2EN, I2C3EN, CAN1EN = 25, CAN2EN, PWREN = 28, DACEN
   };
};

struct APB2ENRbits {
   bool     TIM1EN   :1; // Bit 0 TIM1EN: TIM1 clock enable
   bool     TIM8EN   :1; // Bit 1 TIM8EN: TIM8 clock enable
   uint32_t res1     :2; // Bits 3:2 Reserved, must be kept at reset value.
   bool     USART1EN :1; // Bit 4 USART1EN: USART1 clock enable
   bool     USART6EN :1; // Bit 5 USART6EN: USART6 clock enable
   uint32_t res2     :2; // Bits 7:6 Reserved, must be kept at reset value.
   bool     ADC1EN   :1; // Bit 8 ADC1EN: ADC1 clock enable
   bool     ADC2EN   :1; // Bit 9 ADC2EN: ADC2 clock enable
   bool     ADC3EN   :1; // Bit 10 ADC3EN: ADC3 clock enable
   bool     SDIOEN   :1; // Bit 11 SDIOEN: SDIO clock enable
   bool     SPI1EN   :1; // Bit 12 SPI1EN: SPI1 clock enable
   uint32_t res3     :1; // Bit 13 Reserved, must be kept at reset value.
   bool     SYSCFGEN :1; // Bit 14 SYSCFGEN: System configuration controller clock enable
   uint32_t res4     :1; // Bit 15 Reserved, must be kept at reset value.
   bool     TIM9EN   :1; // Bit 16 TIM9EN: TIM9 clock enable
   bool     TIM10EN  :1; // Bit 17 TIM10EN: TIM10 clock enable
   bool     TIM11EN  :1; // Bit 18 TIM11EN: TIM11 clock enable
   uint32_t res5     :13; // Bits 31:19 Reserved, must be kept at reset value.
};

struct APB2ENRposition {
   enum { TIM1EN = 0, TIM8EN, USART1EN = 4, USART6EN, ADC1EN = 8, ADC2EN, ADC3EN, SDIOEN, SPI1EN,
      SYSCFGEN = 14, TIM9EN = 16, TIM10EN, TIM11EN
   };
};


struct CR_t         : BitsRegistr<CRbits>     , Offset_t<0x00>, CRposition      {};
struct PLLCFGR_t    : BitsRegistr<PLLCFGRbits>, Offset_t<0x04>, PLLCFGRposition {};
struct CFGR_t       : BitsRegistr<CFGRbits>   , Offset_t<0x08>, CFGRposition    {};
struct CIR_t        : DataRegistr             , Offset_t<0x0C> {};
struct AHB1RSTR_t   : DataRegistr             , Offset_t<0x10> {};
struct AHB2RSTR_t   : DataRegistr             , Offset_t<0x14> {};
struct AHB3RSTR_t   : DataRegistr             , Offset_t<0x18> {};
struct APB1RSTR_t   : DataRegistr             , Offset_t<0x20> {};
struct APB2RSTR_t   : DataRegistr             , Offset_t<0x24> {};
struct AHB1ENR_t    : BitsRegistr<AHB1ENbits> , Offset_t<0x30>, AHB1ENposition  {};
struct AHB2ENR_t    : DataRegistr             , Offset_t<0x34> {};
struct AHB3ENR_t    : DataRegistr             , Offset_t<0x38> {};
struct APB1ENR_t    : BitsRegistr<APB1ENRbits>, Offset_t<0x40>, APB1ENRposition {};
struct APB2ENR_t    : BitsRegistr<APB2ENRbits>, Offset_t<0x44>, APB2ENRposition {};
struct AHB1LPENR_t  : DataRegistr             , Offset_t<0x50> {};
struct AHB2LPENR_t  : DataRegistr             , Offset_t<0x54> {};
struct AHB3LPENR_t  : DataRegistr             , Offset_t<0x58> {};
struct APB1LPENR_t  : DataRegistr             , Offset_t<0x60> {};
struct APB2LPENR_t  : DataRegistr             , Offset_t<0x64> {};
struct BDCR_t       : DataRegistr             , Offset_t<0x70> {};
struct CSR_t        : DataRegistr             , Offset_t<0x74> {};
struct SSCGR_t      : DataRegistr             , Offset_t<0x80> {};
struct PLLI2SCFGR_t : DataRegistr             , Offset_t<0x84> {};



} // namespace RCC_ral