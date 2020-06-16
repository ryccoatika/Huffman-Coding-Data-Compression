cmake_minimum_required(VERSION 3.16)
project(Huffman_Coding_Data_Compression)

set(CMAKE_CXX_STANDARD 20)

add_executable(Huffman_Coding_Data_Compression main.cpp src/FileHandler.cpp header/FileHandler.h src/Converter.cpp header/Converter.h src/HuffmanTreeBuilder.cpp header/HuffmanTreeBuilder.h src/HuffmanCode.cpp header/HuffmanCode.h)