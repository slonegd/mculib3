#pragma once

#define USE_MOCK_DELAY
#include "delay.h"
#include <iostream>

namespace mock {

// extern std::ostream* process;

class Delay : ::Delay
{

public:

   bool ms (uint32_t ms) { 
      if (process) *process << "установка задержки " << ms << " миллисекунд " << std::endl;
      return false;
   }

   bool us (uint32_t us)
   {
      if (process) *process << "установка задержки " << us << " микросекунд " << std::endl;
      return false;   
   }
};

} //namespce mock 