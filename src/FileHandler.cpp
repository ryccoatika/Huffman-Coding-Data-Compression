//
// Created by rycco on 6/15/20.
//

#include "../header/FileHandler.h"

string FileHandler::fileToString(const string &filename) {
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

void FileHandler::saveToFile(const string &data, const string &outfile) {
    ofstream output(outfile, ios::out | ios::binary);
    output.write(data.c_str(), data.size());
    output.close();
}