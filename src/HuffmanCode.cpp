//
// Created by rycco on 6/16/20.
//

#include "../header/HuffmanCode.h"

priorityQueue HuffmanCode::calculateCharFrequency(const string& data) {
    map<char, int> freq;
    for (char ch: data) {
        freq[ch]++;
    }

    priorityQueue freqQueue;
    for (auto pair: freq) {
        node n = new Node();
        n->data = pair.first;
        n->frequency = pair.second;
        freqQueue.push(n);
    }
    return freqQueue;
}

void HuffmanCode::calculateHuffmanCode(node n, const string& code, map<char, string> &charBinary) {
    if (n == nullptr) return;
    if (n->left == nullptr && n->right == nullptr) {
        charBinary[n->data] = code;
    }
    calculateHuffmanCode(n->left, code + "0", charBinary);
    calculateHuffmanCode(n->right, code + "1", charBinary);
}

void HuffmanCode::decodeHuffman(node root, string &decode, string &originalData) {
    if (root == nullptr) return;
    if (root->left == nullptr && root->right == nullptr) {
        originalData += root->data;
        return;
    }
    char c = decode[0];
    decode = decode.substr(1);
    if (c == '0')
        decodeHuffman(root->left, decode, originalData);
    else
        decodeHuffman(root->right, decode, originalData);
}
