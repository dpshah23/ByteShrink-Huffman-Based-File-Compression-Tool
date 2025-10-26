#include "HuffmanCompressor.h"
#include "HuffmanDecompressor.h"
#include <iostream>
#include <string>

int main() {
    std::string command;
    HuffmanCompressor compressor;
    HuffmanDecompressor decompressor;
    
    while (true) {
        std::cout << "\n=== ByteShrink - Huffman Compression Tool ===" << std::endl;
        std::cout << "1. Compress file" << std::endl;
        std::cout << "2. Decompress file" << std::endl;
        std::cout << "Type 'exit' to quit" << std::endl;
        std::cout << "Enter your choice: ";
        std::cout.flush();
        
        if (!std::getline(std::cin, command)) {
            std::cerr << "\nError reading input. Exiting..." << std::endl;
            break;
        }
        
        if (command == "exit") {
            std::cout << "Exiting... Goodbye!" << std::endl;
            break;
        }
        
        int choice = 0;
        try {
            choice = std::stoi(command);
        } catch (...) {
            std::cout << "Invalid input. Please enter 1, 2, or 'exit'." << std::endl;
            continue;
        }
        
        std::string inputFile, outputFile;
        
        switch (choice) {
            case 1: {
                std::cout << "Enter input file path: ";
                std::cout.flush();
                std::getline(std::cin, inputFile);
                std::cout << "Enter output file path (.bshk): ";
                std::cout.flush();
                std::getline(std::cin, outputFile);
                
                try {
                    compressor.compress(inputFile, outputFile);
                    std::cout << "\nCompression completed successfully!" << std::endl;
                    std::cout << "Compressed file: " << outputFile << std::endl;
                } catch (std::exception& e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                }
                break;
            }
            
            case 2: {
                std::cout << "Enter input file path (.bshk): ";
                std::cout.flush();
                std::getline(std::cin, inputFile);
                std::cout << "Enter output file path (.txt): ";
                std::cout.flush();
                std::getline(std::cin, outputFile);
                
                try {
                    decompressor.decompress(inputFile, outputFile);
                    std::cout << "\nDecompression completed successfully!" << std::endl;
                    std::cout << "Decompressed file: " << outputFile << std::endl;
                } catch (std::exception& e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                }
                break;
            }
            
            default:
                std::cout << "Invalid choice. Please enter 1, 2, or 'exit'." << std::endl;
                break;
        }
    }
    
    return 0;
}
