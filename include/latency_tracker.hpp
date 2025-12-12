#pragma once
#include <vector>
#include <cstdint>
#include <algorithm>
#include <iostream>
#include <mutex>
 
class LatencyTracker {
public:
    LatencyTracker() {}
    void record(uint64_t ns) {
        samples_.push_back(ns);
    }

    void snapshot_and_print(const char* tag = "") {
        if (samples_.empty()) return;
        std::vector<uint64_t> tmp;
        tmp.swap(samples_); // clear original quickly
        std::sort(tmp.begin(), tmp.end());
        size_t n = tmp.size();
        uint64_t p50 = tmp[ std::max<size_t>(1, (n*50)/100) - 1 ];
        uint64_t p95 = tmp[ std::max<size_t>(1, (n*95)/100) - 1 ];
        uint64_t p99 = tmp[ std::max<size_t>(1, (n*99)/100) - 1 ];
        uint64_t p999 = tmp[ std::max<size_t>(1, (n*999)/1000) - 1 ];
        double avg = 0;
        for (auto v : tmp) avg += double(v);
        avg /= double(n);
        std::cout << "[Latency " << tag << "] count=" << n
                  << " avg=" << uint64_t(avg)
                  << " p50=" << p50
                  << " p95=" << p95
                  << " p99=" << p99
                  << " p99.9=" << p999
                  << " ns\n";
    }

private:
    std::vector<uint64_t> samples_;
};
