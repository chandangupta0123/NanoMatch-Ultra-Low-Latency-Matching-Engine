NanoMatch-Ultra-Low-Latency-Matching-Engine

Description:
An ultra-low-latency C++ Matching Engine designed for high-frequency trading. It features:

Lock-free SPSC queue for fast inter-thread communication.

Cache-optimized order book for minimal memory access latency.

Custom memory pool to avoid dynamic allocation during runtime.

p99 latency instrumentation to monitor performance.

Deterministic performance under high load, achieving nanosecond-level throughput.

This engine is designed for real-time financial systems and low-latency trading simulations.

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

C++17 or later compiler (GCC/Clang/MSVC).

CMake ≥ 3.15.

Linux or Windows (Windows requires Visual Studio with CMake support).

Optional: make for Linux/macOS.

Build Instructions

Open a terminal or PowerShell.

Navigate to the project folder:

cd NanoMatch-Ultra-Low-Latency-Matching-Engine


Create a build directory:

mkdir build
cd build


Generate build files with CMake:

cmake ..


Build the project:

On Linux/macOS:

make -j


On Windows (with Visual Studio):

cmake --build . --config Release


The executable will be generated (e.g., ./hft_engine on Linux/macOS or hft_engine.exe on Windows).

Run the Engine
./hft_engine


The engine prints periodic latency stats.

Press Ctrl+C to stop the program.

Features

Lock-free SPSC Queue: High-throughput, minimal contention.

Memory Pool: No dynamic allocation during runtime.

Cache-optimized Order Book: Reduces cache misses for faster processing.

p99 Latency Tracker: Monitors 99th percentile latency for critical operations.

Example Output
Throughput: 1,200,000 ops/sec
p99 Latency: 350 ns
Average Latency: 200 ns

Contributing

Fork the repository.

Create a new branch for your feature/fix:

git checkout -b feature/my-feature


Commit your changes and push:

git commit -am "Add feature XYZ"
git push origin feature/my-feature


Open a pull request.
