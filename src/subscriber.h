#pragma once

#pragma once

#include "list.h"

struct Subscriber : Listable<Subscriber> {
   virtual void notify() = 0;
};

struct Publisher : private List<Subscriber>
{
   void clear(){clear_subscribe();}
   void subscribe  (Subscriber& v) { push_back(v); }
   void unsubscribe(Subscriber& v) { remove(v);    }
   // ссылка, чтобы не копировал каждый subscriber из *this
   void notify() {
      for (auto& subscriber : *this)
         subscriber.notify();
      }
};
