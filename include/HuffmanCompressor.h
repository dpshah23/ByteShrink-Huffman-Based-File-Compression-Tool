#pragma once
#include <string>
#include "HuffmanTree.h"

class HuffmanCompressor {
public:
    void compress(const std::string& inputFile, const std::string& outputFile);
};
