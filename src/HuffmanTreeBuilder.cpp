//
// Created by rycco on 6/16/20.
//

#include "../header/HuffmanTreeBuilder.h"

bool compare::operator()(node left, node right) {
    return left->frequency > right->frequency;
}

// build huffman tree based priority queue
node HuffmanTreeBuilder::buildHuffmanTree(priorityQueue freqQueue) {
    // create heap
    while (freqQueue.size() != 1) {
        node left = freqQueue.top(); freqQueue.pop();
        node right = freqQueue.top(); freqQueue.pop();
        unsigned sum = left->frequency + right->frequency;

        node temp = new Node();
        temp->data = '\0';
        temp->frequency = sum;
        temp->left = left;
        temp->right = right;

        freqQueue.push(temp);
    }

    return freqQueue.top();
}

// rebuild huffman tree from encoded data
node HuffmanTreeBuilder::rebuildHuffmanTree(string &treeEncoded, string &charTree) {
    char bit = treeEncoded[0];
    treeEncoded = treeEncoded.substr(1);
    node root = new Node();
    if (bit == '0') {
        root->data = charTree[0];
        charTree = charTree.substr(1);
    } else {
        root->data = '\0';
        root->left = rebuildHuffmanTree(treeEncoded, charTree);
        root->right = rebuildHuffmanTree(treeEncoded, charTree);
    }
    return root;
}

void HuffmanTreeBuilder::traverseTree(node root, string &encodedTree, string &charTree) {
    if (root->left == nullptr && root->right == nullptr) {
        encodedTree += "0";
        charTree += root->data;
    } else {
        encodedTree += "1";
        traverseTree(root->left, encodedTree, charTree);
        traverseTree(root->right, encodedTree, charTree);
    }
}