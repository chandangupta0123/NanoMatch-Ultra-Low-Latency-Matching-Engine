#pragma once
#include <atomic>
#include <cstdint>
#include <type_traits>

template<typename T, size_t N>
class SPSCRing {
    static_assert((N & (N - 1)) == 0, "N must be a power of two");
public:
    SPSCRing() : head_(0), tail_(0) {}

    bool push(const T& item) {
        const uint64_t head = head_.load(std::memory_order_relaxed);
        const uint64_t next = (head + 1) & mask_;
        if (next == tail_.load(std::memory_order_acquire)) return false; // full
        buffer_[head] = item;
        head_.store(next, std::memory_order_release);
        return true;
    }

    bool pop(T& out) {
        const uint64_t tail = tail_.load(std::memory_order_relaxed);
        if (tail == head_.load(std::memory_order_acquire)) return false; // empty
        out = buffer_[tail];
        tail_.store((tail + 1) & mask_, std::memory_order_release);
        return true;
    }

private:
    static constexpr uint64_t mask_ = N - 1;
    alignas(64) T buffer_[N];
    alignas(64) std::atomic<uint64_t> head_;
    alignas(64) std::atomic<uint64_t> tail_;
};
