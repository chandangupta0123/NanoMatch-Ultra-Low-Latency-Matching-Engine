NanoMatch-Ultra-Low-Latency-Matching-Engine

An ultra-low-latency C++ Matching Engine designed for high-frequency trading.

This engine features:

Lock-free SPSC queue for fast inter-thread communication

Cache-optimized order book for minimal memory access latency

Custom memory pool to avoid dynamic allocation during runtime

p99 latency instrumentation to monitor performance

Deterministic performance under high load, achieving nanosecond-level throughput

Built for real-time financial systems and low-latency trading simulations.

Project Structure
NanoMatch-Ultra-Low-Latency-Matching-Engine/
├─ include/
│  ├─ order.hpp
│  ├─ spsc_ring.hpp
│  ├─ lockfree_pool.hpp
│  ├─ latency_tracker.hpp
│  └─ order_book.hpp
├─ src/
│  └─ main.cpp
└─ CMakeLists.txt

Prerequisites

C++17 or later (GCC, Clang, or MSVC)

CMake ≥ 3.15

Linux or Windows (Windows requires Visual Studio with CMake support)

Optional: make for Linux/macOS

Build Instructions

Open a terminal or PowerShell

Navigate to the project folder:

cd NanoMatch-Ultra-Low-Latency-Matching-Engine


Create a build directory:

mkdir build
cd build


Generate build files with CMake:

cmake ..


Build the project:

Linux/macOS:

make -j


Windows (Visual Studio):

cmake --build . --config Release


The executable will be generated as:

./hft_engine on Linux/macOS

hft_engine.exe on Windows

Running the Engine
./hft_engine


Prints periodic latency stats

Press Ctrl+C to stop the program

Features

Lock-free SPSC Queue: High-throughput, minimal contention

Memory Pool: Avoids dynamic memory allocation at runtime

Cache-optimized Order Book: Minimizes cache misses for faster processing

p99 Latency Tracker: Monitors 99th percentile latency

Example Output
Throughput: 1,200,000 ops/sec
p99 Latency: 350 ns
Average Latency: 200 ns

Contributing

Fork the repository

Create a new branch for your feature/fix:

git checkout -b feature/my-feature


Commit and push your changes:

git commit -am "Add feature XYZ"
git push origin feature/my-feature


Open a Pull Request
