#include <iostream>
#include <chrono>
#include <thread>

struct Tick {
    volatile size_t cnt {0};
    volatile size_t delay;
    size_t delay_cnt;
    void operator() ()
    {
        while (1) {
            // std::chrono::high_resolution_clock::now();
            delay = delay_cnt;
            while (--delay) {}
            // std::chrono::high_resolution_clock::now();
            cnt++;
        }
    }
    Tick()
    {
        auto begin = std::chrono::high_resolution_clock::now();
        delay = 1'000'000'000;
        while (--delay) {}
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast <std::chrono::microseconds> (end - begin);
        delay_cnt = 1'000'000'000 / duration.count();
    }
     
};

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
     auto lt = last_time;
     last_time = current_time = std::chrono::high_resolution_clock::now();
     auto duration = std::chrono::duration_cast <std::chrono::microseconds> (current_time - lt);
     
     return duration.count();
   }
};

int main ()
{
    /*Stopwatch time_passed;
    volatile size_t delay {1000000};
    auto begin = std::chrono::high_resolution_clock::now();
    while (--delay) {}
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast <std::chrono::microseconds> (end - begin);
    std::cout << duration.count() << std::endl;*/
    
    
    Tick tick;
    Stopwatch time_passed;
    std::thread { [&]{tick();} }.detach();
    while (tick.cnt < 1000) {}

    size_t passed = time_passed;
    std::cout << passed << std::endl;
    

}