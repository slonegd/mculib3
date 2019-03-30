#pragma once

#include <cstdint>

namespace mcu::DMA_bits {

struct LISR {
      bool     FEIF0  :1; // Bits 22, 16, 6, 0 FEIFx: Stream x FIFO error interrupt flag (x=3..0)
      uint32_t        :1; // Bits 23, 17, 7, 1 Reserved, must be kept at reset value
      bool     DMEIF0 :1; // Bits 24, 18, 8, 2 DMEIFx: Stream x direct mode error interrupt flag (x=3..0)
      bool     TEIF0  :1; // Bits 25, 19, 9, 3 TEIFx: Stream x transfer error interrupt flag (x=3..0)
      bool     HTIF0  :1; // Bits 26, 20, 10, 4 HTIFx: Stream x half transfer interrupt flag (x=3..0)
      bool     TCIF0  :1; // Bits 27, 21, 11, 5 TCIFx: Stream x transfer complete interrupt flag (x = 3..0)
      bool     FEIF1  :1; 
      uint32_t        :1; 
      bool     DMEIF1 :1;
      bool     TEIF1  :1;
      bool     HTIF1  :1;
      bool     TCIF1  :1;
      uint32_t        :4;
      bool     FEIF2  :1;
      uint32_t        :1;
      bool     DMEIF2 :1;
      bool     TEIF2  :1;
      bool     HTIF2  :1;
      bool     TCIF2  :1;
      bool     FEIF3  :1;
      uint32_t        :1;
      bool     DMEIF3 :1;
      bool     TEIF3  :1;
      bool     HTIF3  :1;
      bool     TCIF3  :1;
      uint32_t        :4;
}__attribute__((packed));

struct HISR {
      bool     FEIF4  :1; // Bits 22, 16, 6, 0 FEIFx: Stream x FIFO error interrupt flag (x=3..0)
      uint32_t        :1; // Bits 23, 17, 7, 1 Reserved, must be kept at reset value
      bool     DMEIF4 :1; // Bits 24, 18, 8, 2 DMEIFx: Stream x direct mode error interrupt flag (x=3..0)
      bool     TEIF4  :1; // Bits 25, 19, 9, 3 TEIFx: Stream x transfer error interrupt flag (x=3..0)
      bool     HTIF4  :1; // Bits 26, 20, 10, 4 HTIFx: Stream x half transfer interrupt flag (x=3..0)
      bool     TCIF4  :1; // Bits 27, 21, 11, 5 TCIFx: Stream x transfer complete interrupt flag (x = 3..0)
      bool     FEIF5  :1; 
      uint32_t        :1; 
      bool     DMEIF5 :1;
      bool     TEIF5  :1;
      bool     HTIF5  :1;
      bool     TCIF5  :1;
      uint32_t        :4;
      bool     FEIF6  :1;
      uint32_t        :1;
      bool     DMEIF6 :1;
      bool     TEIF6  :1;
      bool     HTIF6  :1;
      bool     TCIF6  :1;
      bool     FEIF7  :1;
      uint32_t        :1;
      bool     DMEIF7 :1;
      bool     TEIF7  :1;
      bool     HTIF7  :1;
      bool     TCIF7  :1;
      uint32_t        :4;
}__attribute__((packed));

struct LIFCR {
   bool     CFEIF0  :1;
   uint32_t         :1;
   bool     CDMEIF0 :1;
   bool     CTEIF0  :1;
   bool     CHTIF0  :1;
   bool     CTCIF0  :1;
   bool     CFEIF1  :1;
   uint32_t         :1;
   bool     CDMEIF1 :1;
   bool     CTEIF1  :1;
   bool     CHTIF1  :1;
   bool     CTCIF1  :1;
   uint32_t         :4;
   bool     CFEIF2  :1;
   uint32_t         :1;
   bool     CDMEIF2 :1;
   bool     CTEIF2  :1;
   bool     CHTIF2  :1;
   bool     CTCIF2  :1;
   bool     CFEIF3  :1;
   uint32_t         :1;
   bool     CDMEIF3 :1;
   bool     CTEIF3  :1;
   bool     CHTIF3  :1;
   bool     CTCIF3  :1;
   uint32_t         :4;
}__attribute__((packed));

struct HIFCR {
   bool     CFEIF4  :1;
   uint32_t         :1;
   bool     CDMEIF4 :1;
   bool     CTEIF4  :1;
   bool     CHTIF4  :1;
   bool     CTCIF4  :1;
   bool     CFEIF1  :1;
   uint32_t         :1;
   bool     CDMEIF5 :1;
   bool     CTEIF5  :1;
   bool     CHTIF5  :1;
   bool     CTCIF5  :1;
   uint32_t         :4;
   bool     CFEIF6  :1;
   uint32_t         :1;
   bool     CDMEIF6 :1;
   bool     CTEIF6  :1;
   bool     CHTIF6  :1;
   bool     CTCIF6  :1;
   bool     CFEIF3  :1;
   uint32_t         :1;
   bool     CDMEIF7 :1;
   bool     CTEIF7  :1;
   bool     CHTIF7  :1;
   bool     CTCIF7  :1;
   uint32_t         :4;
}__attribute__((packed));

// struct ISR {
//    bool     FEIF0  :1; // Bits 22, 16, 6, 0 FEIFx: Stream x FIFO error interrupt flag (x=3..0)
//    uint32_t res1   :1; // Bits 23, 17, 7, 1 Reserved, must be kept at reset value
//    bool     DMEIF0 :1; // Bits 24, 18, 8, 2 DMEIFx: Stream x direct mode error interrupt flag (x=3..0)
//    bool     TEIF0  :1; // Bits 25, 19, 9, 3 TEIFx: Stream x transfer error interrupt flag (x=3..0)
//    bool     HTIF0  :1; // Bits 26, 20, 10, 4 HTIFx: Stream x half transfer interrupt flag (x=3..0)
//    bool     TCIF0  :1; // Bits 27, 21, 11, 5 TCIFx: Stream x transfer complete interrupt flag (x = 3..0)
//    bool     FEIF1  :1; 
//    uint32_t res2   :1; 
//    bool     DMEIF1 :1;
//    bool     TEIF1  :1;
//    bool     HTIF1  :1;
//    bool     TCIF1  :1;
//    uint32_t res3   :4;
//    bool     FEIF2  :1;
//    uint32_t res4   :1;
//    bool     DMEIF2 :1;
//    bool     TEIF2  :1;
//    bool     HTIF2  :1;
//    bool     TCIF2  :1;
//    bool     FEIF3  :1;
//    uint32_t res5   :1;
//    bool     DMEIF3 :1;
//    bool     TEIF3  :1;
//    bool     HTIF3  :1;
//    bool     TCIF3  :1;
//    uint32_t res6   :4;
//    bool     FEIF4  :1;
//    uint32_t res7   :1;
//    bool     DMEIF4 :1;
//    bool     TEIF4  :1;
//    bool     HTIF4  :1;
//    bool     TCIF4  :1;
//    bool     FEIF5  :1;
//    uint32_t res8   :1;
//    bool     DMEIF5 :1;
//    bool     TEIF5  :1;
//    bool     HTIF5  :1;
//    bool     TCIF5  :1;
//    uint32_t res9   :4;
//    bool     FEIF6  :1;
//    uint32_t res10  :1;
//    bool     DMEIF6 :1;
//    bool     TEIF6  :1;
//    bool     HTIF6  :1;
//    bool     TCIF6  :1;
//    bool     FEIF7  :1;
//    uint32_t res11  :1;
//    bool     DMEIF7 :1;
//    bool     TEIF7  :1;
//    bool     HTIF7  :1;
//    bool     TCIF7  :1;
//    uint32_t res12  :4;
// }__attribute__((packed));

// struct IFCR {
//    enum Mask {clear = 0b111101};
//    Mask _0  :6;
//    Mask _1  :6;
//    uint32_t :4;
//    Mask _2  :6;
//    Mask _3  :6;
//    uint32_t :4;
//    Mask _4  :6;
//    Mask _5  :6;
//    uint32_t :4;
//    Mask _6  :6;
//    Mask _7  :6;
//    uint32_t :4;
// }__attribute__((packed));

} //namespace mcu::DMA_bits