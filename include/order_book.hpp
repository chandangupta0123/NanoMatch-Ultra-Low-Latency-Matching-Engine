#pragma once
#include "order.hpp"
#include "lockfree_pool.hpp"
#include <vector>
#include <atomic>
#include <cstdint>
#include <algorithm>
#include <limits>

 
constexpr size_t MAX_PRICE = 2000;      
constexpr size_t POOL_SIZE = 1 << 16;   

struct PriceLevel {
    std::vector<Order*> orders;
    std::atomic<uint32_t> total_qty{0};
    PriceLevel() { orders.reserve(8); }
};

class OrderBook {
public:
    OrderBook()
        : pool_(),
          bids_(MAX_PRICE),
          asks_(MAX_PRICE),
          best_bid_(-1),
          best_ask_(static_cast<int>(MAX_PRICE))
    {}


    bool add_order(const Order& o) {
        Order* p = pool_.allocate();
        if (!p) return false;
        *p = o;

        auto& level = (o.side == Side::BUY ? bids_[o.price] : asks_[o.price]);

       
        level.orders.push_back(p);
        level.total_qty.fetch_add(o.quantity, std::memory_order_relaxed);

        update_best_on_add(o);

        return true;
    }


    uint64_t match_best() {
        uint64_t matched = 0;
        int bid = best_bid_.load(std::memory_order_acquire);
        int ask = best_ask_.load(std::memory_order_acquire);
        while (bid >= 0 && ask < static_cast<int>(MAX_PRICE) && bid >= ask) {
            auto& buy_level = bids_[bid];
            auto& sell_level = asks_[ask];

            if (buy_level.total_qty.load(std::memory_order_relaxed) == 0) {
                
                refresh_best_bid_from(bid - 1);
                bid = best_bid_.load(std::memory_order_acquire);
                continue;
            }
            if (sell_level.total_qty.load(std::memory_order_relaxed) == 0) {
                refresh_best_ask_from(ask + 1);
                ask = best_ask_.load(std::memory_order_acquire);
                continue;
            }
 
            if (buy_level.orders.empty()) { refresh_best_bid_from(bid - 1); bid = best_bid_.load(std::memory_order_acquire); continue; }
            if (sell_level.orders.empty()) { refresh_best_ask_from(ask + 1); ask = best_ask_.load(std::memory_order_acquire); continue; }

            Order* b = buy_level.orders.back();
            Order* s = sell_level.orders.back();

            uint32_t qty = std::min(b->quantity, s->quantity);
            b->quantity -= qty;
            s->quantity -= qty;
            buy_level.total_qty.fetch_sub(qty, std::memory_order_relaxed);
            sell_level.total_qty.fetch_sub(qty, std::memory_order_relaxed);
            matched += qty;

            if (b->quantity == 0) {
                buy_level.orders.pop_back();
                pool_.deallocate(b);
            }
            if (s->quantity == 0) {
                sell_level.orders.pop_back();
                pool_.deallocate(s);
            }
        }
        return matched;
    }

    
    int best_bid() const { return best_bid_.load(std::memory_order_acquire); }
    int best_ask() const { return best_ask_.load(std::memory_order_acquire); }
    size_t pool_capacity() const { return LockFreePool<Order, POOL_SIZE>::capacity(); }

private:
    LockFreePool<Order, POOL_SIZE> pool_;
    std::vector<PriceLevel> bids_;
    std::vector<PriceLevel> asks_;
    std::atomic<int> best_bid_;
    std::atomic<int> best_ask_;


    void update_best_on_add(const Order& o) {
        if (o.side == Side::BUY) {
            int prev = best_bid_.load(std::memory_order_relaxed);
            while (o.price > prev) {
                if (best_bid_.compare_exchange_weak(prev, static_cast<int>(o.price),
                                                    std::memory_order_release,
                                                    std::memory_order_relaxed)) break;
               
            }
        } else {
            int prev = best_ask_.load(std::memory_order_relaxed);
            while (o.price < prev) {
                if (best_ask_.compare_exchange_weak(prev, static_cast<int>(o.price),
                                                    std::memory_order_release,
                                                    std::memory_order_relaxed)) break;
            }
        }
    }


    void refresh_best_bid_from(int start) {
        int p = start;
        for (; p >= 0; --p) {
            if (bids_[p].total_qty.load(std::memory_order_relaxed) > 0) {
                best_bid_.store(p, std::memory_order_release);
                return;
            }
        }
        best_bid_.store(-1, std::memory_order_release);
    }
 
    void refresh_best_ask_from(int start) {
        int p = start;
        for (; p < static_cast<int>(MAX_PRICE); ++p) {
            if (asks_[p].total_qty.load(std::memory_order_relaxed) > 0) {
                best_ask_.store(p, std::memory_order_release);
                return;
            }
        }
        best_ask_.store(static_cast<int>(MAX_PRICE), std::memory_order_release);
    }
};
