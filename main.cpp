#include <iostream>
#include "header/FileHandler.h"
#include "header/Converter.h"
#include "header/HuffmanTreeBuilder.h"
#include "header/HuffmanCode.h"
#include <algorithm>

using namespace std;

void showUsage(string);
void encode(const string&, const string&);
void decode(string&, const string&);

int main(int argc, char *argv[]) {
    if (argc == 1) {
        showUsage(argv[0]);
        return -1;
    }
    if (string(argv[1]) == "-h") {
        showUsage(argv[0]);
        return 0;
    }
    if ((string(argv[1]) != "-e" && string(argv[1]) != "-d") || string(argv[3]) != "-o") {
        cout << argv[0] << ": unrecognized options" << endl;
        return -1;
    }

    if (string(argv[1]) == "-e") {
        string outFile = argv[4];
        string rawData = FileHandler::fileToString(argv[2]);
        if (rawData == "-1") {
            cout << "error reading the file" << endl;
            return -1;
        }
        encode(rawData, outFile);
    } else if (string(argv[1]) == "-d") {
        string outFile = argv[4];
        string encodedData = FileHandler::fileToString(argv[2]);
        if (encodedData == "-1") {
            cout << "error reading the file" << endl;
            return -1;
        }
        decode(encodedData, outFile);
    }

    return 0;
}

void showUsage(string filename) {
    cout << "Usage:" << endl;
    cout << "  " << filename << " [options] <file>..." << endl << endl;
    cout << "Compress data using huffman coding and save the result to the file" << endl << endl;
    cout << "OPTIONS:" << endl;
    cout << "  -h\t\t: " << "show help" << endl;
    cout << "  -e <file>\t: " << "encode and compress the file" << endl;
    cout << "  -d <file>\t: " << "decode and decompress the file" << endl;
    cout << "  -o <file>\t: " << "save the result to the file" << endl << endl;
    cout << "EXAMPLES:" << endl;
    cout << "  " << filename << " -e <file> -o <file>" << endl;
    cout << "  " << filename << " -d <file> -o <file>" << endl;
}

// function for encode
void encode(const string &rawData, const string &outFile) {
    // calculating frequency
    priorityQueue pq = HuffmanCode::calculateCharFrequency(rawData);

    // build huffman tree
    node root = HuffmanTreeBuilder::buildHuffmanTree(pq);

    // generate new binary char by the tree
    map<char, string> charBinary;
    HuffmanCode::calculateHuffmanCode(root, "", charBinary);

    // store huffman tree to string
    string treeEncoded;
    string leafTree;
    HuffmanTreeBuilder::traverseTree(root, treeEncoded, leafTree);

    // convert raw data into new binary
    string rawDataEncoded;
    for (char c: rawData)
        rawDataEncoded += charBinary[c];

    // for store encoded data
    string encoded;

    // store the size of the tree
    encoded += (char) pq.size();
    // store the node of the tree
    while (treeEncoded.size() >= 8) {
        encoded += (char) Converter::binaryToDecimal(treeEncoded.substr(0, 8));
        treeEncoded = treeEncoded.substr(8);
    }
    int reminder = 8 - treeEncoded.size();
    treeEncoded.append(reminder, '0');
    encoded += (char) Converter::binaryToDecimal(treeEncoded);

    // store the char leaf of the tree
    for (char c: leafTree) {
        encoded += c;
    }

    // store encoded raw data into string
    while (rawDataEncoded.size() >= 8) {
        encoded += (char) Converter::binaryToDecimal(rawDataEncoded.substr(0, 8));
        rawDataEncoded = rawDataEncoded.substr(8);
    }
    reminder =  8 - rawDataEncoded.size();
    rawDataEncoded.append(reminder, '0');
    encoded += (char) Converter::binaryToDecimal(rawDataEncoded);
    encoded += (char) reminder;
    FileHandler::saveToFile(encoded, outFile);
}

void decode(string &encodedData, const string &outFile) {
    // get the size of the tree
    int size = encodedData[0];
    encodedData = encodedData.substr(1);

    // get the size of the tree
    int sizeNode = size*2-1;

    // extract the huffman tree part
    int reminder = 0;
    if (sizeNode % 8 != 0) reminder = 1;
    string encodedTree;
    for (unsigned char c: encodedData.substr(0, sizeNode / 8 + reminder )) {
        // convert to binary
        encodedTree += Converter::decimalToBinary(c);
    }
    encodedTree = encodedTree.substr(0, sizeNode);
    encodedData = encodedData.substr(sizeNode / 8 + reminder);

    // extract the leaf character of the tree
    string charTree = encodedData.substr(0, size);
    encodedData = encodedData.substr(size);

    // rebuild the huffman tree
    node root = HuffmanTreeBuilder::rebuildHuffmanTree(encodedTree, charTree);

    // convert encodedData decimal to binary
    string data;
    for (unsigned char c: encodedData) {
        data += Converter::decimalToBinary(c);
    }
    reminder = (unsigned char) encodedData[encodedData.size()-1];
    data = data.substr(0, data.size() - 8 - reminder);

    string rawData;
    while (!data.empty())
        HuffmanCode::decodeHuffman(root, data, rawData);

    FileHandler::saveToFile(rawData, outFile);
}