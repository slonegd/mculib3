#pragma once

#include <cstdint>

namespace mcu::ADCC_bits {

struct CSR {
   bool    AWD1   :1; // Bit 0 AWD: Analog watchdog flag
   bool    EOC1   :1; // Bit 1 EOC1: End of conversion
   bool    JEOC1  :1; // Bit 2 JEOC1: Injected channel end of conversion
   bool    JSTRT1 :1; // Bit 3 JSTRT1: Injected channel Start flag
   bool    STRT1  :1; // Bit 4 STRT1: Regular channel Start flag
   bool    OVR1   :1; // Bit 5 OVR1: Overrun flag
   uint32_t       :2; // Bits 7:6 Reserved, must be kept at reset value.
   bool    AWD2   :1;
   bool    EOC2   :1;
   bool    JEOC2  :1;
   bool    JSTRT2 :1;
   bool    STRT2  :1;
   bool    OVR2   :1;
   uint32_t       :2;
   bool    AWD3   :1;
   bool    EOC3   :1;
   bool    JEOC3  :1;
   bool    JSTRT3 :1;
   bool    STRT3  :1;
   bool    OVR3   :1;
   uint32_t       :10;
}__attribute__((packed));




struct CCR {
   enum Delay { CLKx5 = 0b0000, CLKx6,  CLKx7,  CLKx8,  CLKx9, CLKx10, CLKx11, CLKx12,
               CLKx13, CLKx14, CLKx15, CLKx16, CLKx17, CLKx18, CLKx19, CLKx20 };
   enum Clock { PCLKdiv2 = 0b00, PCLKdiv4, PCLKdiv6, PCLKdiv8 };
   uint32_t MULTI   :5; // Bits 4:0 MULTI[4:0]: Multi ADC mode selection
   uint32_t         :3; // Bits 7:5 Reserved, must be kept at reset value.
   Delay    DELAY   :4; // Bits 11:8 DELAY: Delay between 2 sampling phases
   uint32_t         :1; // Bit 12 Reserved, must be kept at reset value.
   bool     DDS     :1; // Bit 13 DDS: DMA disable selection (for multi-ADC mode)
   uint32_t DMA     :2; // Bits 15:14 DMA: Direct memory access mode for multi ADC mode
   Clock    ADCPRE  :2; // Bits 17:16 ADCPRE: ADC prescaler
   uint32_t         :4; // Bits 21:18 Reserved, must be kept at reset value.
   bool     VBATE   :1; // Bit 22 VBATE: V BAT enable
   bool     TSVREFE :1; // Bit 23 TSVREFE: Temperature sensor and V REFINT enable
   uint32_t         :8; // Bits 31:24 Reserved, must be kept at reset value.
}__attribute__((packed));