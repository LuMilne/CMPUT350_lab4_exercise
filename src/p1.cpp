#include <vector>
#include <random>
#include <iostream>
#include "timer.h"

#define M_SIZE 4000

int main() {
    // Initialize matrices as 1D vectors to preserve contiguous state
    auto rm_array = std::vector<std::uint64_t>(M_SIZE*M_SIZE);
    auto cm_array = std::vector<std::uint64_t>(M_SIZE*M_SIZE);

    // Controlled seed for rng
    std::mt19937_64 rng(0);

    // -----
    // POPULATION
    // -----

    // Row-major population
    for(size_t i = 0; i < M_SIZE; i++) {
        for(size_t j = 0; j < M_SIZE; j++) {
            rm_array[M_SIZE*i + j] = rng();   // Populate
        }
    }

    // reseed generator to trigger same values
    rng.seed(0);

    // Col-major population
    for(size_t i = 0; i < M_SIZE; i++) {
        for(size_t j = 0; j < M_SIZE; j++) {
            cm_array[i + M_SIZE*j] = rng();   // Populate
        }
    }

    // -----
    // SUM
    // -----
    uint64_t rm_sum = 0;
    uint64_t cm_sum = 0;

    Timer stopwatch = Timer();
    // Row-major sum
    for(size_t i = 0; i < M_SIZE; i++) {
        for(size_t j = 0; j < M_SIZE; j++) {
            rm_sum += rm_array[M_SIZE*i + j];   // Collect
        }
    }
    uint64_t rm_time = stopwatch.click<Timer::Micros>();    // Collect time (RM)

    // Col-major sum
    for(size_t i = 0; i < M_SIZE; i++) {
        for(size_t j = 0; j < M_SIZE; j++) {    
            cm_sum += cm_array[i + M_SIZE*j];   // Collect
        }
    }
    uint64_t cm_time = stopwatch.glance<Timer::Micros>();   // Collect time (CM)

    std::cout << "(RM) " << rm_time << " " << rm_sum << "\n";
    std::cout << "(CM) " << cm_time << " " << cm_sum << "\n";
    return 0;
}
