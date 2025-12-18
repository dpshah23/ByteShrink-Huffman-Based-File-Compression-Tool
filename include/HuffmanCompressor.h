#pragma once
#include <functional>
#include <string>
#include "HuffmanTree.h"

class HuffmanCompressor {
public:
    // Optional progress callback receives a normalized value in [0,1].
    void compress(const std::string& inputFile,
                  const std::string& outputFile,
                  const std::function<void(double)>& progress = nullptr);
};
