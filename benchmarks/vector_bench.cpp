/**
 * vector_bench.cpp
 * benchmark comparing MyVector vs std::vector
 */

#include <iostream>
#include <vector>
#define ANKERL_NANOBENCH_IMPLEMENT
#include "my_vector.hpp"
#include <nanobench.h>

namespace detect {
template <typename V>
concept PushBackable = requires(V v, typename V::value_type value) { v.push_back(value); };

template <typename V>
concept Reservable = requires(V v, typename V::size_type n) { v.reserve(n); };
} // namespace detect

constexpr size_t N = 1'000'000;

void benchmark_push_back() {
    ankerl::nanobench::Bench bench;
    bench.title("push_back 1M ints").unit("element").batch(N);

    bench.run("std::vector", [&] {
        std::vector<int> v;
        for (size_t i = 0; i < N; ++i)
            v.push_back(static_cast<int>(i));
        ankerl::nanobench::doNotOptimizeAway(v);
    });

    if constexpr (detect::PushBackable<MyVector<int>>) {
        bench.run("MyVector", [&] {
            MyVector<int> v;
            for (size_t i = 0; i < N; ++i)
                v.push_back(static_cast<int>(i));
            ankerl::nanobench::doNotOptimizeAway(v);
        });
    } else {
        std::cout << "[SKIP] MyVector missing push_back()\n";
    }
}

void benchmark_reserve() {
    ankerl::nanobench::Bench bench;
    bench.title("reserve growth, 1..1M").unit("element").batch(N);

    bench.run("std::vector", [&] {
        std::vector<int> v;
        for (size_t i = 1; i <= N; ++i)
            v.reserve(i);
        ankerl::nanobench::doNotOptimizeAway(v);
    });

    if constexpr (detect::Reservable<MyVector<int>>) {
        bench.run("MyVector", [&] {
            MyVector<int> v;
            for (size_t i = 1; i <= N; ++i)
                v.reserve(i);
            ankerl::nanobench::doNotOptimizeAway(v);
        });
    } else {
        std::cout << "[SKIP] MyVector missing reserve()\n";
    }
}

int main() {
    std::cout << "=== MyVector Benchmark ===\n\n";
    benchmark_push_back();
    benchmark_reserve();
    std::cout << "\nDone!\n";
    return 0;
}
