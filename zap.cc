#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include <cerrno>

#include "huffman.h"

int main(int argc, char *argv[]) {
  Huffman h;
  std::string filename{"truncated_input"};

  std::ifstream ifs(filename,
                    std::ios::in | std::ios::trunc | std::ios::binary);

  std::ofstream ofs(argv[2], std::ios::out | std::ios::binary);

  h.Compress(ifs, ofs);

  std::cout << "Compressed input file " << argv[1] <<
            " into zap file " << argv[2] << std::endl;
}
