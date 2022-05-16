#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include <cerrno>

#include "huffman.h"


int main(int argc, char *argv[]) {
  Huffman h;
  std::string filename{"truncated_output"};

  // Truncate output
  std::ofstream ofs(filename,
                    std::ios::out | std::ios::trunc | std::ios::binary);
  ofs.close();

  std::ifstream ifs(argv[1], std::ios::in | std::ios::binary);
  std::ofstream ofs(filename, std::ios::out | std::ios::binary);

  h.Decompress(ifs, ofs);

  std::cout << "Decompressed zap file " << argv[1] <<
               " into output file " << argv[2] << std::endl;
}
