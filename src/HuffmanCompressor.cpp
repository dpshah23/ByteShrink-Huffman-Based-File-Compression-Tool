#include "HuffmanCompressor.h"
#include <bitset>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <stdexcept>

namespace {
    // Invoke progress callback safely with clamped values.
    void emitProgress(const std::function<void(double)>& cb, double value) {
        if (!cb) return;
        if (value < 0.0) value = 0.0;
        if (value > 1.0) value = 1.0;
        cb(value);
    }
}

void HuffmanCompressor::compress(const std::string& inputFile,
                                 const std::string& outputFile,
                                 const std::function<void(double)>& progress) {
    std::ifstream in(inputFile, std::ios::binary);
    if (!in.is_open())
        throw std::runtime_error("Failed to open input file");

    std::string data;
    std::string buffer(64 * 1024, '\0');
    while (in.read(buffer.data(), static_cast<std::streamsize>(buffer.size())) || in.gcount() > 0) {
        data.append(buffer.data(), static_cast<size_t>(in.gcount()));
    }
    in.close();

    const size_t dataSize = data.size();
    emitProgress(progress, 0.05); // small bump after loading file

    // Calculate frequencies with coarse-grained progress up to 0.25
    std::map<unsigned char, int> freq;
    const size_t freqStep = dataSize > 0 ? dataSize / 20 : 0; // update roughly every 5%
    for (size_t i = 0; i < data.size(); ++i) {
        unsigned char c = static_cast<unsigned char>(data[i]);
        freq[c]++;
        if (freqStep && (i % freqStep == 0)) {
            emitProgress(progress, 0.05 + 0.20 * (static_cast<double>(i + 1) / dataSize));
        }
    }
    emitProgress(progress, 0.25);

    HuffmanTree tree;
    tree.build(freq);
    auto codes = tree.getCodes();

    // Encode the data with progress from 0.25 -> 0.75
    std::string encoded;
    const size_t encodeStep = dataSize > 0 ? dataSize / 50 : 0; // finer updates ~2%
    for (size_t i = 0; i < data.size(); ++i) {
        unsigned char c = static_cast<unsigned char>(data[i]);
        encoded += codes[c];
        if (encodeStep && (i % encodeStep == 0)) {
            emitProgress(progress, 0.25 + 0.50 * (static_cast<double>(i + 1) / dataSize));
        }
    }
    emitProgress(progress, 0.75);

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
    const size_t encodedStep = encoded.size() > 0 ? encoded.size() / 50 : 0; // updates ~2%
    for (size_t i = 0; i < encoded.size(); i += 8) {
        std::string byteStr = encoded.substr(i, 8);
        while (byteStr.size() < 8) byteStr += '0';
        std::bitset<8> bits(byteStr);
        out.put(static_cast<unsigned char>(bits.to_ulong()));
        if (encodedStep && (i % encodedStep == 0)) {
            emitProgress(progress, 0.75 + 0.25 * (static_cast<double>(i + 8) / encoded.size()));
        }
    }
    
    out.close();
    emitProgress(progress, 1.0);
}
