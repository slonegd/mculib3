#pragma once

template<class T>
class Hysteresis
{
public:
   
   bool over(T temp, T max)
   {
      return (temp < max);
   }

   bool recovery(T temp, T min)
   {
      return (temp <= min);
   }

};