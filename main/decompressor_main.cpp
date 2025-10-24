#include "HuffmanDecompressor.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input.bshk> <output.txt>" << std::endl;
        std::cerr << "Example: " << argv[0] << " data/output/sample.bshk data/decompressed/sample.txt" << std::endl;
        return 1;
    }
    
    std::string inputFile = argv[1];
    std::string outputFile = argv[2];
    
    HuffmanDecompressor decompressor;
    try {
        decompressor.decompress(inputFile, outputFile);
        std::cout << "Decompression completed successfully!" << std::endl;
        std::cout << "Decompressed file: " << outputFile << std::endl;
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
