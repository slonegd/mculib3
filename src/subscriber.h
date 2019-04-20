#pragma once

#include "list.h"
#include "systick.h"

struct Subscriber : Listable<Subscriber> {
    virtual void notify() = 0;
};

// FIX Publisher может быть не только про время,
// перенести быструю версию в TickPublisher
struct Publisher : private List<Subscriber>
{
    
    void clear(){clear_subscribe();}
    void subscribe  (Subscriber& v) { push_back(v); }
    void unsubscribe(Subscriber& v) { remove(v);    }
    template<size_t divider> // how faster
    void subscribe (Subscriber& v) {
        faster.push_back(v);
        faster_cnt = divider;
        REF(SysTick).initInterrupt<1000/divider>();
    }
    // ссылка, чтобы не копировал каждый subscriber из *this
    void notify() {
        for (auto& subscriber : this->faster)
            subscriber.notify();
        if (not(faster_qty++ % faster_cnt))
            for (auto& subscriber : *this)
                subscriber.notify();
    }
private:
    struct Faster : List<Subscriber> {} faster {};
    size_t faster_cnt {1};
    size_t faster_qty {0};
};
