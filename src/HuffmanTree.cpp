#include "HuffmanTree.h"

struct Compare {
    bool operator()(HuffmanNode* a, HuffmanNode* b) {
        return a->freq > b->freq;
    }
};

HuffmanTree::HuffmanTree() : root(nullptr) {}

void HuffmanTree::build(const std::unordered_map<unsigned char, int>& freq) {
    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, Compare> pq;

    for (auto& p : freq)
        pq.push(new HuffmanNode(p.first, p.second));

    while (pq.size() > 1) {
        HuffmanNode* left = pq.top(); pq.pop();
        HuffmanNode* right = pq.top(); pq.pop();

        HuffmanNode* parent = new HuffmanNode('\0', left->freq + right->freq);
        parent->left = left;
        parent->right = right;
        pq.push(parent);
    }

    root = pq.top();
    generateCodes(root, "");
}

void HuffmanTree::generateCodes(HuffmanNode* node, const std::string& code) {
    if (!node) return;
    if (!node->left && !node->right) {
        codes[node->data] = code.empty() ? "0" : code;
        return;
    }
    generateCodes(node->left, code + "0");
    generateCodes(node->right, code + "1");
}

std::unordered_map<unsigned char, std::string> HuffmanTree::getCodes() const {
    return codes;
}
