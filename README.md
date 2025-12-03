# C++ Container Benchmarks

Benchmark custom containers against STL using [nanobench](https://github.com/martinus/nanobench).

## Quick Start

```bash
# Build
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Run
./build/bin/vector_bench
```

## Project Structure

```
cpp-containers/
├── containers/my_vector.hpp    # custom container
├── benchmarks/vector_bench.cpp # Benchmark code
└── CMakeLists.txt              # Build config
```

