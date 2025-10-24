#pragma once
#include <string>
#include "HuffmanTree.h"

class HuffmanDecompressor {
public:
    void decompress(const std::string& inputFile, const std::string& outputFile);
};
