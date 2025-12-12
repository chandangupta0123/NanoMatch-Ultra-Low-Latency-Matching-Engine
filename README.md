# 🚀 NanoMatch: Ultra-Low-Latency Matching Engine

An ultra-low-latency C++ Matching Engine designed for high-frequency trading (HFT). This prototype emphasizes deterministic performance, memory efficiency, and advanced concurrency techniques.

## ✨ Key Features & Performance

This engine is built on fundamental low-latency principles, ensuring predictable, nanosecond-level throughput:

| Feature | Low-Latency Technique | Impact |
| :--- | :--- | :--- |
| **Lock-Free SPSC Queue** | `std::atomic` with explicit memory ordering | High-throughput, minimal contention inter-thread communication. |
| **Custom Memory Pool** | CAS-based Lock-Free Stack (MPMC-Safe) | Avoids non-deterministic heap allocation (`malloc`/`new`) during runtime. |
| **Cache-Optimized Order Book** | Price-Level Arrays with `alignas(64)` | Minimizes cache misses; provides fast $\mathbf{O(1)}$ Best Bid/Offer (BBO) lookup. |
| **p99 Latency Instrumentation** | Integrated `LatencyTracker` | Monitors and reports **tail latency** (p95, p99, p99.9) for the full order lifecycle. |

## 📁 Project Structure


## ⚙️ Prerequisites

* **C++ Compiler:** C++17 or later (GCC 10+, Clang 10+, or MSVC).
* **Build System:** CMake $\ge$ 3.15.

## 🏗️ Build Instructions

Follow these steps from the root directory of the repository:

1.  **Navigate to the project folder:**
    ```bash
    cd NanoMatch-Ultra-Low-Latency-Matching-Engine
    ```
2.  **Create and enter the build directory:**
    ```bash
    mkdir build
    cd build
    ```
3.  **Generate build files with CMake (includes high optimization flags):**
    ```bash
    cmake ..
    ```
4.  **Build the project:**
    * **Linux/macOS:**
        ```bash
        make -j
        ```
    * **Windows (Visual Studio):**
        ```bash
        cmake --build . --config Release
        ```

## ▶️ Run the Engine

Execute the generated binary:

| OS | Command |
| :--- | :--- |
| **Linux/macOS** | `./hft_engine` |
| **Windows** | `.\Release\hft_engine.exe` (or similar path) |

The engine runs a simulated workload, printing periodic latency statistics. Press **`Ctrl+C`** to gracefully stop the execution and print the final latency report.

### 📊 Example Output

The output demonstrates the engine's ability to maintain low tail latency under continuous load:


## 🤝 Contributing

Contributions are welcome! Please follow the standard GitHub workflow:

1.  **Fork** the repository.
2.  **Create a new branch:**
    ```bash
    git checkout -b feature/my-new-optimization
    ```
3.  **Commit and push** your changes:
    ```bash
    git commit -am "Feat: Implement MPMC support in SPSC ring" 
    git push origin feature/my-new-optimization
    ```
4.  Open a **Pull Request** explaining your changes and performance impact.

---
