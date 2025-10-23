#include "HuffmanCompressor.h"
#include <iostream>

int main() {
    HuffmanCompressor compressor;
    try {
        compressor.compress("data/input/sample.txt", "data/output/sample.bshk");
        std::cout << "Compression done successfully!" << std::endl;
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
