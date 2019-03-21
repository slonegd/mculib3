#pragma once

#include <cstdint>

namespace mcu::DMA_bits {

struct ISR {
   bool GIF1  :1; // Bits 16, 12, 8, 4, 0 GIFx: Channel x global interrupt flag (x = 1..5)
   bool TCIF1 :1; // Bits 17, 13, 9, 5, 1 TCIFx: Channel x transfer complete flag (x = 1..5)
   bool HTIF1 :1; // Bits 18, 14, 10, 6, 2 HTIFx: Channel x half transfer flag (x = 1..5)
   bool TEIF1 :1; // Bits 19, 15, 11, 7, 3 TEIFx: Channel x transfer error flag (x = 1..5)
   bool GIF2  :1;
   bool TCIF2 :1;
   bool HTIF2 :1;
   bool TEIF2 :1;
   bool GIF3  :1;
   bool TCIF3 :1;
   bool HTIF3 :1;
   bool TEIF3 :1;
   bool GIF4  :1;
   bool TCIF4 :1;
   bool HTIF4 :1;
   bool TEIF4 :1;
   bool GIF5  :1;
   bool TCIF5 :1;
   bool HTIF5 :1;
   bool TEIF5 :1;
   uint32_t   :12;
}__attribute__((packed));

struct IFCR {
   bool CGIF1  :1; // Bits 24, 20, 16, 12, 8, 4, 0 CGIFx: Channel x global interrupt clear (x = 1 ..7)
   bool CTCIF1 :1; // Bits 25, 21, 17, 13, 9, 5, 1 CTCIFx: Channel x transfer complete clear (x = 1 ..7)
   bool CHTIF1 :1; // Bits 26, 22, 18, 14, 10, 6, 2 CHTIFx: Channel x half transfer clear (x = 1 ..7)
   bool CTEIF1 :1; // Bits 27, 23, 19, 15, 11, 7, 3 CTEIFx: Channel x transfer error clear (x = 1 ..7)
   bool CGIF2  :1;
   bool CTCIF2 :1;
   bool CHTIF2 :1;
   bool CTEIF2 :1;
   bool CGIF3  :1;
   bool CTCIF3 :1;
   bool CHTIF3 :1;
   bool CTEIF3 :1;
   bool CGIF4  :1;
   bool CTCIF4 :1;
   bool CHTIF4 :1;
   bool CTEIF4 :1;
   bool CGIF5  :1;
   bool CTCIF5 :1;
   bool CHTIF5 :1;
   bool CTEIF5 :1;
   uint32_t    :12; // Bits 31:28 Reserved, must be kept at reset value.
}__attribute__((packed));

} //namespace mcu::DMA_bits