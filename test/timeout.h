#pragma once

#include <chrono>

class Timeout
{
   std::chrono::time_point<std::chrono::high_resolution_clock> start;
   size_t ms;
public:
   Timeout (size_t ms) : ms {ms}
   {
      restart();
   }

   operator bool()
   {
      return std::chrono::duration_cast<std::chrono::milliseconds> (
         std::chrono::high_resolution_clock::now() - start
      ).count() > ms;
   }

   void restart()
   {
      start = std::chrono::high_resolution_clock::now();
   }
};

