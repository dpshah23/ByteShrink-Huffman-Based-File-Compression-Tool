#pragma once
#include <functional>
#include <string>
#include "HuffmanTree.h"

class HuffmanDecompressor {
public:
    // Optional progress callback receives a normalized value in [0,1].
    void decompress(const std::string& inputFile,
                    const std::string& outputFile,
                    const std::function<void(double)>& progress = nullptr);
};
