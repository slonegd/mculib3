#define STM32F746xx
#define F_CPU   216'000'000UL

#include "rcc.h"




/// эта функция вызываеться первой в startup файле
extern "C" void init_clock()
{

}


int main()
{
   mcu::make_reference<mcu::Periph::RCC>()
      .clock_enable<mcu::Periph::GPIOA>()
      .clock_enable<mcu::Periph::USART8>();
   
   while (1) {
      
   } // while(1) {

}