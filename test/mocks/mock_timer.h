#pragma once

#define USE_MOCK_TIMER
#include "mock_systick.h"
#include "timers.h"
#include <iostream>

namespace mock {

class Timer : public ::Timer
{
public:
   Timer () = default;
   Timer (uint32_t ms) : ::Timer {ms} {}
   bool event(){return true;}
   // void stop() {}
};

} //namespce mock 