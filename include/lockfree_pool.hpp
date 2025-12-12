#pragma once
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <cassert>
#include <type_traits> 


template <typename T, size_t N>
class LockFreePool {
    static_assert(N > 0, "Pool size must be > 0");
public:
    LockFreePool() {
        // initialize next indices
        for (size_t i = 0; i < N - 1; ++i) {
            next_[i].store(i + 1, std::memory_order_relaxed);
        }
        next_[N - 1].store(INVALID_INDEX, std::memory_order_relaxed);
        head_.store(0, std::memory_order_release);
    }
 
    T* allocate() {
        size_t old_head = head_.load(std::memory_order_acquire);
        while (old_head != INVALID_INDEX) {
            size_t next = next_[old_head].load(std::memory_order_relaxed);
            if (head_.compare_exchange_weak(old_head, next,
                                            std::memory_order_acq_rel,
                                            std::memory_order_relaxed)) {
                return &storage_[old_head];
            } 
        }
        return nullptr;
    }
 
    void deallocate(T* ptr) {
        assert(ptr >= storage_ && ptr < storage_ + N);
        size_t idx = static_cast<size_t>(ptr - storage_);
        size_t old_head = head_.load(std::memory_order_relaxed);
        while (true) {
            next_[idx].store(old_head, std::memory_order_relaxed);
            if (head_.compare_exchange_weak(old_head, idx,
                                            std::memory_order_release,
                                            std::memory_order_relaxed)) {
                return;
            } 
        }
    }
 
    static constexpr size_t capacity() { return N; }

private:
    static constexpr size_t INVALID_INDEX = static_cast<size_t>(-1);

    alignas(64) T storage_[N];
    alignas(64) std::atomic<size_t> next_[N];
    alignas(64) std::atomic<size_t> head_;
};
