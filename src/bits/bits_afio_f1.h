#pragma once

#include <cstdint>

namespace mcu::AFIO_bits {

enum Port {PA = 0b000, PB, PC, PD, PE, PF, PG};
struct EVCR {
   enum Pin {_0 = 0b0000, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15 };
   Pin PIN      :4;  // Bits 3:0 PIN[3:0]: Pin selection
   Port PORT    :3;  // Bits 6:4 PORT[2:0]: Port selection
   bool EVOE    :1;  // Bit 7 EVOE: Event output enable
   uint32_t res :24; // Bits 31:8 Reserved
}__attribute__((packed));

struct MAPR {
   enum Remap {No = 0b00, Partial_1, Partial_2, Full };
   enum SWJ   {Full_SWJ_NJTRST = 0b000, Full_SWJ = 0b001, JTAG_off_SW_on = 0b010, JTAG_off_SW_off = 0b100};
   bool     SPI1_REMAP         :1; // Bit 0 SPI1_REMAP: SPI1 remapping
   bool     I2C1_REMAP         :1; // Bit 1 I2C1_REMAP: I2C1 remapping
   bool     USART1_REMAP       :1; // Bit 2 USART1_REMAP: USART1 remapping
   bool     USART2_REMAP       :1; // Bit 3 USART2_REMAP: USART2 remapping
   Remap    USART3_REMAP       :2; // Bits 5:4 USART3_REMAP[1:0]: USART3 remapping
   Remap    TIM1_REMAP         :2; // Bits 7:6 TIM1_REMAP[1:0]: TIM1 remapping
   Remap    TIM2_REMAP         :2; // Bits 9:8 TIM2_REMAP[1:0]: TIM2 remapping
   Remap    TIM3_REMAP         :2; // Bits 11:10 TIM3_REMAP[1:0]: TIM3 remapping
   bool     TIM4_REMAP         :1; // Bit 12 TIM4_REMAP: TIM4 remapping
   uint32_t CAN_REMAP          :2; // Bits 14:13 CAN_REMAP[1:0]: CAN alternate function remapping
   bool     PD01_REMAP         :1; // Bit 15 PD01_REMAP: Port D0/Port D1 mapping on OSC_IN/OSC_OUT
   bool     TIM5CH4_IREMAP     :1; // Bits 16 TIM5CH4_IREMAP: TIM5 channel4 internal remap
   bool     ADC1_ETRGINJ_REMAP :1; // Bits 17 ADC1_ETRGINJ_REMAP: ADC 1 External trigger injected conversion remapping
   bool     ADC1_ETRGREG_REMAP :1; // Bits 18 ADC1_ETRGREG_REMAP: ADC 1 external trigger regular conversion remapping
   bool     ADC2_ETRGINJ_REMAP :1; // Bits 19 ADC1_ETRGREG_REMAP: ADC 1 external trigger regular conversion remapping
   bool     ADC2_ETRGREG_REMAP :1; // Bits 20 ADC2_ETRGINJ_REMAP: ADC 2 external trigger injected conversion remapping
   uint32_t res1               :3; // Bits 23:21 Reserved.
   SWJ      SWJ_CFG            :3; // Bits 26:24 SWJ_CFG[2:0]: Serial wire JTAG configuration
   uint32_t res2               :5; // Bits 31:27 Reserved
}__attribute__((packed));

struct EXTICR {
   Port     EXTI0  :4;  // Bits 15:0 EXTIx[3:0]: EXTI x configuration (x= 0 to 3)
   Port     EXTI1  :4;
   Port     EXTI2  :4;
   Port     EXTI3  :4;
   uint32_t res1   :16; // Bits 31:16 Reserved
   Port     EXTI4  :4;  // Bits 15:0 EXTIx[3:0]: EXTI x configuration (x= 4 to 7)
   Port     EXTI5  :4;
   Port     EXTI6  :4;
   Port     EXTI7  :4;
   uint32_t res2   :16; // Bits 31:16 Reserved
   Port     EXTI8  :4;  // Bits 15:0 EXTIx[3:0]: EXTI x configuration (x= 8 to 11)
   Port     EXTI9  :4;
   Port     EXTI10 :4;
   Port     EXTI11 :4;
   uint32_t res3   :16; // Bits 31:16 Reserved
   Port     EXTI12 :4;  // Bits 15:0 EXTIx[3:0]: EXTI x configuration (x= 12 to 15)
   Port     EXTI13 :4;
   Port     EXTI14 :4;
   Port     EXTI15 :4;
   uint32_t res4   :16; // // Bits 31:16 Reserved
}__attribute__((packed));

struct MAPR2 {
   uint32_t res1    :5; // Bits 4:0 Reserved.
   bool TIM9_REMAP  :1; // Bit 5 TIM9_REMAP: TIM9 remapping
   bool TIM10_REMAP :1; // Bit 6 TIM10_REMAP: TIM10 remapping
   bool TIM11_REMAP :1; // Bit 7 TIM11_REMAP: TIM11 remapping
   bool TIM13_REMAP :1; // Bit 8 TIM13_REMAP: TIM13 remapping
   bool TIM14_REMAP :1; // Bit 9 TIM14_REMAP: TIM14 remapping
   bool FSMC_NADV   :1; // Bit 10 FSMC_NADV: NADV connect/disconnect
}__attribute__((packed));
} //namespace mcu::AFIO_bits