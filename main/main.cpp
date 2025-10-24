#include "HuffmanCompressor.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input.txt> <output.bshk>" << std::endl;
        std::cerr << "Example: " << argv[0] << " data/input/sample.txt data/output/sample.bshk" << std::endl;
        return 1;
    }
    
    std::string inputFile = argv[1];
    std::string outputFile = argv[2];
    
    HuffmanCompressor compressor;
    try {
        compressor.compress(inputFile, outputFile);
        std::cout << "Compression done successfully!" << std::endl;
        std::cout << "Compressed file: " << outputFile << std::endl;
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
