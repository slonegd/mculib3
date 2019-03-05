#pragma once

#include "heap.h"

template<class T>
class Dyn_array {
    T* pointer {nullptr};
    size_t size_ {0};
public:
    void add_size (size_t v) {
        if (not pointer)
            delete[] pointer;
        pointer = new uint16_t[size_ += v];
    }
    auto size() const { return size_; }
    auto address() const { return size_t(pointer); }
};