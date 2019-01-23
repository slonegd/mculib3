#pragma once

#define USE_MOCK_DELAY
#include "delay.h"
#include "process.h"

namespace mock {

// extern std::ostream* process;

class Delay : ::Delay
{
   Process& process = Process::make();
public:

   bool ms (uint32_t ms) { 
      process << "установка задержки " << ms << " миллисекунд " << std::endl;
      return false;
   }

   bool us (uint32_t us)
   {
      process << "установка задержки " << us << " микросекунд " << std::endl;
      return false;   
   }
};

} //namespce mock 