#include "HuffmanDecompressor.h"
#include <fstream>
#include <map>
#include <bitset>
#include <iostream>
#include <stdexcept>

void HuffmanDecompressor::decompress(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream in(inputFile, std::ios::binary);
    if (!in.is_open())
        throw std::runtime_error("Failed to open input file");

    // Read frequency table size
    size_t freqSize;
    in.read(reinterpret_cast<char*>(&freqSize), sizeof(freqSize));
    
    // Read frequency table
    std::map<unsigned char, int> freq;
    for (size_t i = 0; i < freqSize; i++) {
        unsigned char c = in.get();
        int frequency;
        in.read(reinterpret_cast<char*>(&frequency), sizeof(frequency));
        freq[c] = frequency;
    }
    
    // Read original data size
    size_t originalSize;
    in.read(reinterpret_cast<char*>(&originalSize), sizeof(originalSize));
    
    // Read encoded data length
    size_t encodedLength;
    in.read(reinterpret_cast<char*>(&encodedLength), sizeof(encodedLength));
    
    // Read encoded data
    std::string encodedBits;
    char byte;
    size_t bytesRead = 0;
    size_t totalBytes = (encodedLength + 7) / 8; // Round up to get number of bytes
    
    while (bytesRead < totalBytes && in.get(byte)) {
        std::bitset<8> bits(static_cast<unsigned char>(byte));
        encodedBits += bits.to_string();
        bytesRead++;
    }
    
    // Trim to actual encoded length
    encodedBits = encodedBits.substr(0, encodedLength);
    
    in.close();

    // Build Huffman tree from frequency table
    HuffmanTree tree;
    tree.build(freq);
    
    // Decode the data
    std::string decoded;
    HuffmanNode* currentNode = tree.getRoot();
    
    for (char bit : encodedBits) {
        if (bit == '0') {
            currentNode = currentNode->left;
        } else {
            currentNode = currentNode->right;
        }
        
        // If we reached a leaf node, add the character and reset
        if (!currentNode->left && !currentNode->right) {
            decoded += currentNode->data;
            currentNode = tree.getRoot();
        }
    }
    
    // Write decompressed data
    std::ofstream out(outputFile, std::ios::binary);
    if (!out.is_open())
        throw std::runtime_error("Failed to open output file");
    
    out.write(decoded.c_str(), decoded.size());
    out.close();
}
