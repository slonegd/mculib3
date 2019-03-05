#pragma once

#include "heap.h"

template<class T>
class Dyn_array {
    struct T_wrapper {
        T value;
        inline void* operator new[] (size_t size)
        {
            return heap.operator_multinew (size);
        }
        inline void operator delete[] (void* p)
        {
            return heap.operator_multidelete (p);
        }
    };
    T_wrapper* pointer {nullptr};
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