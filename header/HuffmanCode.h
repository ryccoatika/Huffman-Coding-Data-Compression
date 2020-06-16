//
// Created by rycco on 6/16/20.
//

#ifndef HUFFMAN_CODING_DATA_COMPRESSION_HUFFMANCODE_H
#define HUFFMAN_CODING_DATA_COMPRESSION_HUFFMANCODE_H
#include <string>
#include <map>
#include "HuffmanTreeBuilder.h"

using namespace std;

class HuffmanCode {
public:
    static priorityQueue calculateCharFrequency(const string&);
    static void calculateHuffmanCode(node, const string&, map<char, string>&);
    static void decodeHuffman(node, string&, string&);
};


#endif //HUFFMAN_CODING_DATA_COMPRESSION_HUFFMANCODE_H
