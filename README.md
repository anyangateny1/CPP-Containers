# C++ Container Benchmarks

Benchmark and profile my custom containers against STL using [nanobench](https://github.com/martinus/nanobench), perf, valgrind and stat.

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
├── profile/                    # Profiling with perf/valgrind
└── CMakeLists.txt              # Build config
```

## Profiling

```bash
cd profile
make              # build
make stat         # perf stat
make cachegrind   # valgrind cachegrind
```
