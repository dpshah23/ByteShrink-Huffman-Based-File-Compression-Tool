#include "HuffmanDecompressor.h"
#include <bitset>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <stdexcept>

namespace {
    void emitProgress(const std::function<void(double)>& cb, double value) {
        if (!cb) return;
        if (value < 0.0) value = 0.0;
        if (value > 1.0) value = 1.0;
        cb(value);
    }
}

void HuffmanDecompressor::decompress(const std::string& inputFile,
                                      const std::string& outputFile,
                                      const std::function<void(double)>& progress) {
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
    emitProgress(progress, 0.05);
    
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
    
    const size_t readStep = totalBytes > 0 ? totalBytes / 40 : 0; // ~2.5% updates
    while (bytesRead < totalBytes && in.get(byte)) {
        std::bitset<8> bits(static_cast<unsigned char>(byte));
        encodedBits += bits.to_string();
        bytesRead++;
        if (readStep && (bytesRead % readStep == 0)) {
            emitProgress(progress, 0.05 + 0.35 * (static_cast<double>(bytesRead) / totalBytes));
        }
    }
    
    // Trim to actual encoded length
    encodedBits = encodedBits.substr(0, encodedLength);
    
    in.close();
    emitProgress(progress, 0.40);

    // Build Huffman tree from frequency table
    HuffmanTree tree;
    tree.build(freq);
    
    // Decode the data
    std::string decoded;
    HuffmanNode* currentNode = tree.getRoot();
    const size_t decodeStep = encodedBits.size() > 0 ? encodedBits.size() / 50 : 0; // ~2% updates
    for (size_t idx = 0; idx < encodedBits.size(); ++idx) {
        char bit = encodedBits[idx];
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

        if (decodeStep && (idx % decodeStep == 0)) {
            emitProgress(progress, 0.40 + 0.55 * (static_cast<double>(idx + 1) / encodedBits.size()));
        }
    }
    
    // Write decompressed data
    std::ofstream out(outputFile, std::ios::binary);
    if (!out.is_open())
        throw std::runtime_error("Failed to open output file");
    
    out.write(decoded.c_str(), decoded.size());
    out.close();
    emitProgress(progress, 1.0);
}
