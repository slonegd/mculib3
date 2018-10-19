#pragma once


constexpr uint32_t operator "" _Hz       (unsigned long long val) { return val; }
constexpr uint32_t operator "" _kHz      (unsigned long long val) { return val * 1000; }
constexpr uint32_t operator "" _ms       (unsigned long long val) { return val; }
constexpr uint32_t operator "" _s        (unsigned long long val) { return val * 1000; }
constexpr uint32_t operator "" _min      (unsigned long long val) { return val * 60000; }
constexpr uint32_t operator "" _cnt      (unsigned long long val) { return val; }
constexpr uint32_t operator "" _percent  (unsigned long long val) { return val; }
constexpr uint32_t operator "" _channel  (unsigned long long val) { return val; }
constexpr uint32_t operator "" _from1000 (unsigned long long val) { return val; }
constexpr uint32_t operator "" _page     (unsigned long long val) { return val; }
