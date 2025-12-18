#include "HuffmanCompressor.h"
#include "HuffmanDecompressor.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <iomanip>


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

        
        auto trim = [](std::string &s) {
           
            s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
            
            s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
        };
        
        switch (choice) {
            case 1: {
                std::cout << "Enter input file path: ";
                std::cout.flush();
                std::getline(std::cin, inputFile);
                trim(inputFile);
                std::cout << "Enter output file path (.bshk): ";
                std::cout.flush();
                std::getline(std::cin, outputFile);
                trim(outputFile);
                
                // get original size
                uintmax_t originalSize = 0;
                try {
                    originalSize = std::filesystem::file_size(inputFile);
                } catch (...) {
                    // leave as 0 if cannot determine
                }

                try {
                    compressor.compress(inputFile, outputFile);
                    std::cout << "\nCompression completed successfully!" << std::endl;
                    std::cout << "Compressed file: " << outputFile << std::endl;

                    // get compressed size and print percentage change
                    uintmax_t compressedSize = 0;
                    try {
                        compressedSize = std::filesystem::file_size(outputFile);
                    } catch (...) {}

                    if (originalSize > 0 && compressedSize > 0) {
                        double pct = (1.0 - (double)compressedSize / (double)originalSize) * 100.0;
                        if (pct >= 0)
                            std::cout << "Size: " << originalSize << " -> " << compressedSize << " bytes (" << std::fixed << std::setprecision(2) << pct << "% reduction)" << std::endl;
                        else
                            std::cout << "Size: " << originalSize << " -> " << compressedSize << " bytes (" << std::fixed << std::setprecision(2) << -pct << "% increase)" << std::endl;
                    } else if (originalSize > 0) {
                        std::cout << "Original size: " << originalSize << " bytes. Could not determine compressed size." << std::endl;
                    }
                } catch (std::exception& e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                }
                break;
            }
            
            case 2: {
                std::cout << "Enter input file path (.bshk): ";
                std::cout.flush();
                std::getline(std::cin, inputFile);
                trim(inputFile);
                std::cout << "Enter output file path (.txt): ";
                std::cout.flush();
                std::getline(std::cin, outputFile);
                trim(outputFile);
                
                // get compressed (input) size
                uintmax_t compressedSize = 0;
                try {
                    compressedSize = std::filesystem::file_size(inputFile);
                } catch (...) {}

                try {
                    decompressor.decompress(inputFile, outputFile);
                    std::cout << "\nDecompression completed successfully!" << std::endl;
                    std::cout << "Decompressed file: " << outputFile << std::endl;

                    // get decompressed size and print percentage change
                    uintmax_t decompressedSize = 0;
                    try {
                        decompressedSize = std::filesystem::file_size(outputFile);
                    } catch (...) {}

                    if (compressedSize > 0 && decompressedSize > 0) {
                        double pct = (1.0 - (double)decompressedSize / (double)compressedSize) * 100.0;
                        if (pct >= 0)
                            std::cout << "Size: " << compressedSize << " -> " << decompressedSize << " bytes (" << std::fixed << std::setprecision(2) << pct << "% reduction)" << std::endl;
                        else
                            std::cout << "Size: " << compressedSize << " -> " << decompressedSize << " bytes (" << std::fixed << std::setprecision(2) << -pct << "% increase)" << std::endl;
                    }
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
