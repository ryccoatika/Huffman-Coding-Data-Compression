//
// Created by rycco on 6/15/20.
//

#ifndef HUFFMAN_CODING_DATA_COMPRESSION_FILEHANDLER_H
#define HUFFMAN_CODING_DATA_COMPRESSION_FILEHANDLER_H
#include <string>
#include <fstream>
#include <sstream>


using namespace std;

class FileHandler {
public:
    static string fileToString(const string&);
    static void saveToFile(const string&, const string&);
};


#endif //HUFFMAN_CODING_DATA_COMPRESSION_FILEHANDLER_H
