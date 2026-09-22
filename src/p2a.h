#include <cstdint>
#include <cassert>
//#include <iostream>
//#include <bitset>


/*
Source: https://stackoverflow.com/questions/3203764/bit-twiddling-hacks-interleave-bits-the-obvious-way 
    */
uint64_t expand(uint64_t input, uint32_t scale) {
    assert(scale > 1);
    uint64_t output = 0;
    // Iterate over bits in the input
    for(int i = 0; i < sizeof(uint64_t); i++) {
        // Check if bit is 1 or 0
        uint64_t check = input & (1 << i); // Can only have a 1 in the ith bit from the right
        // Shift the bit to its new position
        check = (check << (scale-1)*i);
        //std::bitset<16> show(check);
        //std::cout << show << "\n";
        output |= check;
    }
    return output;
}

inline uint64_t morton3d(uint64_t x, uint64_t y, uint64_t z) {
    uint64_t output = 0;
    // Iterate over the bits in the inputs
    for(int i = 0; i < sizeof(uint64_t); i++) {
        uint64_t x_shift = x & (1 << i)
    }
    return 0;
}