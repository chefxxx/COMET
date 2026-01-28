
# 🚀 COMET - Combinatorial Enumeration Toolkit

**COMET** (COmbinatorial Enumeration Toolkit) is a generic, header-only **C++20** library designed for the efficient generation of various types of combinations from large and diverse data sources.

Originally developed to address specific requirements of the **ALICE experiment at CERN** (specifically for "event-mixing" techniques used in combinatorial background calculation), COMET provides a flexible, zero-copy architecture.

## 🌟 Key Features

*   **Header-Only & Generic**: Easy integration into existing C++ projects; works with any container satisfying the `std::random_access_iterator` concept.
*   **Zero-Copy Overhead**: Uses iterators and `std::span` views to manipulate data without copying elements, keeping memory usage minimal even for heavy data structures.
*   **Flexible Combination Strategies**:
    *   **Basic Combinations**: Cartesian products (Full) and unique subsets (Strictly Upper).
    *   **Block Combinations**: Partition data into logical "buckets" (based on value ranges) and generate combinations only within valid buckets.
    *   **Grouped Combinations**: Handle hierarchical data (e.g., Events containing Particles) by iterating over grouping elements and providing access to their associated data subsets.
    *   **Sliding Window**: Restrict combinations to elements within a specific range.
*   **Policy-Based Design**: Highly customizable behavior via template policies.

## 🔎 Requirements

*   **C++ Standard**: C++20 or later.
*   **Compiler**: GCC 11+, Clang 14+, or AppleClang (supporting C++20).
*   **CMake**: Version 3.30 or higher.

**External Dependencies (fetched automatically via CMake):**
*   Google Test (for unit tests)
*   Google Benchmark (for performance benchmarks)

## 🔜 Integration

Since COMET is header-only, integration is straightforward.

### Option 1: Copy the headers
Simply copy the contents of the `lib/` directory into your project's include path.

### Option 2: CMake `add_subdirectory`
If you include this repository as a submodule:

```cmake
add_subdirectory(COMET)
target_link_libraries(your_target PRIVATE COMET_lib)
```

After that you only need to add the ```#include "comet.h"``` into the file where you want to use the library.

## ✅ Usage

The library provides factory functions (e.g., `makeCombinations`, `makeBlockCombinations`) that return producer objects. These producers are iterable and compatible with C++ range-based for loops.

**👉 Comprehensive usage examples can be found in the [`examples/`](examples/) directory.**

### 1. Basic Combinations
Generate simple pairs or n-tuples.

```cpp
#include "comet.h"
#include <vector>
#include <iostream>

int main() {
    std::vector<int> data = {1, 2, 3};
    std::vector<char> chars = {'a', 'b'};

    // Cartesian Product (Full Policy)
    auto prod = makeCombinations<FullCombinationsPolicy>(data, chars);
    
    for (const auto& [num, ch] : prod) {
        std::cout << num << "-" << ch << "\n";
    }
    // Output: 1-a, 1-b, 2-a, 2-b, 3-a, 3-b
}
```

### 2. Block Combinations (Bucketing)
Divide data into buckets based on properties (e.g., physical position, momentum) and combine only compatible elements.

```cpp
#include "comet.h"

// User-defined logic to extract value for bucketing
struct GetVal { 
    double operator()(const MyStruct& s) const { return s.value; } 
};

int main() {
    // Define bucket boundaries: [0.0, 10.0), [10.0, 20.0)
    std::array<double, 3> bounds = {0.0, 10.0, 20.0};
    
    // Create Policy: ignore overflows (values outside ranges) = false
    // Combinations are alse created within (-inf, 0.0) and [20.0, inf)
    auto bucketPolicy = BucketPolicy(false, GetVal{}, bounds);

    // Create combinations only within the same bucket
    auto producer = makeBlockCombinations<StrictlyUpperCombinationsPolicy>(
        bucketPolicy, 
        1, // Minimum elements in a bucket to consider it
        data1, data2
    );

    for (const auto& [a, b] : producer) {
        // Process a and b (guaranteed to be in the same bucket)
    }
}
```

### 3. Grouped Combinations
Iterate over a "Grouping" source (e.g., Events) and access "Associated" data (e.g., Particles) belonging to that group.

```cpp
#include "comet.h"

int main() {
    // Wrappers to define how to link Grouping IDs to Associated IDs
    auto groupWrap = SourceWithCallable(events, [](const Event& e){ return e.id; });
    auto assocWrap = SourceWithCallable(tracks, [](const Track& t){ return t.eventId; });

    auto groupedProd = makeGroupedCombinations<FullCombinationsPolicy>(
        bucketPolicy, 1, groupWrap, assocWrap, assocWrap
    );

    for (const auto& [event0, trackSpan1, event2, trackSpan2] : groupedProd) {
        // 'event' is the grouping element
        // 'tracksSpan' is a SpanView of tracks belonging to this event
        
        // You can run combinations *inside* this loop on the span
        auto innerComb = makeCombinations<StrictlyUpperCombinationsPolicy>(tracksSpan1, tracksSpan2);
        // ...
    }
}
```

## 🛠️ Building Tests and Benchmarks

In order to build the tests and benchmarks you have to run the following commands.
If you want to clear previous build:

```bash
rm -rf build 
```

Then to build the project:

```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCOMET_BUILD_TESTS=ON ..
cmake --build .
```

To run the tests paste this command: 

```bash
./build/tests/COMET_test
```

To run the benchmarks paste one of the followings commands - each for different kind of benchmark:

```bash
./build/benchmarks/combinationsFull_benchmark
```

There are several benchmarks that can be run:

*   ```blockCombinations_benchmark```
*   ```combinationsFull_benchmark```
*   ```combinationsStrictlyUpper_benchmark```
*   ```combinationsWindowedStrictlyUpper_benchmark```
*   ```discretureVsCometFullCombinations_benchmark```
*   ```discretureVsCometStrictlyUpper_benchmark```
*   ```groupedCombinations_benchmark```
*   ```stdVsCometFullCombinations_benchmark```

## 📊 Performance

COMET is optimized for high-performance computing environments. Benchmarks (included in `benchmarks/`) demonstrate that:
1.  **Memory Usage**: Remains constant and low (allocating only iterators) regardless of the size of the underlying data structures ("Heavy" vs "Light" structures).
2.  **Execution Time**: Comparable to raw, hand-written nested loops, often standing an equal match to the standard library solutions like `std::ranges::views::cartesian_product` in specific compiler configurations or offering functionality not present in the standard library (Block/Grouped combinations).

## 🏗 Project Structure

```text
.
├── CMakeLists.txt   # Unified build configuration.
├── dependencies/    # Project dependencies folder.
├── scripts/         # Scripts folder, formatting etc.
├── lib/             # Core header-only library source code.
├── examples/        # Compilation-ready examples demonstrating basic, block, and grouped combinations.
├── tests/           # Google Test unit tests covering various policies and edge cases.
├── benchmarks/      # Performance comparisons against loops and other libraries.
└── LICENSE          # Full LICENSE text.
```

## 📑 License

This project is licensed under GPL-3.0 license. See the [LICENSE](LICENSE) file for the full license text.