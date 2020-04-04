#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <queue>
#include <algorithm>
#include <cmath>
#define FILENAME "compressor"

using namespace std;

typedef struct Node *node;
struct Node {
    char data;
    unsigned frequency;

    node left = nullptr, right = nullptr;
};

struct compare {
    bool operator()(node left, node right) {
        return left->frequency > right->frequency;
    }
};
typedef priority_queue<node, vector<node>, compare> priorityQueue;

void showUsage();
string fileToString(const string&);
void saveToFile(const string&, const string&);

// function for encode
void encode(const string&, const string&);
priorityQueue calculateCharFrequency(const string&);
node buildHuffmanTree(priorityQueue);
void calculateHuffmanCode(node, const string&, map<char, string>&);
void traverseTree(node, string&, string&);
int binaryToDecimal(string);

// function for decode
void decode(string&, const string&);
string decimalToBinary(int);
node rebuildHuffmanTree(string&, string&);
void decodeHuffman(node, string&, string&);

int main(int argc, char *argv[]) {
    if (argc == 1) {
        showUsage();
        return -1;
    }
    if (string(argv[1]) == "-h") {
        showUsage();
        return 0;
    }
    if ((string(argv[1]) != "-e" && string(argv[1]) != "-d") || string(argv[3]) != "-o") {
        cout << FILENAME << ": unrecognized options" << endl;
        return -1;
    }

    if (string(argv[1]) == "-e") {
        string outFile = argv[4];
        string rawData = fileToString(argv[2]);
        if (rawData == "-1") {
            cout << "error reading the file" << endl;
            return -1;
        }
        encode(rawData, outFile);
    } else if (string(argv[1]) == "-d") {
        string outFile = argv[4];
        string encodedData = fileToString(argv[2]);
        if (encodedData == "-1") {
            cout << "error reading the file" << endl;
            return -1;
        }
        decode(encodedData, outFile);
    }

    return 0;
}

void showUsage() {
    cout << "Usage:" << endl;
    cout << "  " << FILENAME << " [options] <file>..." << endl << endl;
    cout << "Compress data using huffman coding and save the result to the file" << endl << endl;
    cout << "OPTIONS:" << endl;
    cout << "  -h\t\t: " << "show help" << endl;
    cout << "  -e <file>\t: " << "encode and compress the file" << endl;
    cout << "  -d <file>\t: " << "decode and decompress the file" << endl;
    cout << "  -o <file>\t: " << "save the result to the file" << endl << endl;
    cout << "EXAMPLES:" << endl;
    cout << "  " << FILENAME << " -e <file> -o <file>" << endl;
    cout << "  " << FILENAME << " -d <file> -o <file>" << endl;
}

string fileToString(const string& filename) {
    ifstream file(filename);

    if (!file.good()) {
        file.close();
        return "-1";
    }

    stringstream sstream;
    sstream << file.rdbuf();
    string data = sstream.str();

    file.close();
    sstream.clear();

    return data;
}

void saveToFile(const string &data, const string &outFile) {
    ofstream output(outFile, ios::out | ios::binary);
    output.write(data.c_str(), data.size());
    output.close();
}

// function for encode

void encode(const string &rawData, const string &outFile) {
    // calculating frequency
    priorityQueue pq = calculateCharFrequency(rawData);

    // build huffman tree
    node root = buildHuffmanTree(pq);

    // generate new binary char by the tree
    map<char, string> charBinary;
    calculateHuffmanCode(root, "", charBinary);

    // store huffman tree to string
    string treeEncoded;
    string leafTree;
    traverseTree(root, treeEncoded, leafTree);

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
        encoded += (char) binaryToDecimal(treeEncoded.substr(0, 8));
        treeEncoded = treeEncoded.substr(8);
    }
    int reminder = 8 - treeEncoded.size();
    treeEncoded.append(reminder, '0');
    encoded += (char) binaryToDecimal(treeEncoded);

    // store the char leaf of the tree
    for (char c: leafTree) {
        encoded += c;
    }

    // store encoded raw data into string
    while (rawDataEncoded.size() >= 8) {
        encoded += (char) binaryToDecimal(rawDataEncoded.substr(0, 8));
        rawDataEncoded = rawDataEncoded.substr(8);
    }
    reminder =  8 - rawDataEncoded.size();
    rawDataEncoded.append(reminder, '0');
    encoded += (char) binaryToDecimal(rawDataEncoded);
    encoded += (char) reminder;
    saveToFile(encoded, outFile);
}

priorityQueue calculateCharFrequency(const string& data) {
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

node buildHuffmanTree(priorityQueue freqQueue) {
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

void calculateHuffmanCode(node n, const string& code, map<char, string> &charBinary) {
    if (n == nullptr) return;
    if (n->left == nullptr && n->right == nullptr) {
        charBinary[n->data] = code;
    }
    calculateHuffmanCode(n->left, code + "0", charBinary);
    calculateHuffmanCode(n->right, code + "1", charBinary);
}

void traverseTree(node root, string &encodedTree, string &charTree) {
    if (root->left == nullptr && root->right == nullptr) {
        encodedTree += "0";
        charTree += root->data;
    } else {
        encodedTree += "1";
        traverseTree(root->left, encodedTree, charTree);
        traverseTree(root->right, encodedTree, charTree);
    }
}

int binaryToDecimal(string binary) {
    reverse(binary.begin(), binary.end());
    int dec = 0;
    for(int i = 0; i < 8; i++) {
        int curr = binary[i]-48;
        dec += pow(2, i) * curr;
    }
    return dec;
}

// function for decode

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
        encodedTree += decimalToBinary(c);
    }
    encodedTree = encodedTree.substr(0, sizeNode);
    encodedData = encodedData.substr(sizeNode / 8 + reminder);

    // extract the leaf character of the tree
    string charTree = encodedData.substr(0, size);
    encodedData = encodedData.substr(size);

    // rebuild the huffman tree
    node root = rebuildHuffmanTree(encodedTree, charTree);

    // convert encodedData decimal to binary
    string data;
    for (unsigned char c: encodedData) {
        data += decimalToBinary(c);
    }
    reminder = (unsigned char) encodedData[encodedData.size()-1];
    data = data.substr(0, data.size() - 8 - reminder);

    string rawData;
    while (!data.empty())
        decodeHuffman(root, data, rawData);

    saveToFile(rawData, outFile);
}

string decimalToBinary(int dec) {
    string binary;
    while (dec != 0) {
        binary += to_string(dec % 2);
        dec = dec / 2;
    }
    binary.append(8 - binary.size(), '0');
    reverse(binary.begin(), binary.end());

    return binary;
}

node rebuildHuffmanTree(string &treeEncoded, string &charTree) {
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

void decodeHuffman(node root, string &decode, string &originalData) {
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