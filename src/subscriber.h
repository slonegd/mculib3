#pragma once

#pragma once

#include "list.h"

struct Subscriber : Listable<Subscriber> {
   virtual void notify() = 0;
};

struct Publisher : private List<Subscriber>
{
   void subscribe  (Subscriber* p) { push_back(p); }
   void unsubscribe(Subscriber* p) { remove(p);    }
   // ссылка, чтобы не копировал каждый subscriber из *this
   void notify() {
      for (auto& subscriber : *this)
         subscriber.notify();
      }
};
