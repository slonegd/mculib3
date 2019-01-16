#pragma once

#include <iostream>
#include <chrono>
#include <thread>

class Stopwatch 
{
   std::chrono::time_point<std::chrono::high_resolution_clock> current_time, last_time;

public:

   Stopwatch() 
   {
      last_time = std::chrono::high_resolution_clock::now();
   }

   operator size_t () 
   {
     current_time = std::chrono::high_resolution_clock::now();
     auto duration = std::chrono::duration_cast <std::chrono::microseconds> (current_time - last_time);
     last_time = std::chrono::high_resolution_clock::now();
     return duration.count();
   }