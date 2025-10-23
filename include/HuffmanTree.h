#pragma once
#include <unordered_map>
#include <queue>
#include <string>
#include "HuffmanNode.h"

class HuffmanTree {
private:
    HuffmanNode* root;
    std::unordered_map<unsigned char, std::string> codes;
    void generateCodes(HuffmanNode* node, const std::string& code);

public:
    HuffmanTree();
    void build(const std::unordered_map<unsigned char, int>& freq);
    std::unordered_map<unsigned char, std::string> getCodes() const;
};
