//
// Created by rycco on 6/16/20.
//

#include "../header/Converter.h"

int Converter::binaryToDecimal(string binary) {
    reverse(binary.begin(), binary.end());
    int dec = 0;
    for(int i = 0; i < 8; i++) {
        int curr = binary[i]-48;
        dec += pow(2, i) * curr;
    }
    return dec;
}

string Converter::decimalToBinary(int dec) {
    string binary;
    while (dec != 0) {
        binary += to_string(dec % 2);
        dec = dec / 2;
    }
    binary.append(8 - binary.size(), '0');
    reverse(binary.begin(), binary.end());

    return binary;
}
