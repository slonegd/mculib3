#pragma once

#include "defines.h"
#if defined(STM32F030x6)
   #include "RCC_F0_bits.h"
#elif defined(STM32F405xx)
   #include "RCC_F4_bits.h"
#endif



extern const uint32_t fCPU;


#undef RCC
class RCC
{
public:
   using PLLsource         = RCC_ral::PLLsource;
   using APBprescaler      = RCC_ral::APBprescaler;
   using AHBprescaler      = RCC_ral::AHBprescaler;
   using SystemClockSwitch = RCC_ral::SystemClockSwitch;
#if defined(STM32F405xx)
   using PLLPdiv           = RCC_ral::PLLPdiv;
#elif defined (STM32F030x6)
   using PLLmultiplier     = RCC_ral::PLLmultiplier;
#endif
   enum Bus { APB1, APB2 };

   static constexpr uint32_t Base = RCC_BASE;

   RCC() = delete;
   static RCC* create() { return reinterpret_cast<RCC*>(Base); }
   void doSome() { CR.bits.res1 = 0; }

   static void     HSEon();
   static void     waitHSEready();
   static void     PLLon();
   static void     waitPLLready();
   static void     setPLLsource (PLLsource val);
   static uint32_t getAPBclock (APBprescaler val);
   static uint32_t getAPB2clock();
   static void     setAHBprescaler (AHBprescaler val);
   static void     systemClockSwitch (SystemClockSwitch val);

   template<class Periph> static void clockEnable();

#if defined(STM32F405xx)
   static void     setAPB1prescaler (APBprescaler val);
   static void     setAPB2prescaler (APBprescaler val);
   static uint32_t getAPB1clock();
   static void     setPLLP (PLLPdiv val);
   template <uint8_t val>  static void setPLLM();
   template <uint16_t val> static void setPLLN();
   template <uint8_t val>  static void setPLLQ();
#elif defined (STM32F030x6)
   static void     setAPBprecsaler (APBprescaler val);
   static void     setPLLmultiplier (PLLmultiplier val);
#endif




protected:
#define MAKE_REF(Type, name) static __IO Type& name() { return *(Type *)(Base + Type::Offset); }
   MAKE_REF(RCC_ral::CR_t     , clockContr)
   MAKE_REF(RCC_ral::CFGR_t   , conf      )
   MAKE_REF(RCC_ral::APB1ENR_t, APB1en    )
   MAKE_REF(RCC_ral::APB2ENR_t, APB2en    )
#if defined (STM32F405xx)
   MAKE_REF(RCC_ral::PLLCFGR_t, pllConf   )
   MAKE_REF(RCC_ral::AHB1ENR_t, AHB1en    )
#elif defined (STM32F030x6)
   MAKE_REF(RCC_ral::AHBENR_t,  AHBen     )
#endif
#undef MAKE_REF

private:
#if defined (STM32F405xx)
   RCC_ral::CR_t         CR;         // RCC clock control register
   RCC_ral::PLLCFGR_t    PLLCFGR;    // RCC PLL configuration register
   RCC_ral::CFGR_t       CFGR;       // RCC clock configuration register
   RCC_ral::CIR_t        CIR;        // RCC clock interrupt register
   RCC_ral::AHB1RSTR_t   AHB1RSTR;   // RCC AHB1 peripheral reset register
   RCC_ral::AHB2RSTR_t   AHB2RSTR;   // RCC AHB2 peripheral reset register
   RCC_ral::AHB3RSTR_t   AHB3RSTR;   // RCC AHB3 peripheral reset register
   Reserve_t<1>          reserve1;
   RCC_ral::APB1RSTR_t   APB1RSTR;   // RCC APB1 peripheral reset register
   RCC_ral::APB2RSTR_t   APB2RSTR;   // RCC APB2 peripheral reset register
   Reserve_t<2>          reserve2;
   RCC_ral::AHB1ENR_t    AHB1ENR;    // RCC AHB1 peripheral clock register
   RCC_ral::AHB2ENR_t    AHB2ENR;    // RCC AHB2 peripheral clock register
   RCC_ral::AHB3ENR_t    AHB3ENR;    // RCC AHB3 peripheral clock register
   Reserve_t<1>          reserve3;
   RCC_ral::APB1ENR_t    APB1ENR;    // RCC APB1 peripheral clock enable register
   RCC_ral::APB2ENR_t    APB2ENR;    // RCC APB2 peripheral clock enable register
   Reserve_t<2>          reserve4;
   RCC_ral::AHB1LPENR_t  AHB1LPENR;  // RCC AHB1 peripheral clock enable in low power mode register
   RCC_ral::AHB2LPENR_t  AHB2LPENR;  // RCC AHB2 peripheral clock enable in low power mode register
   RCC_ral::AHB3LPENR_t  AHB3LPENR;  // RCC AHB3 peripheral clock enable in low power mode register
   Reserve_t<1>          reserve5; 
   RCC_ral::APB1LPENR_t  APB1LPENR;  // RCC APB1 peripheral clock enable in low power mode register
   RCC_ral::APB2LPENR_t  APB2LPENR;  // RCC APB2 peripheral clock enable in low power mode register
   Reserve_t<2>          reserve6;
   RCC_ral::BDCR_t       BDCR;       // RCC Backup domain control register
   RCC_ral::CSR_t        CSR;        // RCC clock control & status register
   Reserve_t<2>          reserve7;
   RCC_ral::SSCGR_t      SSCGR;      // RCC spread spectrum clock generation register
   RCC_ral::PLLI2SCFGR_t PLLI2SCFGR; // RCC PLLI2S configuration register
#elif defined (STM32F030x6)
   RCC_ral::CR_t       CR;       // RCC clock control register
   RCC_ral::CFGR_t     CFGR;     // RCC clock configuration register
   RCC_ral::CIR_t      CIR;      // RCC clock interrupt register
   RCC_ral::APB2RSTR_t APB2RSTR; // RCC APB2 peripheral reset register
   RCC_ral::APB1RSTR_t APB1RSTR; // RCC APB1 peripheral reset register
   RCC_ral::AHBENR_t   AHBENR;   // RCC AHB peripheral clock register
   RCC_ral::APB2ENR_t  APB2ENR;  // RCC APB2 peripheral clock enable register
   RCC_ral::APB1ENR_t  APB1ENR;  // RCC APB1 peripheral clock enable register
   RCC_ral::BDCR_t     BDCR;     // RCC Backup domain control register
   RCC_ral::CSR_t      CSR;      // RCC clock control & status register
   RCC_ral::AHBRSTR_t  AHBRSTR;  // RCC AHB peripheral reset register
   RCC_ral::CFGR2_t    CFGR2;    // RCC clock configuration register 2
   RCC_ral::CFGR3_t    CFGR3;    // RCC clock configuration register 3
   RCC_ral::CR2_t      CR2;      // RCC clock control register 2
#endif




};

#include "RCC_src.h"