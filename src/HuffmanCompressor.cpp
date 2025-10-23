#include "HuffmanCompressor.h"
#include <fstream>
#include <unordered_map>
#include <bitset>
#include <stdexcept>

void HuffmanCompressor::compress(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream in(inputFile, std::ios::binary);
    if (!in.is_open())
        throw std::runtime_error("Failed to open input file");

    std::string data((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    in.close();

  
    std::unordered_map<unsigned char, int> freq;
    for (unsigned char c : data) freq[c]++;

    HuffmanTree tree;
    tree.build(freq);
    auto codes = tree.getCodes();

    std::string encoded;
    for (unsigned char c : data)
        encoded += codes[c];

    std::ofstream out(outputFile, std::ios::binary);
    for (size_t i = 0; i < encoded.size(); i += 8) {
        std::string byteStr = encoded.substr(i, 8);
        while (byteStr.size() < 8) byteStr += '0';
        std::bitset<8> bits(byteStr);
        out.put(static_cast<unsigned char>(bits.to_ulong()));
    }
    out.close();
}
