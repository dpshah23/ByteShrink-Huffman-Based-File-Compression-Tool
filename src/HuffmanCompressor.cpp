#include "HuffmanCompressor.h"
#include <fstream>
#include <map>
#include <bitset>
#include <iostream>
#include <stdexcept>

void HuffmanCompressor::compress(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream in(inputFile, std::ios::binary);
    if (!in.is_open())
        throw std::runtime_error("Failed to open input file");

    std::string data((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    in.close();

    // Calculate frequencies
    std::map<unsigned char, int> freq;
    for (unsigned char c : data) freq[c]++;

    HuffmanTree tree;
    tree.build(freq);
    auto codes = tree.getCodes();

    // Encode the data
    std::string encoded;
    for (unsigned char c : data)
        encoded += codes[c];

    std::ofstream out(outputFile, std::ios::binary);
    
    // Write frequency table size
    size_t freqSize = freq.size();
    out.write(reinterpret_cast<const char*>(&freqSize), sizeof(freqSize));
    
    // Write frequency table
    for (auto& p : freq) {
        out.put(p.first);  // character
        out.write(reinterpret_cast<const char*>(&p.second), sizeof(p.second));  // frequency
    }
    
    // Write original data size
    size_t originalSize = data.size();
    out.write(reinterpret_cast<const char*>(&originalSize), sizeof(originalSize));
    
    // Write encoded data length
    size_t encodedLength = encoded.size();
    out.write(reinterpret_cast<const char*>(&encodedLength), sizeof(encodedLength));
    
    // Write encoded data as bytes
    for (size_t i = 0; i < encoded.size(); i += 8) {
        std::string byteStr = encoded.substr(i, 8);
        while (byteStr.size() < 8) byteStr += '0';
        std::bitset<8> bits(byteStr);
        out.put(static_cast<unsigned char>(bits.to_ulong()));
    }
    
    out.close();
}
