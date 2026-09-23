#include "p2a.h"
#include <iostream>
#include <bitset>

void test() {
    uint64_t test = 0b1101;
    std::bitset<16> out(test);
    std::cout << out << "\n";
    uint64_t result = expand(test, 3);
    std::bitset<16> out2(result);
    std::cout << out2 << "\n";
}

int main() { 
    uint64_t x = 1;     // 0001
    uint64_t y = 2;     // 0010
    uint64_t z = 4;     // 0100
    uint16_t prod = morton3d(x,y,z);
    std::bitset<16> mort(prod);
    std::cout << mort << "\n";
    return 0; 
}
