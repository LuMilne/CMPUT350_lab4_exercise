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
    test();
    return 0; 
}
