#pragma once

#include "heap.h"

template<class T>
class Dyn_array {
    Heap_on_stack_allocated<T>* pointer {nullptr};
    size_t size_ {0};
public:
    void add_size (size_t v) {
        if (pointer)
            delete[] (pointer);
        pointer = new T_wrapper[size_ += v];
    }
    auto size() const { return size_; }
    auto address() const { return size_t(pointer); }
};