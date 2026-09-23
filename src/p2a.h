#include <cstdint>
#include <cassert>
//#include <iostream>
//#include <bitset>
/*
Source: https://stackoverflow.com/questions/3203764/bit-twiddling-hacks-interleave-bits-the-obvious-way 
Accessed: 09/22/2026 4:00
Note: Function made by referencing "Obvious" Algorithm from user polygenelubricants
*/
inline uint64_t expand(uint64_t input, uint32_t scale) {
    assert(scale > 1);
    uint64_t output = 0;
    // Iterate over bits in the input
    for(int i = 0; i < sizeof(uint64_t); i++) {
        // Check if bit is 1 or 0
        uint64_t check = input & (static_cast<uint64_t>(1) << i); // Can only have a 1 in the ith bit from the right
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
        /* Reduce space usage
        ---------------------------------------------
        // Retrieve bit at ith location from the right
        uint64_t x_shift = x & (1 << i);
        uint64_t y_shift = y & (1 << i);
        uint64_t z_shift = z & (1 << i);
        // Shift bits down
        x_shift = x_shift << i*2;
        y_shift = y_shift << i*2+1;
        z_shift = z_shift << i*2+2;

        output |= (x_shift | y_shift | z_shift);
        ---------------------------------------------*/
        output |= (x & (static_cast<uint64_t>(1) << i)) << (i*2);
        output |= (y & (static_cast<uint64_t>(1) << i)) << (i*2+1);
        output |= (z & (static_cast<uint64_t>(1) << i)) << (i*2+2);
    }
    return output;
}