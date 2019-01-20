#pragma once

#include <sstream>
#include <memory>

namespace mock {

class Process {
   Process() = default;
public:
   std::unique_ptr<std::stringstream> stream = std::make_unique<std::stringstream>();

   static auto& make()
   {
      static Process process{};
      return process;
   }

   // void set_stream (std::stringstream& v) { stream = &v; }

   
   template<class T> Process& operator<< (const T& v)
   {
      if (stream) *stream << v;
      return *this;
   }

   Process& operator<< (std::ostream&(&f)(std::ostream&))
   {
      if (stream) *stream << f;
      return *this;
   }

   void clear(){ if (stream) stream->str(std::string{}); }

   auto str() { return stream ? stream->str() : std::string{}; }
};

} // namespace mock {