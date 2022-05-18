#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include <cerrno>

#include "huffman.h"

int main(int argc, char *argv[]) {
  Huffman h;

  std::ifstream ifs(argv[1], std::ios::in | std::ios::binary);
  // Truncate output
  std::ofstream ofs(argv[2], std::ios::out | std::ios::trunc);

  h.Decompress(ifs, ofs);

  std::cout << "Decompressed zap file " << argv[1] <<
               " into output file " << argv[2] << std::endl;


  ifs.close();
  ofs.close();
}
