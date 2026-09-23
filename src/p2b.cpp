#include "p2a.h"
#include <iostream>
#include <bitset>
#include <vector>
#include <random>
#include "timer.h"

#define M_SIZE 16
#define STRIDE 4

void p2a_test() {
    uint64_t x = 1;     // 0001
    uint64_t y = 2;     // 0010
    uint64_t z = 4;     // 0100
    uint64_t prod = morton3d(x,y,z);
    std::bitset<16> mort(prod);
    std::cout << mort << "\n";
}

// Row-major kernel
std::vector<uint64_t> Ka(uint64_t x, uint64_t y, uint64_t z) {
    std::vector<uint64_t> mat = std::vector<uint64_t>(x*y*z);
    for(uint64_t i = 0; i < x; i++) {
        for(uint64_t j = 0; j < y; j++) {
            for(uint64_t k = 0; k < z; k++) {
                uint64_t val = x*y*i + z*j + k;
                mat[val] = i+j+k;   // Populate
            }
        }
    }
    return mat;
}

// Morton-order kernel
std::vector<uint64_t> Kb(uint64_t x, uint64_t y, uint64_t z) {
    auto mat = std::vector<uint64_t>(4*4*4);
    for(uint64_t i = 0; i < x; i++) {
        for(uint64_t j = 0; j < y; j++) {
            for(uint64_t k = 0; k < z; k++) {
                mat[morton3d(i,j,k)] = i+j+k;   // Populate
            }
        }
    }
    return mat;
}

size_t rmiA(size_t x, size_t y, size_t z) {
    return x*M_SIZE*M_SIZE + y*M_SIZE + z;
}

size_t rmiK(size_t x, size_t y, size_t z) {
    return x*STRIDE*STRIDE + y*STRIDE + z;
}

size_t rmiConv(size_t x, size_t y, size_t z) {
    size_t width = M_SIZE/STRIDE;
    return (x/STRIDE)*width*width + (y/STRIDE)*width + z/STRIDE;
}

int main() {
    // Initialize contiguous 'matrices'
    auto row_mat = std::vector<uint64_t>(M_SIZE*M_SIZE*M_SIZE);
    auto morton_mat = std::vector<uint64_t>(M_SIZE*M_SIZE*M_SIZE);

    // -----
    // POPULATE
    // -----

    // Init rng
    std::mt19937_64 rng(0);
    // Row-major population
    for(int i = 0; i < M_SIZE; i++) {
        for(int j = 0; j < M_SIZE; j++) {
            for(int k = 0; k < M_SIZE; k++) {
                row_mat[M_SIZE*M_SIZE*i + M_SIZE*j + k] = rng();   // Populate
            }
        }
    }

    rng.seed(0);
    // Morton population
    for(int i = 0; i < M_SIZE; i++) {
        for(int j = 0; j < M_SIZE; j++) {
            for(int k = 0; k < M_SIZE; k++) {
                morton_mat[morton3d(i,j,k)] = rng();   // Populate
            }
        }
    }

    // -----
    // TEST
    // -----
    std::vector<uint64_t> kern_a = Ka(4,4,4);
    std::vector<uint64_t> kern_b = Kb(4,4,4);

    Timer stopwatch = Timer();
    // Row-major convolution
    uint64_t output_size = M_SIZE*M_SIZE*M_SIZE / (STRIDE*STRIDE*STRIDE);
    auto rm_output = std::vector<uint64_t>(output_size);
    for(size_t frame_x = 0; frame_x < M_SIZE; frame_x += STRIDE) {           // -----
        for(size_t frame_y = 0; frame_y < M_SIZE; frame_y += STRIDE) {       // Iterate across frames
            for(size_t frame_z = 0; frame_z < M_SIZE; frame_z += STRIDE) {   // -----
                uint64_t sum = 0;
                for(int x = 0; x < STRIDE; x++) {                       // -----
                    for(int y = 0; y < STRIDE; y++) {                   // Iterate within frames
                        for(int z = 0; z < STRIDE; z++) {               // -----
                            sum += kern_a[rmiK(x, y, z)] * row_mat[rmiA(frame_x+x, frame_y+y, frame_z+z)];
                        }
                    }
                }
                rm_output[rmiConv(frame_x, frame_y, frame_z)] = sum;                              // Store convoluted frame
            }
        }
    }
    uint64_t rm_time = stopwatch.click<Timer::Micros>();    // Collect time (RM)
/*
    std::cout << "rm_output";
    for(size_t i = 0; i < M_SIZE; i += STRIDE) {
        std::cout << "\n";
        for(size_t j = 0; j < M_SIZE; j += STRIDE) {
            for(size_t k = 0; k < M_SIZE; k += STRIDE) {
                std::cout << "(" << i/STRIDE << j/STRIDE << k/STRIDE << ")" << rm_output[rmiConv(i,j,k)] << " ";
            }
            std::cout << "\n";
        }
    }
*/

    // Morton Convolution
    auto morton_output = std::vector<uint64_t>(output_size);
    for(size_t frame_x = 0; frame_x < M_SIZE; frame_x += STRIDE) {           // -----
        for(size_t frame_y = 0; frame_y < M_SIZE; frame_y += STRIDE) {       // Iterate across frames
            for(size_t frame_z = 0; frame_z < M_SIZE; frame_z += STRIDE) {   // -----
                uint64_t sum = 0;
                for(size_t x = 0; x < STRIDE; x++) {            // -----
                    for(size_t y = 0; y < STRIDE; y++) {        // Iterate within frames
                        for(size_t z = 0; z < STRIDE; z++) {    // -----
                            sum += kern_b[morton3d(x,y,z)] * morton_mat[(frame_x+x, frame_y+y, frame_z+z)];
                        }
                    }
                }
                morton_output[rmiConv(frame_x, frame_y, frame_z)] = sum;
            }
        }
    }
    uint64_t morton_time = stopwatch.glance<Timer::Micros>();   // Collect time (CM)


    std::cout << "morton_output";
    for(size_t i = 0; i < M_SIZE; i += STRIDE) {
        std::cout << "\n";
        for(size_t j = 0; j < M_SIZE; j += STRIDE) {
            for(size_t k = 0; k < M_SIZE; k += STRIDE) {
                std::cout << "(" << i/STRIDE << j/STRIDE << k/STRIDE << ")" << morton_output[rmiConv(i,j,k)] << " ";
            }
            std::cout << "\n";
        }
    }

    for(size_t i = 0; i < M_SIZE; i += STRIDE) {
        for(size_t j = 0; j < M_SIZE; j += STRIDE) {
            for(size_t k = 0; k < M_SIZE; k += STRIDE) {
                // assert(rm_output[rmiConv(i,j,k)] == morton_output[morton3d(i,j,k)]);
            }
        }
    }


    std::cout << "Row-major " << rm_time << "ms\n";
    std::cout << "Morton    " << morton_time << "ms\n";

    return 0; 
}
