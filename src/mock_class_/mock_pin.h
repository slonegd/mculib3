# pragma once

#include <vector>
#include "string_result.h"

namespace mcu {

	enum class PinMode {Input, Output, USART1_TX, USART1_RX, USART2_TX, USART2_RX, USART3_TX, USART3_RX};

	struct TX{};
	struct RX{};
	struct RTS{};
	struct LED{};
   struct PA1{};
   struct PA2{};

   class Pin;

   std::vector<Pin*> pins;
   size_t index{0};

class Pin 
{
public:

   bool set{false};
   enum Pins {tx, rx, rts, led, PA1, PA2};
   static int counter;
   Pins pin;
   Pin(){
      ++counter; 
      if      (counter == 1) pin = tx;
      else if (counter == 2) pin = rx;
      else if (counter == 3) pin = rts;
      else if (counter == 4) pin = led;
   }


   bool operator= (bool v) 
   {
      if (pin == tx) {
         result << (v ? "устанавливаем значение пина TX true"   : "устанавливаем значение пина TX false")  << '\n';
      } else if (pin == rx) {
          result << (v ? "устанавливаем значение пина RX true"  : "устанавливаем значение пина RX false")  << '\n';
      } else if (pin == rts){
         result << (v ? "устанавливаем значение пина RTS true"  : "устанавливаем значение пина RTS false") << '\n';
      } else if (pin == led) {
          result << (v ? "устанавливаем значение пина LED true" : "устанавливаем значение пина LED false") << '\n';
      } 
      return v;
   }
   
   operator bool() { return set; }

   template<class Pin_, PinMode mode> static auto& make()
   {  
      static Pin pin; 

      if (std::is_same_v<Pin_, TX> and mode == PinMode::USART1_TX) {
         result << "создали пин TX в альтернативном режиме" << '\n';
      } else if (std::is_same_v<Pin_, RX> and mode == PinMode::USART1_RX) {
         result << "создали пин RX в альтернативном режиме" << '\n';
      } else if (std::is_same_v<Pin_, RTS> and mode == PinMode::Output) {
         result << "создали пин RTS в режиме выхода" << '\n';
      } else if (std::is_same_v<Pin_, LED> and mode == PinMode::Output) {
         result << "создали пин LED в режиме выхода" << '\n';
      }

      pins.push_back(&pin);
      index++;

      return pin;
   }

   template<class Pin_, PinMode mode> static Pin& make_new();

};

int Pin::counter{0};
} // mcu