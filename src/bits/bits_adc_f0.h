#pragma once

#include <cstdint>

namespace mcu::ADC_bits {

struct SR {
   bool     ADRDY :1; // Bit 0 ADRDY: ADC ready
   bool     EOSMP :1; // Bit 1 EOSMP: End of sampling flag
   bool     EOC   :1; // Bit 2 EOC: End of conversion flag
   bool     EOSEQ :1; // Bit 3 EOSEQ: End of sequence flag
   bool     OVR   :1; // Bit 4 OVR: ADC overrun
   uint32_t       :2; // Bit 6:5 Reserved, must be kept at reset value.
   bool     AWD   :1; // Bit 7 AWD: Analog watchdog flag
   uint32_t       :24;// Bits 31:8 Reserved, must be kept at reset value.
}__attribute__((packed));

struct IER {
   bool     ADRDYIE :1; // Bits 0 ADRDYIE: ADC ready interrupt enable
   bool     EOSMPIE :1; // Bit 1 EOSMPIE: End of sampling flag interrupt enable
   bool     EOCIE   :1; // Bit 2 EOCIE: End of conversion interrupt enable
   bool     EOSEQIE :1; // Bit 3 EOSEQIE: End of conversion sequence interrupt enable
   bool     OVRIE   :1; // Bit 4 OVRIE: Overrun interrupt enable
   uint32_t         :1; // Bit 6:5 Reserved, must be kept at reset value.
   bool     AWDIE   :1; // Bit 7 AWDIE: Analog watchdog interrupt enable
   uint32_t         :24;// Bits 31:8 Reserved, must be kept at reset value.
}__attribute__((packed));

struct CR {
   bool     ADEN    :1; // Bit 0 ADEN: ADC enable command
   bool     ADDIS   :1; // Bit 1 ADDIS: ADC disable command
   bool     ADSTART :1; // Bit 2 ADSTART: ADC start conversion command
   uint32_t         :1; // Bit 3 Reserved, must be kept at reset value.
   bool     ADSTP   :1; // Bit 4 ADSTP: ADC stop conversion command
   uint32_t         :26;// Bits 30:5 Reserved, must be kept at reset value.
   bool     ADCAL   :1; // Bit 31 ADCAL: ADC calibration
}__attribute__((packed));

struct CFGR {
   enum Resolution { _12_bit = 0b00, _10_bit, _8_bit, _6_bit }; 
   enum TriggerN   { _0 = 0b000, _1, _2, _3, _4, _5, _6, _7 };
   enum Polarity   { disable = 0b00, rising, falling, both };
   enum DMA_mode   { one_shot = 0b0, circular };
   enum Align      { right = 0b0, left };
   enum Clock      { dedicated_14MHz_clock = 0b00, PCLKdiv2, PCLKdiv4 };
   // какой-то адский глюк, не принимает Trigger (EXTSEL), считает его размер более 3 бит
   bool       DMAEN   :1; // Bit 0 DMAEN: Direct memory access enable
   DMA_mode   DMACFG  :1; // Bit 1 DMACFG: Direct memory access configuration
   uint32_t   SCANDIR :1; // Bit 2 SCANDIR: Scan sequence direction
   Resolution RES     :2; // Bit 4:3 RES[1:0]: Data resolution
   Align      ALIGN   :1; // Bit 5 ALIGN: Data alignment  
   TriggerN   EXTSEL  :3; // Bits 8:6 EXTSEL[2:0]: External trigger selection
   uint32_t           :1; // Bit 9 Reserved, must be kept at reset value.
   Polarity   EXTEN   :2; // Bits 11:10 EXTEN[1:0]: External trigger enable and polarity selection
   uint32_t   OVRMOD  :1; // Bit 12 OVRMOD: Overrun management mode
   bool       CONT    :1; // Bit 13 CONT: Single / continuous conversion mode
   bool       WAIT    :1; // Bit 14 WAIT: Wait conversion mode
   bool       AUTOFF  :1; // Bit 15 AUTOFF: Auto-off mode
   bool       DISCEN  :1; // Bit 16 DISCEN: Discontinuous mode
   uint32_t           :5; // Bits 21:17 Reserved, must be kept at reset value.
   uint32_t   AWDSGL  :1; // Bit 22 AWDSGL: Enable the watchdog on a single channel or on all channels
   bool       AWDEN   :1; // Bit 23 AWDEN: Analog watchdog enable
   uint32_t           :2; // Bits 25:24 Reserved, must be kept at reset value.
   uint32_t   AWDCH   :5; // Bits 30:26 AWDCH[4:0]: Analog watchdog channel selection
   uint32_t           :1; // Bit 31 Reserved, must be kept at reset value.

   uint32_t           :30; // Bits 29:0 Reserved, must be kept at reset value.
   Clock      CKMODE  :2;  // Bits 31:30 CKMODE[1:0]: ADC clock mode
}__attribute__((packed));


struct SMPR {
   enum Sample_time { 
      _1_5_CLK = 0b000, _7_5_CLK , _13_5_CLK, _28_5_CLK , 
      _41_5_CLK       , _55_5_CLK, _71_5_CLK, _239_5_CLK,
      Default = _239_5_CLK
   };
   Sample_time SMP :3;  // Bits 2:0 SMP[2:0]: Sampling time selection
   uint32_t        :29; // Bits 31:3 Reserved, must be kept at reset value.
}__attribute__((packed));

struct TR {
   uint32_t LT    :12; // Bit 11:0 LT[11:0]: Analog watchdog lower threshold
   uint32_t       :4;  // Bits 15:12 Reserved, must be kept at reset value.
   uint32_t HT    :12; // Bit 27:16 HT[11:0]: Analog watchdog higher threshold
   uint32_t       :4;  // Bits 31:28 Reserved, must be kept at reset value.
}__attribute__((packed));


struct CHSELR {
   bool     CHSEL0  :1; // Bits 17:0 CHSELx: Channel-x selection
   bool     CHSEL1  :1;
   bool     CHSEL2  :1;
   bool     CHSEL3  :1;
   bool     CHSEL4  :1;
   bool     CHSEL5  :1;
   bool     CHSEL6  :1;
   bool     CHSEL7  :1;
   bool     CHSEL8  :1;
   bool     CHSEL9  :1;
   bool     CHSEL10 :1;
   bool     CHSEL11 :1;
   bool     CHSEL12 :1;
   bool     CHSEL13 :1;
   bool     CHSEL14 :1;
   bool     CHSEL15 :1;
   bool     CHSEL16 :1;
   bool     CHSEL17 :1;
   uint32_t         :14; // Bits 31:18 Reserved, must be kept at reset value.
}__attribute__((packed));


} //namespace mcu::ADC_bits