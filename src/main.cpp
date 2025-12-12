#include "../include/order.hpp"
#include "../include/spsc_ring.hpp"
#include "../include/order_book.hpp"
#include "../include/latency_tracker.hpp"

#include <thread>
#include <chrono>
#include <atomic>
#include <iostream>
#include <vector>
#include <csignal>

static constexpr size_t RING_SIZE = 1 << 12;
static SPSCRing<Order, RING_SIZE> ring;
static OrderBook book;
static LatencyTracker lifecycle_tracker;
static std::atomic<bool> running{true};

uint64_t now_ns() {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(
               std::chrono::steady_clock::now().time_since_epoch())
        .count();
}

void producer_thread(uint64_t start_id, uint64_t rate_per_sec) {
    uint64_t id = start_id;
    using namespace std::chrono;
    auto next = high_resolution_clock::now();
    nanoseconds inter( (1000000000ULL) / std::max<uint64_t>(1, rate_per_sec) );
    while (running.load(std::memory_order_relaxed)) {
        Order o;
        o.order_id = id++;
        o.price = 900 + static_cast<int>(id % 200); 
        o.quantity = 1 + (id % 5);
        o.side = (id % 2 ? Side::BUY : Side::SELL);
        o.created_ns = now_ns();

      
        while (!ring.push(o)) { 
            std::this_thread::yield();
        }

        next += inter;
        std::this_thread::sleep_until(next);
    }
}

void consumer_thread() {
    Order o;
    const size_t report_interval = 200000; 
    size_t counter = 0;
    while (running.load(std::memory_order_relaxed)) { 
        while (ring.pop(o)) { 
            bool added = book.add_order(o);
            (void)added; 
 
            uint64_t matched = book.match_best();
            (void)matched;

          
            uint64_t t_end = now_ns();
            lifecycle_tracker.record(t_end - o.created_ns);

            ++counter;
            if ((counter % report_interval) == 0) {
                std::cout << "Processed " << counter
                          << " orders. Pool capacity: " << book.pool_capacity()
                          << " BestBid=" << book.best_bid()
                          << " BestAsk=" << book.best_ask()
                          << "\n";
                lifecycle_tracker.snapshot_and_print("snapshot");
            }
        } 
        std::this_thread::sleep_for(std::chrono::microseconds(50));
    }
}

void sigint_handler(int) {
    running.store(false);
}

int main() {
    std::signal(SIGINT, sigint_handler);
    std::cout << "Starting engine demo. MAX_PRICE=" << MAX_PRICE << "\n";

   
    std::thread prod1(producer_thread, 1ULL, 50000ULL);  
    std::thread cons(consumer_thread);

    prod1.join();
    cons.join();

   
    lifecycle_tracker.snapshot_and_print("final");
    std::cout << "Exiting.\n";
    return 0;
}
