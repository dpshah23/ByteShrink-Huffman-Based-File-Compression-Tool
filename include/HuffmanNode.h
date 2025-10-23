#pragma once
#include <cstddef>

class HuffmanNode {
public:
    unsigned char data;
    int freq;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(unsigned char d, int f)
        : data(d), freq(f), left(nullptr), right(nullptr) {}
};
