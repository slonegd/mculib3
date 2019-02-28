#pragma once

#include <cstdint>

namespace mcu::TIM_bits {

struct CR1 {
   enum OnePulseMode { notStopped  = 0b0, counterStop };
   enum Direction { up  = 0b0, down };
   bool         CEN  :1;  // Bit  0 CEN: Counter enable
   uint32_t     UDIS :1;  // Bit  1 UDIS: Update disable
   uint32_t     URS  :1;  // Bit  2 URS: Update request source
   OnePulseMode OPM  :1;  // Bit  3 OPM: One-pulse mode
   Direction    DIR  :1;  // Bit  4 DIR: Direction
   uint32_t     CMS  :2;  // Bits 6:5 CMS: Center-aligned mode selection
   bool         ARPE :1;  // Bit  7 ARPE: Auto-reload preload enable
   uint32_t     CKD  :2;  // Bits 9:8 CKD: Clock division
   uint32_t          :22;  // Bits 15:10 Reserved, must be kept at reset value.
}__attribute__((packed));

struct CR2 {
   uint32_t CCPC  :1;  // Bit  0  CCPC: Capture/compare preloaded control
   uint32_t       :1;  // Bit  1  Reserved, must be kept at reset value.
   uint32_t CCUS  :1;  // Bit  2  CCUS: Capture/compare control update selection
   uint32_t CCDS  :1;  // Bit  3  CCDS: Capture/compare DMA selection
   uint32_t MMS   :3;  // Bits 6:4 MMS[2:0]: Master mode selection
   uint32_t TI1S  :1;  // Bit  7  TI1S: TI1 selection
   uint32_t OIS1  :1;  // Bit  8  OIS1: Output Idle state 1 (OC1 output)
   uint32_t OIS1N :1;  // Bit  9  OIS1N: Output Idle state 1 (OC1N output)
   uint32_t OIS2  :1;  // Bit  10 OIS2: Output Idle state 2 (OC2 output)
   uint32_t OIS2N :1;  // Bit  11 OIS2N: Output Idle state 2 (OC2N output)
   uint32_t OIS3  :1;  // Bit  12 OIS3: Output Idle state 3 (OC3 output)
   uint32_t OIS3N :1;  // Bit  13 OIS3N: Output Idle state 3 (OC3N output)
   uint32_t OIS4  :1;  // Bit  14 OIS4: Output Idle state 4 (OC4 output)
   uint32_t       :17; // Bits 31:15 Reserved, must be kept at reset value.
}__attribute__((packed));

struct SMCR {
   enum ExtTriggerPolarity { notInverted = 0b0, inverted };
   enum Trigger { _0 = 0b000, _1, _2, _3, TI1edge, FiltrTI1, FiltrTI2, External };
   enum SlaveMode { SMDisabled = 0b000, Encoder1, Encoder2, Encoder3, Reset, Gated, TriggerMode, ExternalClock };
   SlaveMode          SMS  :3; // Bits 2:0 SMS: Slave mode selection
   uint32_t           OCCS :1; // Bit 3 OCCS: OCREF clear selection.
   Trigger            TS   :3; // Bits 6:4 TS[2:0]: Trigger selection
   uint32_t           MSM  :1; // Bit 7 MSM: Master/slave mode
   uint32_t           ETF  :4; // Bits 11:8 ETF[3:0]: External trigger filter
   uint32_t           ETPS :2; // Bits 13:12 ETPS[1:0]: External trigger prescaler
   uint32_t           ECE  :1; // Bit 14 ECE: External clock enable
   ExtTriggerPolarity ETP  :1; // Bit 15 ETP: External trigger polarity
   uint32_t                :16;
}__attribute__((packed));

struct DIER {
   bool     UIE   :1;  // Bit 0 UIE: Update interrupt enable
   bool     CC1IE :1;  // Bit 1 CC1IE: Capture/Compare 1 interrupt enable
   bool     CC2IE :1;  // Bit 2 CC2IE: Capture/Compare 2 interrupt enable
   bool     CC3IE :1;  // Bit 3 CC2IE: Capture/Compare 3 interrupt enable
   bool     CC4IE :1;  // Bit 4 CC2IE: Capture/Compare 4 interrupt enable
   bool     COMIE :1;  // Bit 5 COMIE: COM interrupt enable
   bool     TIE   :1;  // Bit 6 TIE: Trigger interrupt enable
   bool     BIE   :1;  // Bit 7 BIE: Break interrupt enable
   bool     UDE   :1;  // Bit 8 UDE: Update DMA request enable
   bool     CC1DE :1;  // Bit 9 CC1DE: Capture/Compare 1 DMA request enable
   bool     CC2DE :1;  // Bit 10 CC1DE: Capture/Compare 2 DMA request enable
   bool     CC3DE :1;  // Bit 11 CC1DE: Capture/Compare 3 DMA request enable
   bool     CC4DE :1;  // Bit 12 CC1DE: Capture/Compare 4 DMA request enable
   bool     COMDE :1;  // Bit 13 COMDE: COM DMA request enable
   bool     TDE   :1;  // Bit 14 TDE: Trigger DMA request enable
   uint32_t       :17; // Bit 15 Reserved, must be kept at reset value.
}__attribute__((packed));

enum SelectionCompareMode { Output = 0b00, Input, InputALT, InputTRC }; 

struct Output_t {
   enum CompareMode { Off = 0b000, ActiveOnMatch, InactiveOnMatch, ToggleOnMatch,
   ForceInactive, ForceActive, PWMmode, InvertedPWMmode };
   SelectionCompareMode CC1S     :2; // Bits 1:0 CC1S: Capture/Compare 1 selection
   bool                 OC1FE    :1; // Bit 2 OC1FE: Output compare 1 fast enable
   bool                 OC1PE    :1; // Bit 3 OC1PE: Output compare 1 preload enable
   CompareMode          OC1M     :3; // Bits 6:4 OC1M: Output compare 1 mode
   bool                 OC1CE    :1; // Bit 7 OC1CE: Output compare 1 clear enable
   SelectionCompareMode CC2S     :2;
   bool                 OC2FE    :1;
   bool                 OC2PE    :1;
   CompareMode          OC2M     :3;
   bool                 OC2CE    :1;
   uint32_t                      :16;
   SelectionCompareMode CC3S     :2;
   bool                 OC3FE    :1;
   bool                 OC3PE    :1;  
   CompareMode          OC3M     :3;
   bool                 OC3CE    :1;
   SelectionCompareMode CC4S     :2;
   bool                 OC4FE    :1;
   bool                 OC4PE    :1;
   CompareMode          OC4M     :3;
   bool                 OC4CE    :1;
   uint32_t                      :16;
}__attribute__((packed));

struct Input_t { 
   SelectionCompareMode CC1S     :2; // Bits 1:0 CC1S: Capture/Compare 1 selection
   uint32_t             IC1PSC   :2; // Bits 3:2 IC1PSC: Input capture 1 prescaler
   uint32_t             IC1F     :4; // Bits 7:4 IC1F[3:0]: Input capture 1 filter
   SelectionCompareMode CC2S     :2; // Bits 9:8 CC2S: Capture/Compare 2 selection
   uint32_t             IC2PSC   :2; // Bits 11:10 IC2PSC[1:0]: Input capture 2 prescaler
   uint32_t             IC2F     :4; // Bits 15:12 IC2F: Input capture 2 filter
   uint32_t                      :16;
   SelectionCompareMode CC3S     :2;
   uint32_t             IC3PSC   :2;
   uint32_t             IC3F     :4;
   SelectionCompareMode CC4S     :2;
   uint32_t             IC4PSC   :2;
   uint32_t             IC4F     :4; 
   uint32_t                      :16;

}__attribute__((packed));

struct CCMR {
   union {
      Output_t output;
      Input_t  input;
   };
};


struct CCER {
   /// CC1P и CC1NP в CCERbits без смещения
   enum  Polarity {rising, falling, both};
   bool     CC1E  :1; // Bit 0 CC1E:   Capture/Compare 1 output enable
   uint32_t CC1P  :1; // Bit 1 CC1P:   Capture/Compare 1 output Polarity
   bool     CC1NE :1; // Bit 2 CC1NE:  Capture/Compare 1 complementary output enable
   uint32_t CC1NP :1; // Bit 3 CC1NP:  Capture/Compare 1 complementary output polarity
   bool     CC2E  :1; // Bit 4 CC2E:   Capture/Compare 2 output enable
   uint32_t CC2P  :1; // Bit 5 CC2P:   Capture/Compare 2 output polarity
   bool     CC2NE :1; // Bit 6 CC2NE:  Capture/Compare 2 complementary output enable
   uint32_t CC2NP :1; // Bit 7 CC2NP:  Capture/Compare 2 complementary output polarity
   bool     CC3E  :1; // Bit 8 CC3E:   Capture/Compare 3 output enable
   uint32_t CC3P  :1; // Bit 9 CC3P:   Capture/Compare 3 output polarity
   bool     CC3NE :1; // Bit 10 CC3NE: Capture/Compare 3 complementary output enable
   uint32_t CC3NP :1; // Bit 11 CC3NP: Capture/Compare 3 complementary output polarity
   bool     CC4E  :1; // Bit 12 CC4E:  Capture/Compare 4 output enable
   uint32_t CC4P  :1; // Bit 13 CC4P:  Capture/Compare 4 output polarity
   bool     CC4NE :1;
   uint32_t CC4NP :1;
   uint32_t       :16;
}__attribute__((packed));

struct BDTR {
   uint32_t DTG  :8; // Bits 7:0 DTG[7:0]: Dead-time generator setup
   uint32_t LOCK :2; // Bits 9:8 LOCK[1:0]: Lock configuration
   uint32_t OSSI :1; // Bit 10 OSSI: Off-state selection for Idle mode
   uint32_t OSSR :1; // Bit 11 OSSR: Off-state selection for Run mode
   bool     BKE  :1; // Bit 12 BKE: Break enable
   uint32_t BKP  :1; // Bit 13 BKP: Break polarity
   uint32_t AOE  :1; // Bit 14 AOE: Automatic output enable
   bool     MOE  :1; // Bit 15 MOE: Main output enableuint32_t      :16;
   uint32_t      :16;
}__attribute__((packed));

struct DCR {
   uint32_t DBA  :5;  // Bits 4:0 DBA[4:0]: DMA base address
   uint32_t      :3;  // Bits 7:5 Reserved, must be kept at reset value.
   uint32_t DBL  :5;  // Bits 12:8 DBL[4:0]: DMA burst length
   uint32_t      :19; // Bits 31:13 Reserved, must be kept at reset value.
}__attribute__((packed));

} // namespace mcu::RCC_bits {

