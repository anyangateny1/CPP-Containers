/**
 * vector_bench.cpp
 * benchmark comparing MyVector vs std::vector
 */

#include <iostream>
#include <vector>

#define ANKERL_NANOBENCH_IMPLEMENT
#include <nanobench.h>

#include "my_vector.hpp"


// detect using SFINAE to detect if metthods are completed.
namespace detect {

template <typename T, typename E = int, typename = void> struct has_push_back : std::false_type {};

template <typename T, typename E>
struct has_push_back<T, E, std::void_t<decltype(std::declval<T>().push_back(std::declval<E>()))>>
    : std::true_type {};

} // namespace detect

// Benchmark

void benchmark_push_back() {
    constexpr size_t N = 1'000'000;

    // Check if MyVector has push_back
    if constexpr (!detect::has_push_back<MyVector<int>, int>::value) {
        std::cout << "[SKIP] MyVector missing push_back() - implement it first!\n";
        return;
    }

    ankerl::nanobench::Bench bench;
    bench.title("push_back 1M ints").unit("element").batch(N);

    bench.run("std::vector", [&] {
        std::vector<int> v;
        for (size_t i = 0; i < N; ++i) {
            v.push_back(static_cast<int>(i));
        }
        ankerl::nanobench::doNotOptimizeAway(v);
    });

    bench.run("MyVector", [&] {
        MyVector<int> v;
        for (size_t i = 0; i < N; ++i) {
            v.push_back(static_cast<int>(i));
        }
        ankerl::nanobench::doNotOptimizeAway(v);
    });
}

int main() {
    std::cout << "=== MyVector Benchmark ===\n\n";

    benchmark_push_back();

    std::cout << "\nDone!\n";
    return 0;
}
