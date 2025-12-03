/**
 * profile.cpp
 * Simple profiling target without benchmark overhead
 */

#include "my_vector.hpp"
#include <vector>
#include <iostream>

int main() {
    std::cout << "Profiling MyVector...\n";
    
    // Test push_back performance
    MyVector<int> v;
    for (int i = 0; i < 1'000'000; ++i) {
        v.push_back(i);
    }
    
    // Prevent optimization
    volatile int sum = 0;
    for (size_t i = 0; i < v.size(); ++i) {
        sum += v[i];
    }
    
    std::cout << "Done. Sum: " << sum << "\n";
    return 0;
}

