#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include <cerrno>

#include "huffman.h"

int main(int argc, char *argv[]) {
  Huffman h;

  std::ifstream ifs(argv[1], std::ios::in);

  std::ofstream ofs;
  ofs.open(argv[2]);
  if (ofs)
    ofs.open(argv[2], std::ios::out | std::ios::trunc | std::ios::binary);

  h.Compress(ifs, ofs);

  std::cout << "Compressed input file " << argv[1] <<
            " into zap file " << argv[2] << std::endl;
}
