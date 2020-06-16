//
// Created by rycco on 6/16/20.
//

#ifndef HUFFMAN_CODING_DATA_COMPRESSION_HUFFMANTREEBUILDER_H
#define HUFFMAN_CODING_DATA_COMPRESSION_HUFFMANTREEBUILDER_H
#include <vector>
#include <queue>
#include <string>

using namespace std;

typedef struct Node *node;
struct Node {
    char data;
    unsigned frequency;

    node left = nullptr, right = nullptr;
};

struct compare {
    bool operator()(node, node);
};
typedef priority_queue<node, vector<node>, compare> priorityQueue;

class HuffmanTreeBuilder {
public:
    static node buildHuffmanTree(priorityQueue);
    static node rebuildHuffmanTree(string&, string&);
    static void traverseTree(node, string&, string&);
};


#endif //HUFFMAN_CODING_DATA_COMPRESSION_HUFFMANTREEBUILDER_H
