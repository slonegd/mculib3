#pragma once

#include "heap.h"

template<class T>
class Dyn_array {
    using type = Heap_on_stack_allocated<T>;
    type* pointer {nullptr};
    size_t size_ {0};
public:
    void add_size (size_t v) {
        if (pointer)
            delete[] (pointer);
        pointer = new type[size_ += v];
    }
    auto size() const { return size_; }
    auto address() const { return size_t(pointer); }
    auto begin() { return pointer; }
    auto end()   { return pointer + size_; }
    void clear() 
    {
        if (pointer)
            delete[] (pointer);
        pointer = nullptr;
        size_ = 0;
    }
};