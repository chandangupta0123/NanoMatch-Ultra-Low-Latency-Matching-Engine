#pragma once
#include <cstdint>
#include <atomic>

enum class Side : uint8_t { BUY = 0, SELL = 1 };

struct alignas(64) Order {
    uint64_t order_id;
    uint32_t price;     
    uint32_t quantity;
    Side side;
    uint64_t created_ns;    
};
