#pragma once
#include <map>
#include <queue>
#include <string>
#include "HuffmanNode.h"

class HuffmanTree {
private:
    HuffmanNode* root;
    std::map<unsigned char, std::string> codes;
    void generateCodes(HuffmanNode* node, const std::string& code);

public:
    HuffmanTree();
    ~HuffmanTree();
    void build(const std::map<unsigned char, int>& freq);
    std::map<unsigned char, std::string> getCodes() const;
    HuffmanNode* getRoot() const;
    void cleanup();
};
