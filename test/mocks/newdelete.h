#pragma once

#include <stdint.h>
#include <cstddef>
#include <new>
#include <stdio.h>

namespace mock {

    std::size_t alloc_counter = 0;

    void* malloc(std::size_t size)  //throw (std::bad_alloc)
    {
        void* p = std::malloc(size);
        printf("malloc: %zu %p %zu\n", ++alloc_counter, p, size);
        return p;
    }

    void free(void* p) noexcept
    {
        printf("free: %zu %p\n", --alloc_counter, p);
        std::free(p);
        return;
    }
}


extern "C++" {

    inline void* operator new(std::size_t size) _GLIBCXX_THROW (std::bad_alloc)
    {
        return mock::malloc(size);
    }

    inline void operator delete(void* p) noexcept
    {
        mock::free(p);
    }

    inline void* operator new[](std::size_t size) _GLIBCXX_THROW (std::bad_alloc)
    {
        return mock::malloc(size);
    }

    inline void operator delete[](void* p) throw()
    {
        mock::free(p);
    }

    inline void* operator new(std::size_t size, const std::nothrow_t&) noexcept
    {
        return mock::malloc(size);
    }

    inline void operator delete(void* p, const std::nothrow_t&) noexcept
    {
        mock::free(p);
    }

    inline void* operator new[](std::size_t size, const std::nothrow_t&) noexcept
    {
        return mock::malloc(size);
    }

    inline void operator delete[](void* p, const std::nothrow_t&) noexcept
    {
        mock::free(p);
    }

    inline void operator delete(void* p, long unsigned int)
    {
        mock::free(p);
    }

    inline void operator delete [](void* p, long unsigned int)
    {
        mock::free(p);
    }

} // extern "C++"
